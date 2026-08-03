#ifndef MYSTL_LIST_HPP
#define MYSTL_LIST_HPP

#include "../core/memory.hpp"
#include "../core/iterator.hpp"
#include <cstddef>
#include <utility>

namespace mystl {

template <typename T>
struct list_node {
    list_node* prev;
    list_node* next;
    T data;

    template <typename... Args>
    list_node(Args&&... args) : prev(nullptr), next(nullptr), data(std::forward<Args>(args)...) {}
};

template <typename T, typename Pointer, typename Reference>
struct list_iterator {

    using iterator_category = bidirectional_iterator_tag; 
    using value_type        = T;
    using difference_type   = std::ptrdiff_t;
    using pointer           = Pointer;
    using reference         = Reference;

    using node_ptr = list_node<T>*;
    using self     = list_iterator<T, Pointer, Reference>;

    node_ptr node;

    list_iterator() : node(nullptr) {}
    explicit list_iterator(node_ptr n) : node(n) {}

    reference operator*() const { return node->data; }
    pointer operator->() const  { return &(node->data); }

    self& operator++() {
        node = node->next;
        return *this;
    }
    
    self operator++(int) {
        self tmp = *this;
        node = node->next;
        return tmp;
    }

    self& operator--() {
        node = node->prev;
        return *this;
    }
    
    self operator--(int) {
        self tmp = *this;
        node = node->prev;
        return tmp;
    }

    bool operator==(const self& other) const { return node == other.node; }
    bool operator!=(const self& other) const { return node != other.node; }
};

template <typename T, typename Allocator = mystl::allocator<T>>
class list {
public:
    using value_type      = T;
    using allocator_type  = Allocator;
    using size_type       = std::size_t;
    using pointer         = T*;
    using reference       = T&;

    using const_pointer   = const T*;
    using const_reference = const T&;

    using node_type = list_node<T>;
    using node_ptr  = node_type*;

    using node_allocator = typename Allocator::template rebind<node_type>::other;

    using iterator       = list_iterator<T, T*, T&>;
    using const_iterator = list_iterator<T, const T*, const T&>;

private:
    node_allocator alloc_;
    node_ptr dummy_;
    size_type size_;

    node_ptr create_node(const T& value) {
        node_ptr n = alloc_.allocate(1);
        alloc_.construct(n, value);
        return n;
    }

    void destroy_node(node_ptr n) {
        alloc_.destroy(n);
        alloc_.deallocate(n, 1);
    }

public:
    list() : size_(0) {
        dummy_ = alloc_.allocate(1); 
        dummy_->next = dummy_;
        dummy_->prev = dummy_;
    }

    ~list() {
        clear();
        alloc_.deallocate(dummy_, 1); // Zameen wapas
    }

    iterator begin() { return iterator(dummy_->next); }
    iterator end()   { return iterator(dummy_); }

    const_iterator begin() const { return const_iterator(dummy_->next); }
    const_iterator end() const   { return const_iterator(dummy_); }
    const_iterator cbegin() const { return const_iterator(dummy_->next); }
    const_iterator cend() const   { return const_iterator(dummy_); }

    size_type size() const { return size_; }
    bool empty() const     { return size_ == 0; }

    iterator insert(iterator pos, const T& value) {
        node_ptr current = pos.node;
        node_ptr new_node = create_node(value);

        new_node->next = current;
        new_node->prev = current->prev;
        
        current->prev->next = new_node;
        current->prev = new_node;

        ++size_;
        return iterator(new_node);
    }

    iterator erase(iterator pos) {
        node_ptr current = pos.node;
        if (current == dummy_) return pos; 

        node_ptr next_node = current->next;
        node_ptr prev_node = current->prev;

        prev_node->next = next_node;
        next_node->prev = prev_node;

        destroy_node(current);
        --size_;

        return iterator(next_node);
    }

    void push_back(const T& value)  { insert(end(), value); }
    void push_front(const T& value) { insert(begin(), value); }
    
    void pop_back()  { erase(--end()); }
    void pop_front() { erase(begin()); }

    void clear() {
        while (!empty()) {
            pop_back();
        }
    }

    list(const list& other) : list() {
        for (auto& item : other) push_back(item);
    }
    list& operator=(const list& other) {
        if (this != &other) {
            clear();
            for (auto& item : other) push_back(item);
        }
        return *this;
    }
    list(list&& other) noexcept : alloc_(std::move(other.alloc_)), dummy_(other.dummy_), size_(other.size_) {
        other.dummy_ = alloc_.allocate(1); 
        other.dummy_->next = other.dummy_;
        other.dummy_->prev = other.dummy_;
        other.size_ = 0;
    }
    list& operator=(list&& other) noexcept {
        if (this != &other) {
            clear();
            alloc_.deallocate(dummy_, 1);
            alloc_ = std::move(other.alloc_);
            dummy_ = other.dummy_;
            size_ = other.size_;
            
            other.dummy_ = alloc_.allocate(1);
            other.dummy_->next = other.dummy_;
            other.dummy_->prev = other.dummy_;
            other.size_ = 0;
        }
        return *this;
    }
};

}

#endif