#include "../includes/http_server.h"

int main() {
  HTTPServer server(8080, 2);
  server.start();
}
