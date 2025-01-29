#include "../includes/http_server.h"
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <numeric>
#include <thread>

static const size_t MAX_PENDING_TASKS = 1000;
static const size_t MAX_REQUEST_SIZE = 1024 * 1024; // 1MB
static const int SOCKET_TIMEOUT_SECONDS = 30;
static const size_t MAX_RESPONSE_TIMES = 50;

HTTPServer::HTTPServer(int port, size_t num_threads) 
    : server(port), pool(num_threads) {
    response_times.reserve(MAX_RESPONSE_TIMES);
}

void HTTPServer::start() {
    server.start([this](int client_socket) { 
        handle_client(client_socket); 
    });
}

void HTTPServer::handle_client(int client_socket) {
    struct timeval timeout;      
    timeout.tv_sec = SOCKET_TIMEOUT_SECONDS;
    timeout.tv_usec = 0;
    if (setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        close(client_socket);
        return;
    }

    if (pool.get_pending_tasks() > MAX_PENDING_TASKS) {
        send_error_response(client_socket, 503, "Service Unavailable");
        close(client_socket);
        return;
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    pool.enqueue([this, client_socket, start_time] {
        try {
            process_client_connection(client_socket, start_time);
        } catch (const std::exception& e) {
            try {
                send_error_response(client_socket, 500, "Internal Server Error");
            } catch (...) {}
            close(client_socket);
        } catch (...) {
            close(client_socket);
        }
    });
}

void HTTPServer::process_client_connection(
    int client_socket,
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time) 
{
    try {
        std::string request = read_fd(client_socket);
        if (request.empty()) {
            throw std::runtime_error("Empty request received");
        }

        // Simulate CPU-intensive work
        const int matrix_size = 300;
        std::vector<double> results(matrix_size * matrix_size, 0.0);
        for (int i = 0; i < matrix_size; i++) {
            for (int j = 0; j < matrix_size; j++) {
                double sum = 0.0;
                for (int k = 0; k < 1000; k++) {
                    sum += std::sin(i * j * k) * std::cos(i + j + k);
                }
                results[i * matrix_size + j] = sum;
            }
        }

        std::string response = handle_request(request);
        write_fd(client_socket, response);

        auto end_time = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double, std::milli>(end_time - start_time).count();
        update_response_times(duration);

        close(client_socket);
    } catch (const std::exception& e) {
        throw;
    }
}

std::string HTTPServer::read_fd(int client_socket) {
    char buffer[4096];
    std::string request;
    size_t total_size = 0;

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);

        if (bytes_read < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                throw std::runtime_error("Socket read timeout");
            }
            throw std::runtime_error("Socket read error");
        }

        if (bytes_read == 0) {
            break;
        }

        total_size += bytes_read;
        if (total_size > MAX_REQUEST_SIZE) {
            throw std::runtime_error("Request too large");
        }

        request.append(buffer, bytes_read);
        if (request.find("\r\n\r\n") != std::string::npos) {
            break;
        }
    }

    return request;
}

bool HTTPServer::write_fd(int client_socket, const std::string& response) {
    size_t total_sent = 0;
    while (total_sent < response.length()) {
        ssize_t sent = send(client_socket, 
                          response.c_str() + total_sent,
                          response.length() - total_sent,
                          MSG_NOSIGNAL);

        if (sent <= 0) {
            return false;
        }

        total_sent += sent;
    }
    return true;
}

void HTTPServer::update_response_times(double duration) {
    std::lock_guard<std::mutex> lock(response_mutex);
    response_times.push_back(duration);
    while (response_times.size() > MAX_RESPONSE_TIMES) {
        response_times.erase(response_times.begin());
    }
}

std::string HTTPServer::generate_stats_json() {
    std::lock_guard<std::mutex> lock(response_mutex);

    double avg = 0;
    double max_time = 0;
    size_t count = response_times.size();

    if (!response_times.empty()) {
        avg = std::accumulate(response_times.begin(), response_times.end(), 0.0) / count;
        max_time = *std::max_element(response_times.begin(), response_times.end());
    }

    std::stringstream json;
    json << std::fixed << std::setprecision(2);
    json << "{\"times\":[";
    for (size_t i = 0; i < response_times.size(); ++i) {
        json << response_times[i];
        if (i < response_times.size() - 1) json << ",";
    }
    json << "],\"avg\":" << avg 
         << ",\"max\":" << max_time 
         << ",\"count\":" << count 
         << "}";

    return json.str();
}

void HTTPServer::send_error_response(int client_socket, int status_code, const std::string& message) {
    std::stringstream response;
    response << "HTTP/1.1 " << status_code << " " << message << "\r\n"
             << "Content-Type: text/html\r\n\r\n"
             << "<html><body><h1>" << status_code << " " << message << "</h1></body></html>";

    write_fd(client_socket, response.str());
}

std::string HTTPServer::handle_request(const std::string& request) {
    std::istringstream request_stream(request);
    std::string method, path, protocol;
    request_stream >> method >> path >> protocol;

    if (method.empty() || path.empty()) {
        throw std::runtime_error("Invalid request format");
    }

    if (method == "GET") {
        if (path == "/") {
            return generate_html_response();
        } else if (path == "/stats") {
            return generate_stats_response();
        }
    }
    
    throw std::runtime_error("Not Found");
}

std::string HTTPServer::generate_stats_response() {
    std::string stats = generate_stats_json();
    return "HTTP/1.1 200 OK\r\n"
           "Content-Type: application/json\r\n"
           "Content-Length: " + std::to_string(stats.length()) + "\r\n"
           "\r\n" + stats;
}

std::string HTTPServer::generate_html_response() {
    std::ifstream file("src/dashboard.html");
    if (!file.is_open()) {
        throw std::runtime_error("Dashboard template not found");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string html_content = buffer.str();

    return "HTTP/1.1 200 OK\r\n"
           "Content-Type: text/html\r\n"
           "Content-Length: " + std::to_string(html_content.length()) + "\r\n"
           "\r\n" + html_content;
}
