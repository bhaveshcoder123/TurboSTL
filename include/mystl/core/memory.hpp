#ifndef MYSTL_MEMORY_HPP
#define MYSTL_MEMORY_HPP

#include <cstddef>  
#include <new>     
#include <utility> 

namespace mystl {

template <typename T>
class allocator {
public:
    using value_type      = T;
    using pointer         = T*;
    using const_pointer   = const T*;
    using reference       = T&;
    using const_reference = const T&;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

    template <typename U>
    struct rebind {
        using other = allocator<U>;
    };

    allocator() noexcept = default;
    ~allocator() = default;

    template <typename U>
    allocator(const allocator<U>&) noexcept {}

    pointer allocate(size_type n) {
        if (n == 0) {
            return nullptr;
        }
        if (n > static_cast<size_type>(-1) / sizeof(T)) {
            throw std::bad_alloc(); 
        }
        return static_cast<pointer>(::operator new(n * sizeof(T)));
    }

    void deallocate(pointer p, size_type n) noexcept {
        (void)n;
        ::operator delete(p);
    }

    template <typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        ::new(static_cast<void*>(p)) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U* p) {
        p->~U();
    }
};

template <typename T, typename U>
inline bool operator==(const allocator<T>&, const allocator<U>&) noexcept {
    return true;
}

template <typename T, typename U>
inline bool operator!=(const allocator<T>&, const allocator<U>&) noexcept {
    return false;
}

}

#endif