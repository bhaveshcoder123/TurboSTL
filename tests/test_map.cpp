#include <iostream>
#include <cassert>
#include <string>
#include "../include/mystl/containers/map.hpp"

void test_basic_map_and_iterators() {
    mystl::map<int, std::string> m;
    
    assert(m.empty());
    
    m.insert(std::make_pair(3, "Three"));
    m.insert(std::make_pair(1, "One"));
    m.insert(std::make_pair(2, "Two"));
    
    assert(m.size() == 3);

    auto it = m.begin();
    assert(it->first == 1 && it->second == "One"); ++it;
    assert(it->first == 2 && it->second == "Two"); ++it;
    assert(it->first == 3 && it->second == "Three"); ++it;
    assert(it == m.end());

    it = m.find(2);
    it->second = "Two_Modified"; 
    assert(m.find(2)->second == "Two_Modified");
    
    std::cout << "[SUCCESS] Basic Map Insertion & Iterator Safety Passed!\n";
}

void test_map_operator_brackets() {
    mystl::map<std::string, int> m;
    
    assert(m.size() == 0);
    int& val = m["Score"]; 
    assert(m.size() == 1);
    assert(val == 0);
    assert(m["Score"] == 0);
    
    m["Score"] = 99;
    assert(m["Score"] == 99);
    assert(m.size() == 1); 

    m["Health"] = 100;
    m["Mana"] = 50;
    assert(m.size() == 3);
    
    std::cout << "[SUCCESS] Map operator[] (Auto-insertion & Update) Passed!\n";
}

void test_erase_and_clear() {
    mystl::map<int, double> m;
    m[10] = 1.1;
    m[20] = 2.2;
    m[30] = 3.3;

    assert(m.size() == 3);

    m.erase(20);
    assert(m.size() == 2);
    
    assert(m.find(20) == m.end());
    
    assert(m.find(10) != m.end());
    assert(m.find(30) != m.end());

    m.clear();
    assert(m.empty());
    assert(m.size() == 0);

    std::cout << "[SUCCESS] Map Erase & Clear Passed!\n";
}

int main() {
    std::cout << "--- Running mystl::map (Red-Black Tree) Tests ---\n";
    
    test_basic_map_and_iterators();
    test_map_operator_brackets();
    test_erase_and_clear();
    
    std::cout << "-------------------------------------------------\n";
    std::cout << "All Map tests passed! Red-Black Tree is highly stable.\n";
    
    return 0;
}