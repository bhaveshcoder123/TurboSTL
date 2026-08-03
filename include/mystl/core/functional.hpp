#ifndef MYSTL_FUNCTIONAL_HPP
#define MYSTL_FUNCTIONAL_HPP

#include <cstddef> 
#include <string> 

namespace mystl {

template <typename T>
struct less {
    bool operator()(const T& lhs, const T& rhs) const {
        return lhs < rhs;
    }
};

template <typename T>
struct greater {
    bool operator()(const T& lhs, const T& rhs) const {
        return lhs > rhs;
    }
};

template <typename T>
struct equal_to {
    bool operator()(const T& lhs, const T& rhs) const {
        return lhs == rhs;
    }
};

template <typename T>
struct hash; // Empty , requires the user to implement his own hash



template <typename T>
struct hash<T*> {
    std::size_t operator()(T* ptr) const {
        return reinterpret_cast<std::size_t>(ptr); 
    }
};

template <>
struct hash<int> {
    std::size_t operator()(int val) const {
        return static_cast<std::size_t>(val);
    }
};

//DJB2 Algorithm
template <>
struct hash<std::string> {
    std::size_t operator()(const std::string& str) const {
        std::size_t hash_value = 5381;
        for (char c : str) {
            hash_value = ((hash_value << 5) + hash_value) + c; 
        }
        return hash_value;
    }
};

}

#endif