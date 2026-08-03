#include <iostream>
#include <cassert>
#include <string>
#include "../include/mystl/containers/vector.hpp" 

struct Tracker {
    static int active_objects;
    
    Tracker() { active_objects++; }
    Tracker(const Tracker&) { active_objects++; }
    Tracker(Tracker&&) noexcept { active_objects++; }
    
    ~Tracker() { active_objects--; }
};

int Tracker::active_objects = 0;

void test_basic_integers() {
    mystl::vector<int> v;
    
    assert(v.size() == 0);
    assert(v.capacity() == 0);

    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    assert(v.size() == 3);
    assert(v.capacity() >= 3);
    
    assert(v[0] == 10);
    assert(v[1] == 20);
    assert(v[2] == 30);

    v.pop_back();
    assert(v.size() == 2);
    assert(v[1] == 20);

    v.clear();
    assert(v.size() == 0);
    assert(v.capacity() >= 2); 

    std::cout << "[SUCCESS] Basic Integer Test Passed!\n";
}

void test_object_lifetime() {
    {
        mystl::vector<Tracker> v;
        v.push_back(Tracker()); 
        v.push_back(Tracker()); 
        v.push_back(Tracker());
        v.pop_back(); 
    } 
    assert(Tracker::active_objects == 0);

    std::cout << "[SUCCESS] Object Lifetime & Memory Leak Test Passed!\n";
}

void test_strings() {
    mystl::vector<std::string> v;
    v.push_back("Hello");
    v.push_back("World");
    
    assert(v[0] == "Hello");
    assert(v[1] == "World");
    
    std::cout << "[SUCCESS] Complex Object (std::string) Test Passed!\n";
}

int main() {
    std::cout << "--- Running mystl::vector Tests ---\n";
    
    test_basic_integers();
    test_strings();
    test_object_lifetime();
    
    std::cout << "-----------------------------------\n";
    std::cout << "All tests passed! Vector is working perfectly.\n";
    
    return 0;
}