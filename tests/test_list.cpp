#include <iostream>
#include <cassert>
#include <string>
#include "../include/mystl/containers/list.hpp"

struct Tracker {
    static int active_objects;
    
    Tracker() { active_objects++; }
    Tracker(const Tracker&) { active_objects++; }
    Tracker(Tracker&&) noexcept { active_objects++; }
    
    ~Tracker() { active_objects--; }
};

int Tracker::active_objects = 0;

void test_basic_list() {
    mystl::list<int> l;
    
    assert(l.empty());
    assert(l.size() == 0);

    l.push_back(10);
    l.push_front(20);
    l.push_back(30);

    assert(l.size() == 3);

    auto it = l.begin();
    assert(*it == 20); ++it;
    assert(*it == 10); ++it;
    assert(*it == 30); ++it;
    assert(it == l.end());

    auto bit = l.end();
    --bit; assert(*bit == 30);
    --bit; assert(*bit == 10);
    --bit; assert(*bit == 20);
    assert(bit == l.begin());

    l.pop_front(); 
    assert(l.size() == 2);
    assert(*l.begin() == 10);

    l.pop_back();
    assert(l.size() == 1);
    
    l.clear();
    assert(l.empty());
    assert(l.size() == 0);

    std::cout << "[SUCCESS] Basic List & Iterator Test Passed!\n";
}

void test_strings_list() {
    mystl::list<std::string> l;
    l.push_back("Data");
    l.push_front("Structures");
    l.insert(++l.begin(), "and"); 
    
    auto it = l.begin();
    assert(*it++ == "Structures");
    assert(*it++ == "and");
    assert(*it == "Data");

    std::cout << "[SUCCESS] Complex Object (std::string) Test Passed!\n";
}

void test_object_lifetime() {
    {
        mystl::list<Tracker> l;
        l.push_back(Tracker()); 
        l.push_front(Tracker()); 
        l.push_back(Tracker()); 
        
        l.pop_back();
    } 

    assert(Tracker::active_objects == 0);

    std::cout << "[SUCCESS] Object Lifetime & Node Memory Leak Test Passed!\n";
}

int main() {
    std::cout << "--- Running mystl::list Tests ---\n";
    
    test_basic_list();
    test_strings_list();
    test_object_lifetime();
    
    std::cout << "--------------------------------\n";
    std::cout << "All tests passed!.\n";
    
    return 0;
}