import socket
import time
from concurrent.futures import ThreadPoolExecutor

def make_request():
    server = 'localhost'
    port = 8080
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        sock.connect((server, port))
        request = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n"
        sock.sendall(request.encode())
        response = sock.recv(4096)
        return len(response)
    finally:
        sock.close()

def run_concurrent_requests(num_concurrent=10, total_requests=100):
    with ThreadPoolExecutor(max_workers=num_concurrent) as executor:
        start_time = time.time()
        futures = [executor.submit(make_request) for _ in range(total_requests)]
        completed = 0
        for future in futures:
            future.result()
            completed += 1
            if completed % 10 == 0:
                print(f"Completed {completed}/{total_requests} requests")

    duration = time.time() - start_time
    print(f"\nCompleted {total_requests} requests in {duration:.2f} seconds")
    print(f"Average throughput: {total_requests/duration:.2f} requests/second")

if __name__ == "__main__":
    # Run 100 requests with 10 concurrent connections
    run_concurrent_requests(num_concurrent=10, total_requests=100)
