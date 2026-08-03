#include <iostream>
#include <cassert>
#include "../include/mystl/core/smart_ptr.hpp"

struct TestObj {
    int val;
    TestObj(int v) : val(v) {}
};

void test_unique_ptr() {
    mystl::unique_ptr<TestObj> p1(new TestObj(10));
    assert(p1->val == 10);

    mystl::unique_ptr<TestObj> p2 = mystl::make_unique<TestObj>(20);
    assert(p2->val == 20);

    mystl::unique_ptr<TestObj> p3 = std::move(p1);
    assert(p1.get() == nullptr);
    assert(p3->val == 10);

    p3.reset(new TestObj(30));
    assert(p3->val == 30);
    
    TestObj* raw = p3.release();
    assert(p3.get() == nullptr);
    assert(raw->val == 30);
    delete raw; 
}

void test_shared_ptr() {
    mystl::shared_ptr<TestObj> sp1 = mystl::make_shared<TestObj>(100);
    assert(sp1.use_count() == 1);
    assert(sp1->val == 100);

    {
        mystl::shared_ptr<TestObj> sp2 = sp1;
        assert(sp1.use_count() == 2);
        assert(sp2.use_count() == 2);
        assert(sp2->val == 100);

        mystl::shared_ptr<TestObj> sp3;
        sp3 = sp2;
        assert(sp1.use_count() == 3);
    } 

    assert(sp1.use_count() == 1);

    mystl::shared_ptr<TestObj> sp4 = std::move(sp1);
    assert(sp1.use_count() == 0);
    assert(sp1.get() == nullptr);
    assert(sp4.use_count() == 1);
    assert(sp4->val == 100);
}

int main() {
    test_unique_ptr();
    test_shared_ptr();
    std::cout << "[SUCCESS] Smart Pointers tested perfectly!\n";
    return 0;
}