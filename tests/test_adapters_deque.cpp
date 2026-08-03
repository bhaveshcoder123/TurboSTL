#include <iostream>
#include <cassert>
#include "../include/mystl/containers/stack.hpp"
#include "../include/mystl/containers/queue.hpp"
#include "../include/mystl/containers/deque.hpp"
#include "../include/mystl/containers/priority_queue.hpp"

void test_stack() {
    mystl::stack<int> s;
    s.push(10);
    s.push(20);
    s.push(30);
    
    assert(s.size() == 3);
    assert(s.top() == 30);
    s.pop();
    assert(s.top() == 20);
    
    std::cout << "[SUCCESS] Stack Test Passed!\n";
}

void test_queue() {
    mystl::queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    
    assert(q.size() == 3);
    assert(q.front() == 1);
    assert(q.back() == 3);
    
    q.pop();
    assert(q.front() == 2);
    
    std::cout << "[SUCCESS] Queue Test Passed!\n";
}

void test_deque() {
    mystl::deque<int> dq;
    
    dq.push_back(10);  
    dq.push_front(20); 
    dq.push_back(30);  
    dq.push_front(40);
    
    assert(dq.size() == 4);
    assert(dq.front() == 40);
    assert(dq.back() == 30);
    
    assert(dq[0] == 40);
    assert(dq[1] == 20);
    assert(dq[2] == 10);
    assert(dq[3] == 30);
    
    dq.pop_front(); 
    dq.pop_back(); 
    
    assert(dq.front() == 20);
    assert(dq.back() == 10);
    assert(dq.size() == 2);

    std::cout << "[SUCCESS] Deque (Circular Buffer) Test Passed!\n";
}

void test_priority_queue() {
    mystl::priority_queue<int> pq; 
    pq.push(15);
    pq.push(10);
    pq.push(30);
    pq.push(5);
    
    assert(pq.size() == 4);
    assert(pq.top() == 30);
    pq.pop();
    
    assert(pq.top() == 15);
    pq.pop();
    
    assert(pq.top() == 10);
    
    std::cout << "[SUCCESS] Priority Queue (Max-Heap) Test Passed!\n";
}

int main() {
    std::cout << "--- Running Adapters & Deque Tests ---\n";
    
    test_stack();
    test_queue();
    test_deque();
    test_priority_queue();
    
    std::cout << "--------------------------------------\n";
    std::cout << "All tests passed successfully!\n";
    
    return 0;
}