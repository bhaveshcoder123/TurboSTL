#ifndef MYSTL_DEQUE_HPP
#define MYSTL_DEQUE_HPP

#include "../core/memory.hpp"
#include "../core/iterator.hpp"
#include <cstddef>
#include <utility>

namespace mystl {

template <typename T, typename Allocator = mystl::allocator<T>>
class deque {
public:
    using value_type      = T;
    using allocator_type  = Allocator;
    using size_type       = std::size_t;
    using pointer         = T*;
    using reference       = T&;
    using const_reference = const T&;

private:
    allocator_type alloc_;
    pointer data_;
    size_type capacity_;
    size_type front_idx_;
    size_type back_idx_;
    size_type size_;

    size_type increment(size_type idx) const { return (idx + 1) % capacity_; }
    size_type decrement(size_type idx) const { return (idx == 0) ? capacity_ - 1 : idx - 1; }

    void reallocate(size_type new_cap) {
        pointer new_data = alloc_.allocate(new_cap);
        
        size_type curr = front_idx_;
        for (size_type i = 0; i < size_; ++i) {
            alloc_.construct(new_data + i, std::move(data_[curr]));
            alloc_.destroy(data_ + curr);
            curr = increment(curr);
        }

        if (data_) {
            alloc_.deallocate(data_, capacity_);
        }

        data_ = new_data;
        capacity_ = new_cap;
        front_idx_ = 0;
        back_idx_ = (size_ == 0) ? 0 : size_ - 1;
    }

public:
    deque() : data_(nullptr), capacity_(0), front_idx_(0), back_idx_(0), size_(0) {}

    ~deque() {
        clear();
        if (data_) {
            alloc_.deallocate(data_, capacity_);
        }
    }

    size_type size() const { return size_; }
    bool empty() const { return size_ == 0; }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            reallocate(capacity_ == 0 ? 2 : capacity_ * 2);
        }
        
        if (size_ == 0) {
            front_idx_ = 0;
            back_idx_ = 0;
        } else {
            back_idx_ = increment(back_idx_);
        }
        
        alloc_.construct(data_ + back_idx_, value);
        ++size_;
    }

    void push_front(const T& value) {
        if (size_ == capacity_) {
            reallocate(capacity_ == 0 ? 2 : capacity_ * 2);
        }

        if (size_ == 0) {
            front_idx_ = 0;
            back_idx_ = 0;
        } else {
            front_idx_ = decrement(front_idx_);
        }

        alloc_.construct(data_ + front_idx_, value);
        ++size_;
    }

    void pop_back() {
        if (size_ > 0) {
            alloc_.destroy(data_ + back_idx_);
            if (size_ > 1) {
                back_idx_ = decrement(back_idx_);
            }
            --size_;
        }
    }

    void pop_front() {
        if (size_ > 0) {
            alloc_.destroy(data_ + front_idx_);
            if (size_ > 1) {
                front_idx_ = increment(front_idx_);
            }
            --size_;
        }
    }

    reference front() { return data_[front_idx_]; }
    reference back()  { return data_[back_idx_]; }

    reference operator[](size_type pos) {
        return data_[(front_idx_ + pos) % capacity_];
    }

    void clear() {
        while (!empty()) {
            pop_front();
        }
    }
};

}

#endif