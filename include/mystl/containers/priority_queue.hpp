#ifndef MYSTL_PRIORITY_QUEUE_HPP
#define MYSTL_PRIORITY_QUEUE_HPP

#include "vector.hpp"
#include <utility>
#include "../core/functional.hpp"

namespace mystl {

template <typename T, typename Container = mystl::vector<T>, typename Compare = mystl::less<T>>
class priority_queue {
protected:
    Container c;  
    Compare comp;

    void sift_up(size_t idx) {
        while (idx > 0) {
            size_t parent = (idx - 1) / 2;
            if (comp(c[parent], c[idx])) {
                T temp = std::move(c[parent]);
                c[parent] = std::move(c[idx]);
                c[idx] = std::move(temp);
                idx = parent;
            } else {
                break;
            }
        }
    }

    void sift_down(size_t idx) {
        size_t size = c.size();
        while (true) {
            size_t largest = idx;
            size_t left = 2 * idx + 1;
            size_t right = 2 * idx + 2;

            if (left < size && comp(c[largest], c[left])) {
                largest = left;
            }
            if (right < size && comp(c[largest], c[right])) {
                largest = right;
            }

            if (largest != idx) {
                T temp = std::move(c[idx]);
                c[idx] = std::move(c[largest]);
                c[largest] = std::move(temp);
                idx = largest;
            } else {
                break;
            }
        }
    }

public:
    using container_type  = Container;
    using value_type      = typename Container::value_type;
    using size_type       = typename Container::size_type;
    using reference       = typename Container::reference;
    using const_reference = typename Container::const_reference;

    priority_queue() : c(), comp() {}

    bool empty() const { 
        return c.empty(); 
    }
    
    size_type size() const { 
        return c.size(); 
    }

    const_reference top() const {
        return c[0]; 
    }

    void push(const value_type& value) {
        c.push_back(value);
        sift_up(c.size() - 1); 
    }

    void pop() {
        if (c.empty()) return;
        
        c[0] = std::move(c[c.size() - 1]);
        c.pop_back();
        
        if (!c.empty()) {
            sift_down(0);
        }
    }
};

}

#endif 