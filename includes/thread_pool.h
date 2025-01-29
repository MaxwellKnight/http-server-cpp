#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

class ThreadPool {
public:
    // Constructor with number of threads to create
    explicit ThreadPool(size_t threads);
    
    // Destructor - ensures all threads are properly joined
    ~ThreadPool();

    // Delete copy constructor and assignment operator
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    // Enqueues a task to be executed by the thread pool
    void enqueue(std::function<void()> task);

    // Gets the number of pending tasks in the queue
    size_t get_pending_tasks() const {
        std::unique_lock<std::mutex> lock(queue_mutex);
        return tasks.size();
    }

private:
    // Worker threads
    std::vector<std::thread> workers;
    
    // Task queue
    std::queue<std::function<void()>> tasks;
    
    // Synchronization
    mutable std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;

    // Worker thread function
    void work();
};

#endif // THREAD_POOL_H
