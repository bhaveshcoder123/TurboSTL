#include <iostream>
#include <cassert>
#include "../include/mystl/algorithms/algo.hpp"
#include "../include/mystl/containers/vector.hpp"

void test_basics() {
    int a = 10, b = 20;
    mystl::swap(a, b);
    assert(a == 20 && b == 10);
    assert(mystl::max(a, b) == 20);
    assert(mystl::min(a, b) == 10);
}

void test_elements() {
    mystl::vector<int> v;
    v.push_back(3);
    v.push_back(1);
    v.push_back(4);
    v.push_back(1);
    v.push_back(5);

    auto min_it = mystl::min_element(v.begin(), v.end());
    auto max_it = mystl::max_element(v.begin(), v.end());
    assert(*min_it == 1);
    assert(*max_it == 5);
}

void test_sort_and_bounds() {
    mystl::vector<int> v;
    v.push_back(5);
    v.push_back(2);
    v.push_back(9);
    v.push_back(1);
    v.push_back(5);
    v.push_back(6);

    mystl::sort(v.begin(), v.end());

    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 5);
    assert(v[3] == 5);
    assert(v[4] == 6);
    assert(v[5] == 9);

    auto lb = mystl::lower_bound(v.begin(), v.end(), 5);
    auto ub = mystl::upper_bound(v.begin(), v.end(), 5);
    
    assert(mystl::distance(v.begin(), lb) == 2);
    assert(mystl::distance(v.begin(), ub) == 4);
    
    auto it = v.begin();
    mystl::advance(it, 3);
    assert(*it == 5);
}

void test_permutations_and_reverse() {
    mystl::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    bool has_next = mystl::next_permutation(v.begin(), v.end());
    assert(has_next == true);
    assert(v[0] == 1);
    assert(v[1] == 3);
    assert(v[2] == 2);

    mystl::reverse(v.begin(), v.end());
    assert(v[0] == 2);
    assert(v[1] == 3);
    assert(v[2] == 1);
}

int main() {
    test_basics();
    test_elements();
    test_sort_and_bounds();
    test_permutations_and_reverse();
    std::cout << "[SUCCESS] All algorithms tested perfectly!\n";
    return 0;
}