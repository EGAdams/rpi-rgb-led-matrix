#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <mutex>

template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T> queue_;
    std::mutex mutex_;

public:
    void enqueue(const T& item) {
        std::lock_guard<std::mutex> lock(this->mutex_);
        this->queue_.push(item);
    }

    bool dequeue(T& item) {
        std::lock_guard<std::mutex> lock(this->mutex_);
        if (this->queue_.empty()) {
            return false;
        }
        item = this->queue_.front();
        this->queue_.pop();
        return true;
    }

    void clear() {
        std::lock_guard<std::mutex> lock(this->mutex_);
        std::queue<T> empty;
        std::swap(this->queue_, empty);
    }
};

#endif // THREAD_SAFE_QUEUE_H
