#include "../includes/http_server.h"
#include <sstream>
#include <unistd.h>
#include <cstring>

HTTPServer::HTTPServer(int port, size_t num_threads) 
    : server(port), pool(num_threads) {}

void HTTPServer::start() {
   server.start([this](int client_socket) { handle_client(client_socket); });
}

void HTTPServer::handle_client(int client_socket) {
    pool.enqueue([client_socket, this] { 
        std::string request = read_fd(client_socket);
        std::string response = handle_request(request);
		write_fd(client_socket, response);
        close(client_socket);
    });
}

std::string HTTPServer::read_fd(int client_socket) {
    char buffer[BUFF_SIZE] = {0};
    read(client_socket, buffer, sizeof(buffer));
    return std::string(buffer);
}

std::string HTTPServer::handle_request(const std::string& request) {
    std::istringstream request_stream(request);
    std::string method, path;
    request_stream >> method >> path;

    if (method == "GET" && path == "/") {
        return "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Hello, World!</h1></body></html>";
    } else {
        return "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n<html><body><h1>404 Not Found</h1></body></html>";
    }
}

void HTTPServer::write_fd(int client_socket, const std::string& response) {
    send(client_socket, response.c_str(), response.length(), 0);
}
