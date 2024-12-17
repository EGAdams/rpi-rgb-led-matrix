// ThreadSafeQueue.h
#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;
    bool stopListening;

public:
    // Constructor
    ThreadSafeQueue() : stopListening( false ) {}

    // Destructor
    ~ThreadSafeQueue() { clear(); }

    // Disable copy and assignment
    ThreadSafeQueue( const ThreadSafeQueue& ) = delete;
    ThreadSafeQueue& operator=( const ThreadSafeQueue& ) = delete;

    // Methods
    void enqueue( const T& value );
    bool dequeue( T& result );
    void clear();
};
#endif
