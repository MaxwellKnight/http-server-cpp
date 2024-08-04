#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <functional>

class TCPServer {
public:
    TCPServer(int port);
    ~TCPServer();
    void start(const std::function<void(int)>& client_handler);
	std::string get_client_ip(int client_socket);

private:
    int port;
    int server_fd;

private:
    void setup();
    void tcp_listen(const std::function<void(int)>& client_handler);
    int tcp_accept();
    void tcp_close();
};

#endif // TCPSERVER_H
