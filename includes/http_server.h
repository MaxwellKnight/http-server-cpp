#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "../includes/tcp_server.h"
#include "../includes/thread_pool.h"
#include <string>
#include <vector>
#include <mutex>

class HTTPServer {
public:
    HTTPServer(int port, size_t num_threads);

    void start();

private:
    TCPServer server;
    ThreadPool pool;
    std::vector<double> response_times;
    std::mutex response_mutex;

    // Client handling
    void handle_client(int client_socket);
    void process_client_connection(int client_socket);
    
    // Request/Response handling
    std::string read_fd(int client_socket);
    bool write_fd(int client_socket, const std::string& response);
    std::string handle_request(const std::string& request);
    
    // Response generation
    std::string generate_html_response();
    std::string generate_stats_response();
    std::string generate_stats_json();
    void send_error_response(int client_socket, int status_code, const std::string& message);
    
    // Stats tracking
    void update_response_times(double duration);
};

#endif // HTTP_SERVER_H
