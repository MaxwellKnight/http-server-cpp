
# README: Concurrent HTTP Server with Thread Pool

## Overview
This project implements a concurrent HTTP server in C++ using a `TCPServer` for handling TCP connections, a `ThreadPool` for concurrent task execution, and an `HTTPServer` that utilizes both to handle HTTP requests efficiently. The server adheres to the SOLID principles, emphasizing modularity and maintainability.

## Features
- **TCP Server**: Handles incoming TCP connections and provides a basic infrastructure for a network server.
- **Thread Pool**: Manages a pool of worker threads to handle tasks concurrently, improving performance and responsiveness.
- **HTTP Server**: Extends the TCP server to handle HTTP requests and responses, supporting HTTP/1.1.

## Components

### TCPServer
- **Purpose**: Manages TCP connections, listens for incoming requests, and accepts client connections.
- **Usage**: Provides a foundation for network communication.
- **Key Methods**:
  - `start(const std::function<void(int)>& clientHandler)`: Starts the server and handles incoming connections with the provided client handler.

### ThreadPool
- **Purpose**: Manages a pool of worker threads to execute tasks concurrently.
- **Usage**: Efficiently handles multiple tasks in parallel, improving performance.
- **Key Methods**:
  - `enqueue(std::function<void()> task)`: Adds a task to the queue for processing by a worker thread.

### HTTPServer
- **Purpose**: Extends the TCPServer to handle HTTP/1.1 requests and responses.
- **Usage**: Provides an HTTP server implementation that can handle multiple requests concurrently using the ThreadPool.
- **Key Methods**:
  - `start()`: Starts the HTTP server and begins accepting and handling HTTP requests.

## Building and Running

### Prerequisites
- C++ compiler supporting C++11 or later
- Standard library (e.g., GNU libstdc++)

### Build Instructions
Use the makefile provided in order to build the project.
```sh
make
```
Run the project 
```sh
./build/server
```


### Code Structure
- TCPServer.h / TCPServer.cpp: Defines and implements the TCP server functionalities.
- ThreadPool.h / ThreadPool.cpp: Defines and implements the thread pool for concurrent task execution.
- HTTPServer.h / HTTPServer.cpp: Defines and implements the HTTP server that uses TCPServer and ThreadPool.
- main.cpp: Entry point of the application, initializes and starts the HTTP server.

### Impact on Network Understanding

This project provides a practical demonstration of several key networking and concurrency concepts:

- TCP Networking: Shows how to establish and manage TCP connections, which are fundamental for network communication.
- Concurrency: Utilizes a thread pool to manage multiple tasks concurrently, highlighting the importance of multithreading in network servers to handle multiple connections efficiently.
- HTTP Protocol: Implements a basic HTTP/1.1 server, offering insights into how web servers process and respond to HTTP requests.
