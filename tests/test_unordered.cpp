#include <iostream>
#include <cassert>
#include <string>
#include "../include/mystl/containers/unordered_set.hpp"
#include "../include/mystl/containers/unordered_map.hpp"

void test_unordered_set() {
    mystl::unordered_set<int> uset;
    
    assert(uset.empty());
    
    uset.insert(10);
    uset.insert(20);
    uset.insert(30);
    uset.insert(10); 
    
    assert(uset.size() == 3);
    
    assert(uset.find(20) != uset.end());
    assert(uset.find(99) == uset.end());
    
    uset.erase(20);
    assert(uset.size() == 2);
    assert(uset.find(20) == uset.end());

    mystl::unordered_set<std::string> string_set;
    string_set.insert("Hello");
    string_set.insert("World");
    assert(string_set.find("Hello") != string_set.end());
    assert(string_set.size() == 2);

    std::cout << "[SUCCESS] Unordered Set Tests Passed!\n";
}

void test_unordered_map() {
    mystl::unordered_map<std::string, int> umap;
    
    assert(umap.empty());
    
    umap["Codeforces"] = 3500;
    umap["LeetCode"] = 2800;
    umap["AtCoder"] = 3200;
    
    assert(umap.size() == 3);
    assert(umap["Codeforces"] == 3500);
    
    umap["LeetCode"] = 2900;
    assert(umap["LeetCode"] == 2900);
    assert(umap.size() == 3); 
    
    auto it = umap.find("AtCoder");
    assert(it != umap.end());
    assert(it->second == 3200);
    
    umap.erase("Codeforces");
    assert(umap.size() == 2);
    assert(umap.find("Codeforces") == umap.end());

    std::cout << "[SUCCESS] Unordered Map (operator[] & Lookup) Tests Passed!\n";
}

void test_rehashing_stress_test() {
    mystl::unordered_map<int, int> stress_map;
    
    for (int i = 0; i < 1000; ++i) {
        stress_map[i] = i * 10;
    }
    
    assert(stress_map.size() == 1000);
    
    for (int i = 0; i < 1000; ++i) {
        assert(stress_map[i] == i * 10);
    }
    
    assert(stress_map.load_factor() < 1.0f); 

    std::cout << "[SUCCESS] Rehashing Stress Test Passed!.\n";
}

int main() {
    std::cout << "--- Running mystl::unordered containers (Hashtable) Tests ---\n";
    
    test_unordered_set();
    test_unordered_map();
    test_rehashing_stress_test();
    
    std::cout << "-------------------------------------------------------------\n";
    std::cout << "All Unordered Data Structure tests passed successfully!\n";
    
    return 0;
}