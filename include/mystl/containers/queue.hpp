#ifndef MYSTL_QUEUE_HPP
#define MYSTL_QUEUE_HPP

#include "list.hpp" 
#include <utility>

namespace mystl {

template <typename T, typename Container = mystl::list<T>>
class queue {
protected:
    Container c; 

public:
    using container_type  = Container;
    using value_type      = typename Container::value_type;
    using size_type       = typename Container::size_type;
    using reference       = typename Container::reference;
    using const_reference = typename Container::const_reference;

    queue() : c() {}
    ~queue() = default;

    bool empty() const { 
        return c.empty(); 
    }
    
    size_type size() const { 
        return c.size(); 
    }

    reference front() { 
        return *c.begin(); 
    }

    const_reference front() const { 
        return *c.begin(); 
    }

    reference back() { 
        auto it = c.end();
        --it;
        return *it;
    }

    const_reference back() const { 
        auto it = c.end();
        --it;
        return *it;
    }

    void push(const value_type& value) { 
        c.push_back(value); 
    }

    void pop() { 
        c.pop_front(); 
    }
};

} 

#endif