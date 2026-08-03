#ifndef MYSTL_SMART_PTR_HPP
#define MYSTL_SMART_PTR_HPP

#include <cstddef>
#include <utility>

namespace mystl {

template <typename T>
class unique_ptr {
private:
    T* ptr_;

public:
    explicit unique_ptr(T* p = nullptr) : ptr_(p) {}

    ~unique_ptr() { 
        delete ptr_; 
    }

    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

    unique_ptr(unique_ptr&& other) noexcept : ptr_(other.ptr_) { 
        other.ptr_ = nullptr; 
    }

    unique_ptr& operator=(unique_ptr&& other) noexcept {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    T* get() const { return ptr_; }

    T* release() {
        T* temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }

    void reset(T* p = nullptr) {
        delete ptr_;
        ptr_ = p;
    }
};

template <typename T>
class unique_ptr<T[]> {
private:
    T* ptr_;
public:
    explicit unique_ptr(T* p = nullptr) : ptr_(p) {}
    ~unique_ptr() { delete[] ptr_; } // Note the delete[]

    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;
    
    unique_ptr(unique_ptr&& other) noexcept : ptr_(other.ptr_) { other.ptr_ = nullptr; }
    unique_ptr& operator=(unique_ptr&& other) noexcept {
        if (this != &other) { delete[] ptr_; ptr_ = other.ptr_; other.ptr_ = nullptr; }
        return *this;
    }
    
    T& operator[](std::size_t i) const { return ptr_[i]; }
    T* get() const { return ptr_; }
    void reset(T* p = nullptr) { delete[] ptr_; ptr_ = p; }
};

template <typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
class shared_ptr {
private:
    T* ptr_;
    std::size_t* ref_count_;

    void cleanup() {
        if (ref_count_) {
            --(*ref_count_);
            if (*ref_count_ == 0) {
                delete ptr_;
                delete ref_count_;
            }
        }
    }

public:
    explicit shared_ptr(T* p = nullptr) : ptr_(p), ref_count_(p ? new std::size_t(1) : nullptr) {}

    ~shared_ptr() { 
        cleanup(); 
    }

    shared_ptr(const shared_ptr& other) : ptr_(other.ptr_), ref_count_(other.ref_count_) {
        if (ref_count_) {
            ++(*ref_count_);
        }
    }

    shared_ptr& operator=(const shared_ptr& other) {
        if (this != &other) {
            cleanup();
            ptr_ = other.ptr_;
            ref_count_ = other.ref_count_;
            if (ref_count_) {
                ++(*ref_count_);
            }
        }
        return *this;
    }

    shared_ptr(shared_ptr&& other) noexcept : ptr_(other.ptr_), ref_count_(other.ref_count_) {
        other.ptr_ = nullptr;
        other.ref_count_ = nullptr;
    }

    shared_ptr& operator=(shared_ptr&& other) noexcept {
        if (this != &other) {
            cleanup();
            ptr_ = other.ptr_;
            ref_count_ = other.ref_count_;
            other.ptr_ = nullptr;
            other.ref_count_ = nullptr;
        }
        return *this;
    }

    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    T* get() const { return ptr_; }
    
    std::size_t use_count() const { 
        return ref_count_ ? *ref_count_ : 0; 
    }

    void reset(T* p = nullptr) {
        cleanup();
        ptr_ = p;
        ref_count_ = p ? new std::size_t(1) : nullptr;
    }
};

template <typename T, typename... Args>
shared_ptr<T> make_shared(Args&&... args) {
    return shared_ptr<T>(new T(std::forward<Args>(args)...));
}

} 

#endif