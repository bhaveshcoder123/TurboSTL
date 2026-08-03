# TurboSTL: Standard Template Library Implementation

TurboSTL is a high-performance, from-scratch implementation of the C++ Standard Template Library (STL). It provides heavily optimized, memory-safe data structures and algorithms built entirely independent of `<vector>`, `<map>`, and other standard library container headers. 

This project was engineered to demonstrate a deep understanding of standard C++ semantics, manual memory management via custom allocators, template metaprogramming, and algorithmic complexity.

---

## Architecture & Engineering Principles

- **Memory Safety & Resource Acquisition Is Initialization (RAII):** Strict adherence to the Rule of Five. All resource-managing classes implement precise deep-copy semantics and move operations, ensuring zero memory leaks or double-free corruptions.
- **Custom Memory Allocation:** Containers bypass raw `new`/`delete` operators, utilizing a custom `mystl::allocator` to separate memory allocation from object initialization.
- **Move Semantics & Perfect Forwarding:** Leverages C++11/C++14/C++17 features (`std::move`, `std::forward`, `std::is_nothrow_move_constructible`) to minimize deep copies and guarantee exception safety during container reallocations.

---

## API Reference and Usage Guide

TurboSTL is designed as a drop-in replacement for `std::` namespaces. Include the respective headers from the `include/mystl/` directory to utilize the components.

### 1. Memory Management (Smart Pointers)
Header: `#include "core/smart_ptr.hpp"`

TurboSTL provides deterministic, RAII-compliant smart pointers for safe dynamic memory management.

#### `mystl::unique_ptr<T>`
Provides exclusive ownership of a dynamically allocated object. Supports scalar types and array types (`T[]`).

```cpp
// Scalar usage
mystl::unique_ptr<int> ptr = mystl::make_unique<int>(42);
std::cout << *ptr; // 42

// Transferring ownership
mystl::unique_ptr<int> ptr2 = std::move(ptr);
assert(ptr.get() == nullptr);

// Array usage
mystl::unique_ptr<int[]> arr(new int[100]);
arr[0] = 99;
```

#### `mystl::shared_ptr<T>`
Provides shared ownership of an object via a heap-allocated reference counter. The managed object is destroyed when the last `shared_ptr` relinquishes ownership.

```cpp
mystl::shared_ptr<std::string> sp1 = mystl::make_shared<std::string>("Data");
{
    mystl::shared_ptr<std::string> sp2 = sp1;
    assert(sp1.use_count() == 2);
} // sp2 goes out of scope
assert(sp1.use_count() == 1);
```

---

### 2. Sequence Containers

#### `mystl::vector<T>`
Header: `#include "containers/vector.hpp"`

A dynamically resizing array. Provides $O(1)$ amortized append operations and $O(1)$ random access. Implements exception-safe reallocation utilizing `std::is_nothrow_move_constructible`.

```cpp
mystl::vector<int> vec;
vec.reserve(10); // Pre-allocate memory to prevent costly reallocations

vec.push_back(1);
vec.push_back(2);

std::cout << vec[0]; // 1
vec.pop_back();      // Removes 2
```

#### `mystl::list<T>`
Header: `#include "containers/list.hpp"`

A circular, doubly-linked list utilizing a sentinel (dummy) node. Provides $O(1)$ insertions and deletions at any position, provided the iterator is known.

```cpp
mystl::list<int> lst;
lst.push_back(10);
lst.push_front(20);

// Iterating through the list
for (auto it = lst.begin(); it != lst.end(); ++it) {
    std::cout << *it << " "; // Outputs: 20 10
}
```

#### `mystl::deque<T>`
Header: `#include "containers/deque.hpp"`

A double-ended queue implemented as a dynamically resizing circular buffer. Provides $O(1)$ insertions and deletions at both the front and the back.

```cpp
mystl::deque<int> dq;
dq.push_back(10);
dq.push_front(5);
std::cout << dq.front(); // 5
std::cout << dq.back();  // 10
```

---

### 3. Associative Containers (Red-Black Tree)
Headers: `#include "containers/map.hpp"`, `#include "containers/set.hpp"`

TurboSTL associative containers are backed by a rigorously balanced **Red-Black Tree**. They guarantee $O(\log N)$ time complexity for search, insertion, and deletion operations.

