#include "../includes/http_server.h"

int main() {
    HTTPServer server(8080, 1); 
    server.start();
    return 0;
}
