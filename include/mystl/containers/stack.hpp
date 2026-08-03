#ifndef MYSTL_STACK_HPP
#define MYSTL_STACK_HPP

#include "vector.hpp" 
#include <utility>

namespace mystl {

template <typename T, typename Container = mystl::vector<T>>
class stack {
protected:
    Container c; 

public:
    using container_type  = Container;
    using value_type      = typename Container::value_type;
    using size_type       = typename Container::size_type;
    using reference       = typename Container::reference;
    using const_reference = typename Container::const_reference;

    stack() : c() {}
    ~stack() = default;

    bool empty() const { 
        return c.empty(); 
    }
    
    size_type size() const { 
        return c.size(); 
    }

    reference top() { 
        auto it = c.end();
        --it;
        return *it;
    }

    const_reference top() const { 
        auto it = c.end();
        --it;
        return *it;
    }

    void push(const value_type& value) { 
        c.push_back(value); 
    }

    void pop() { 
        c.pop_back(); 
    }
};

} 

#endif 