#### `mystl::map<Key, Value>`
Stores key-value pairs sorted by unique keys.

```cpp
mystl::map<std::string, int> user_ages;

// Insertion via operator[]
user_ages["Alice"] = 28;
user_ages["Bob"] = 34;

// O(log N) Lookup
auto it = user_ages.find("Alice");
if (it != user_ages.end()) {
    std::cout << "Alice is " << it->second << " years old.";
}

// Ordered Traversal
for (const auto& pair : user_ages) {
    std::cout << pair.first << ": " << pair.second << "\n";
}
```

#### `mystl::set<Key>`
Stores unique keys in sorted order.

```cpp
mystl::set<int> unique_numbers;
unique_numbers.insert(10);
unique_numbers.insert(20);
unique_numbers.insert(10); // Ignored, duplicate key

assert(unique_numbers.size() == 2);
```

---

### 4. Unordered Containers (Hash Tables)
Headers: `#include "containers/unordered_map.hpp"`, `#include "containers/unordered_set.hpp"`

Unordered containers are backed by a dynamic Hash Table using **Separate Chaining** (linked lists) for collision resolution. They provide amortized $O(1)$ time complexity for insertions and lookups.

#### `mystl::unordered_map<Key, Value>`
```cpp
mystl::unordered_map<std::string, double> product_prices;

product_prices.insert({"Laptop", 1299.99});
product_prices["Mouse"] = 49.99;

// Triggers automatic rehashing when load factor exceeds threshold
for(int i = 0; i < 1000; i++) {
    product_prices[std::to_string(i)] = i * 1.5;
}
```

---

### 5. Container Adapters
Headers: `#include "containers/stack.hpp"`, `#include "containers/queue.hpp"`, `#include "containers/priority_queue.hpp"`

Adapters restrict the interface of underlying sequence containers to provide specific data access models.

*   **`mystl::stack<T>`**: LIFO data structure. Backed by `mystl::vector` by default.
*   **`mystl::queue<T>`**: FIFO data structure. Backed by `mystl::list` by default.
*   **`mystl::priority_queue<T>`**: Max-Heap data structure. Guarantees $O(\log N)$ insertions and $O(1)$ access to the maximum element. Backed by `mystl::vector`.

```cpp
mystl::priority_queue<int> pq;
pq.push(10);
pq.push(50);
pq.push(20);

std::cout << pq.top(); // 50
pq.pop();
std::cout << pq.top(); // 20
```

---

## Testing Methodology

This library is rigorously tested to ensure algorithmic correctness and absolute memory safety. The `tests/` directory contains individual, targeted test files for each container and algorithm.

All tests must be compiled with the **Address Sanitizer (ASAN)** to detect heap corruption, buffer overflows, and memory leaks.

### Building and Running Tests

Requires CMake (3.10+) and a C++17 compatible compiler.

```bash
# Generate build files
cmake -B build -S .

# Build all test executables
cmake --build build

# Execute the test suite via CTest
cd build
ctest --output-on-failure
```

Alternatively, you can compile and run specific tests manually:
```bash
g++ -std=c++17 -Wall -Wextra -fsanitize=address tests/test_vector.cpp -o test_vector
./test_vector
```

---

## Known Divergences from `std::`

To optimize for implementation clarity and educational focus, this project makes specific architectural trade-offs compared to production implementations (e.g., GCC's `libstdc++` or LLVM's `libc++`):

1. **`deque` Memory Layout:** The standard mandates that `std::deque` insertions do not invalidate references to existing elements. To achieve this, production implementations use a map of chunked memory arrays. `mystl::deque` is implemented as a dynamically resizing circular buffer, which is highly performant but invalidates pointers upon reallocation.
2. **`operator[]` Optimization:** In `mystl::map` and `mystl::unordered_map`, the bracket operator currently performs a dual traversal (lookup, followed by insertion if not found). Future iterations will refactor internal `insert` methods to return iterators, reducing this to a single $O(\log N)$ or $O(1)$ traversal.
3. **Tree Destruction:** `mystl::rb_tree` relies on a recursive post-order traversal for clearance. While mathematically correct, extremely deep trees could theoretically trigger stack overflows in strictly constrained environments.

---

## License

This software is released under the MIT License.