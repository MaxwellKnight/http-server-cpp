#include "../includes/tcp_server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

TCPServer::TCPServer(int port) : port(port), server_fd(-1) {}

TCPServer::~TCPServer() {
    tcp_close();
}

void TCPServer::start(const std::function<void(int)>& client_handler) {
    setup();
    tcp_listen(client_handler);
}

void TCPServer::setup() {
    struct sockaddr_in address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) != 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void TCPServer::tcp_listen(const std::function<void(int)>& client_handler) {
    if (listen(server_fd, 3) != 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

	printf("server is listening on port: %d\n", port);
    while (true) {
        int client_socket = tcp_accept();
        if (client_socket >= 0) {
            client_handler(client_socket);
        }
    }
}

int TCPServer::tcp_accept() {
    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);
    int client_socket = accept(server_fd, (struct sockaddr*)&client_address, &client_len);
    if (client_socket < 0) {
        perror("accept failed");
    }
    return client_socket;
}

void TCPServer::tcp_close() {
    if (server_fd != -1) {
        close(server_fd);
    }
}
