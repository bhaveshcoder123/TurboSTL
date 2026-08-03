#include <iostream>
#include <cassert>
#include "../include/mystl/containers/set.hpp"

void test_basic_set() {
    mystl::set<int> s;

    assert(s.empty());
    
    s.insert(50);
    s.insert(30);
    s.insert(70);
    s.insert(20);
    s.insert(40);
    
    s.insert(50);
    
    assert(s.size() == 5);

    auto it = s.begin();
    assert(*it == 20); ++it;
    assert(*it == 30); ++it;
    assert(*it == 40); ++it;
    assert(*it == 50); ++it;
    assert(*it == 70);
    
    std::cout << "[SUCCESS] Basic Insertion and In-Order Traversal Passed!\n";
}

void test_lookup_and_erase() {
    mystl::set<int> s;
    s.insert(10);
    s.insert(20);
    s.insert(30);

    auto it = s.find(20);
    assert(it != s.end());
    assert(*it == 20);

    auto missing_it = s.find(99);
    assert(missing_it == s.end());

    s.erase(20);
    assert(s.size() == 2);
    
    assert(s.find(20) == s.end());
    
    auto order_it = s.begin();
    assert(*order_it == 10); ++order_it;
    assert(*order_it == 30);

    std::cout << "[SUCCESS] O(log N) Find and Erase Tests Passed!\n";
}

int main() {
    std::cout << "--- Running mystl::set (Red-Black Tree) Tests ---\n";
    
    test_basic_set();
    test_lookup_and_erase();
    
    std::cout << "--------------------------------------------------\n";
    std::cout << "All Set tests passed successfully! RBT Engine is solid.\n";
    
    return 0;
}