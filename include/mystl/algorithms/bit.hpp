#ifndef MYSTL_BIT_HPP
#define MYSTL_BIT_HPP

#include <type_traits>

namespace mystl {

template <typename T>
constexpr int popcount(T x) noexcept {
    static_assert(std::is_unsigned<T>::value, "mystl::popcount requires an unsigned integer type");
    
    int count = 0;
    while (x != 0) {
        x &= (x - 1);
        count++;
    }
    return count;
}

template <typename T>
constexpr bool has_single_bit(T x) noexcept {
    static_assert(std::is_unsigned<T>::value, "mystl::has_single_bit requires an unsigned integer type");
    
    return x != 0 && (x & (x - 1)) == 0;
}

} 

#endif