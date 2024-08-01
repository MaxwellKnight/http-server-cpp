
#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "../includes/tcp_server.h"
#include "../includes/thread_pool.h"
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF_SIZE 4096

class HTTPServer {
public:
    HTTPServer(int port, size_t num_threads);

    void start();

private:
    TCPServer server;
    ThreadPool pool;

    void handle_client(int client_socket);
    std::string read_fd(int client_socket);
    std::string handle_request(const std::string& request);
    void write_fd(int client_socket, const std::string& response);
};

#endif // HTTPSERVER_H
