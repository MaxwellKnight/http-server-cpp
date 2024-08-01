#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <functional>

class TCPServer {
public:
    TCPServer(int port);
    ~TCPServer();

    void start(const std::function<void(int)>& client_handler);

private:
    int port;
    int server_fd;

    void setup();
    void tcp_listen(const std::function<void(int)>& client_handler);
    int tcp_accept();
    void tcp_close();
};

#endif // TCPSERVER_H
