#include "../includes/http_server.h"

int main() {
    HTTPServer server(8080, 4); // Port 8080 with 4 threads in the thread pool
    server.start();
    return 0;
}
