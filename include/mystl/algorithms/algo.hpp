#ifndef MYSTL_ALGO_HPP
#define MYSTL_ALGO_HPP

#include <utility>
#include "../core/iterator.hpp"

namespace mystl {

template <typename T>
void swap(T& a, T& b) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

template <typename T>
const T& max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

template <typename T>
const T& min(const T& a, const T& b) {
    return (b < a) ? b : a;
}

template <typename ForwardIt>
ForwardIt min_element(ForwardIt first, ForwardIt last) {
    if (first == last) return last;
    ForwardIt smallest = first;
    ++first;
    for (; first != last; ++first) {
        if (*first < *smallest) {
            smallest = first;
        }
    }
    return smallest;
}

template <typename ForwardIt>
ForwardIt max_element(ForwardIt first, ForwardIt last) {
    if (first == last) return last;
    ForwardIt largest = first;
    ++first;
    for (; first != last; ++first) {
        if (*largest < *first) {
            largest = first;
        }
    }
    return largest;
}

template <typename InputIt>
typename mystl::iterator_traits<InputIt>::difference_type distance(InputIt first, InputIt last) {
    typename mystl::iterator_traits<InputIt>::difference_type result = 0;
    while (first != last) {
        ++first;
        ++result;
    }
    return result;
}

template <typename InputIt, typename Distance>
void advance(InputIt& it, Distance n) {
    while (n > 0) {
        ++it;
        --n;
    }
    while (n < 0) {
        --it;
        ++n;
    }
}

template <typename ForwardIt, typename T>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value) {
    auto count = mystl::distance(first, last);
    while (count > 0) {
        auto step = count / 2;
        auto it = first;
        mystl::advance(it, step);
        if (*it < value) {
            first = ++it;
            count -= step + 1;
        } else {
            count = step;
        }
    }
    return first;
}

template <typename ForwardIt, typename T>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value) {
    auto count = mystl::distance(first, last);
    while (count > 0) {
        auto step = count / 2;
        auto it = first;
        mystl::advance(it, step);
        if (!(value < *it)) {
            first = ++it;
            count -= step + 1;
        } else {
            count = step;
        }
    }
    return first;
}

template <typename BidirectionalIt>
void reverse(BidirectionalIt first, BidirectionalIt last) {
    while ((first != last) && (first != --last)) {
        mystl::swap(*first, *last);
        ++first;
    }
}

template <typename BidirectionalIt>
bool next_permutation(BidirectionalIt first, BidirectionalIt last) {
    if (first == last) return false;
    BidirectionalIt i = last;
    if (first == --i) return false;

    while (true) {
        BidirectionalIt i1, i2;
        i1 = i;
        if (*--i < *i1) {
            i2 = last;
            while (!(*i < *--i2));
            mystl::swap(*i, *i2);
            mystl::reverse(i1, last); 
            return true;
        }
        if (i == first) {
            mystl::reverse(first, last);
            return false;
        }
    }
}

template <typename RandomIt>
void sort(RandomIt first, RandomIt last) {
    if (first == last) return;

    auto dist = last - first; 
    if (dist <= 1) return;
    RandomIt pivot_it = first + dist / 2;
    auto pivot = *pivot_it; 
    
    RandomIt left = first;
    RandomIt right = last - 1;
    
    while (left <= right) {
        while (*left < pivot) ++left;
        while (pivot < *right) --right;
        if (left <= right) {
            mystl::swap(*left, *right);
            ++left;
            --right;
        }
    }

    if (first < right + 1) mystl::sort(first, right + 1);
    if (left < last) mystl::sort(left, last);
}

} 

#endif