#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include "tcp_server.h"
#include "thread_pool.h"

class HTTPServer {
public:
    HTTPServer(int port, size_t num_threads);
    void start();

private:
    TCPServer server;
    ThreadPool pool;
    std::vector<double> response_times;
    std::mutex response_mutex;
    
    // Connection timing structure
    struct ClientTask {
        int socket;
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
        
        ClientTask(int s, std::chrono::time_point<std::chrono::high_resolution_clock> t) 
            : socket(s), start_time(t) {}
    };

    // Client handling methods
    void handle_client(int client_socket);
    void process_client_connection(int client_socket, 
                                 std::chrono::time_point<std::chrono::high_resolution_clock> start_time);

    // Request/Response handling
    std::string read_fd(int client_socket);
    bool write_fd(int client_socket, const std::string& response);
    std::string handle_request(const std::string& request);
    void send_error_response(int client_socket, int status_code, const std::string& message);

    // Response generation
    std::string generate_html_response();
    std::string generate_stats_response();
    std::string generate_stats_json();

    // Metrics tracking
    void update_response_times(double duration);
};

#endif // HTTP_SERVER_H
