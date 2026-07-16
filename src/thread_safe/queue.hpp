#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <mutex>
#include <queue>

namespace threadSafe {
    template <typename T>
    class Queue {
        std::queue<T> _queue;
        std::mutex _mutex;

    public:
        size_t size() {
            std::lock_guard<std::mutex> lock(_mutex);
            return _queue.size();
        }

        void push(T value) {
            std::lock_guard<std::mutex> lock(_mutex);
            _queue.push(value);
        }

        bool tryPop(T *value) {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_queue.empty()) return false;
            (*value) = _queue.front();
            _queue.pop();
            return true;
        }
    };
} // namespace threadSafe

#endif // QUEUE_HPP
