import socket

server = 'localhost'
port = 8080

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    sock.connect((server, port))
    request = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n"
    sock.sendall(request.encode())
    response = sock.recv(4096)
    print(response.decode())
finally:
    # Close the socket
    sock.close()
