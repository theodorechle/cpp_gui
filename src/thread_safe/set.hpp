#ifndef SET_HPP
#define SET_HPP

#include <functional>
#include <mutex>
#include <set>

namespace threadSafe {
    template <typename T>
    class Set {
        std::set<T> *_set;
        std::mutex _mutex;

    public:
        Set() : _set{new std::set<T>()} {}

        size_t size() {
            std::lock_guard<std::mutex> lock(_mutex);
            return _set->size();
        }

        void insert(T value) {
            std::lock_guard<std::mutex> lock(_mutex);
            _set->insert(value);
        }

        std::set<T> *readAndClear() {
            std::lock_guard<std::mutex> lock(_mutex);
            std::set<T> *value = _set;
            _set = new std::set<T>();
            return value;
        }

        void clear() {
            std::lock_guard<std::mutex> lock(_mutex);
            _set->clear();
        }

        bool empty() {
            std::lock_guard<std::mutex> lock(_mutex);
            return _set->empty();
        }

        void deleteIf(std::function<bool(T const &)> condition) {
            std::lock_guard<std::mutex> lock(_mutex);
            for (typename std::set<T>::const_iterator it = _set->begin(); it != _set->end();) {
                if (condition(*it)) it = _set->erase(it);
                else it++;
            }
        }
    };
} // namespace threadSafe
#endif // SET_HPP
