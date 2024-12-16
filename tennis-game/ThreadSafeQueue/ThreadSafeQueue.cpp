// ThreadSafeQueue.cpp
#include "ThreadSafeQueue.h"

template<typename T>
void ThreadSafeQueue<T>::enqueue(const T& value) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(value);
    }
    cond_.notify_one();
}

template<typename T>
bool ThreadSafeQueue<T>::dequeue(T& result) {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.empty() && !stopListening) {
        cond_.wait(lock);
    }
    if (queue_.empty()) {
        return false;
    }
    result = queue_.front();
    queue_.pop();
    return true;
}

template<typename T>
void ThreadSafeQueue<T>::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    while (!queue_.empty()) {
        queue_.pop();
    }
}
