#ifndef MYSTL_VECTOR_HPP
#define MYSTL_VECTOR_HPP

#include "../core/memory.hpp"
#include "../core/iterator.hpp"
#include <cstddef>   
#include <utility>   
#include <stdexcept>

namespace mystl {

template <typename T, typename Allocator = mystl::allocator<T>>
class vector {
public:
    using value_type      = T;
    using allocator_type  = Allocator;
    using size_type       = std::size_t;
    using pointer         = T*;
    using reference       = T&;
    using const_reference = const T&;
    using iterator       = pointer;
    using const_iterator = const pointer;

private:
    allocator_type alloc_;          
    pointer start_;                 
    pointer finish_;                
    pointer end_of_storage_;        

public:
    vector() : start_(nullptr), finish_(nullptr), end_of_storage_(nullptr) {}

    ~vector() {
        clear();
        if (start_) {
            alloc_.deallocate(start_, capacity()); 
        }
    }

    iterator begin() { return start_; }
    iterator end()   { return finish_; }

    size_type size() const     { return static_cast<size_type>(finish_ - start_); }
    size_type capacity() const { return static_cast<size_type>(end_of_storage_ - start_); }
    bool empty() const         { return start_ == finish_; }

    void reserve(size_type new_cap) {
        if (new_cap > capacity()) {
            pointer new_start = alloc_.allocate(new_cap);
            pointer new_finish = new_start;

            for (pointer p = start_; p != finish_; ++p) {
                alloc_.construct(new_finish, std::move(*p));
                alloc_.destroy(p); 
                ++new_finish;
            }

            if (start_) {
                alloc_.deallocate(start_, capacity());
            }

            start_ = new_start;
            finish_ = new_finish;
            end_of_storage_ = start_ + new_cap;
        }
    }

    void push_back(const T& value) {
        if (finish_ == end_of_storage_) {
            size_type new_cap = (capacity() == 0) ? 1 : capacity() * 2;
            reserve(new_cap);
        }
        alloc_.construct(finish_, value);
        ++finish_;
    }

    void pop_back() {
        if (size() > 0) {
            --finish_;
            alloc_.destroy(finish_); 
        }
    }

    void clear() {
        while (finish_ != start_) {
            --finish_;
            alloc_.destroy(finish_);
        }
    }

    reference operator[](size_type pos) { 
        return start_[pos]; 
    }
    
    const_reference operator[](size_type pos) const { 
        return start_[pos]; 
    }
};

}

#endif