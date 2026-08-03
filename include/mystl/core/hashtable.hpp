#ifndef MYSTL_HASHTABLE_HPP
#define MYSTL_HASHTABLE_HPP

#include "memory.hpp"
#include "functional.hpp"
#include "iterator.hpp"
#include <cstddef>
#include <utility>

namespace mystl {

template <typename T>
struct hash_node {
    T data;
    hash_node* next;

    template <typename... Args>
    hash_node(Args&&... args) : data(std::forward<Args>(args)...), next(nullptr) {}
};

template <typename T, typename Hash = mystl::hash<T>, typename Equal = mystl::equal_to<T>, typename Allocator = mystl::allocator<T>>
class hashtable {
public:
    using value_type      = T;
    using size_type       = std::size_t;
    using hasher          = Hash;
    using key_equal       = Equal;
    using allocator_type  = Allocator;
    
    using node_type       = hash_node<T>;
    using node_ptr        = node_type*;
    using node_allocator  = typename Allocator::template rebind<node_type>::other;
    using ptr_allocator   = typename Allocator::template rebind<node_ptr>::other;

private:
    node_ptr* buckets_;       
    size_type bucket_count_; 
    size_type size_;         
    float max_load_factor_; 
    
    hasher hash_;
    key_equal equals_;
    node_allocator node_alloc_;
    ptr_allocator ptr_alloc_;

    size_type bucket_index(const T& val, size_type b_count) const {
        return hash_(val) % b_count;
    }

    node_ptr create_node(const T& val) {
        node_ptr n = node_alloc_.allocate(1);
        node_alloc_.construct(n, val);
        return n;
    }

    void destroy_node(node_ptr n) {
        node_alloc_.destroy(n);
        node_alloc_.deallocate(n, 1);
    }

    node_ptr* allocate_buckets(size_type count) {
        node_ptr* b = ptr_alloc_.allocate(count);
        for (size_type i = 0; i < count; ++i) {
            b[i] = nullptr;
        }
        return b;
    }

    void deallocate_buckets(node_ptr* b, size_type count) {
        if (b) {
            ptr_alloc_.deallocate(b, count);
        }
    }

public:
    template <typename U_Ptr, typename U_Ref>
    struct hash_iterator {
        using iterator_category = forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = U_Ptr;
        using reference         = U_Ref;

        node_ptr node;
        const hashtable* ht; 
        size_type bucket_idx;

        hash_iterator() : node(nullptr), ht(nullptr), bucket_idx(0) {}
        hash_iterator(node_ptr n, const hashtable* h, size_type idx) : node(n), ht(h), bucket_idx(idx) {}

        template <typename O_Ptr, typename O_Ref>
        hash_iterator(const hash_iterator<O_Ptr, O_Ref>& other) 
            : node(other.node), ht(other.ht), bucket_idx(other.bucket_idx) {}

        reference operator*() const { return node->data; }
        pointer operator->() const { return &(node->data); }

        hash_iterator& operator++() {
            if (node->next != nullptr) {
                node = node->next; 
            } else {
                ++bucket_idx;
                while (bucket_idx < ht->bucket_count_ && ht->buckets_[bucket_idx] == nullptr) {
                    ++bucket_idx;
                }
                if (bucket_idx < ht->bucket_count_) {
                    node = ht->buckets_[bucket_idx];
                } else {
                    node = nullptr;
                }
            }
            return *this;
        }

        bool operator==(const hash_iterator& other) const { return node == other.node; }
        bool operator!=(const hash_iterator& other) const { return node != other.node; }
    };

    using iterator       = hash_iterator<T*, T&>;
    using const_iterator = hash_iterator<const T*, const T&>;

    hashtable(size_type initial_bucket_count = 8) 
        : bucket_count_(initial_bucket_count), size_(0), max_load_factor_(1.0f) {
        buckets_ = allocate_buckets(bucket_count_);
    }

    ~hashtable() {
        clear();
        deallocate_buckets(buckets_, bucket_count_);
    }

    size_type size() const { return size_; }
    bool empty() const { return size_ == 0; }
    
    float load_factor() const { 
        return static_cast<float>(size_) / static_cast<float>(bucket_count_); 
    }

    hashtable(const hashtable& other) 
        : bucket_count_(other.bucket_count_), size_(0), max_load_factor_(other.max_load_factor_), 
          hash_(other.hash_), equals_(other.equals_), node_alloc_(other.node_alloc_), ptr_alloc_(other.ptr_alloc_) {
        buckets_ = allocate_buckets(bucket_count_);
        for (size_type i = 0; i < other.bucket_count_; ++i) {
            node_ptr curr = other.buckets_[i];
            while (curr != nullptr) {
                insert(curr->data);
                curr = curr->next;
            }
        }
    }

    hashtable& operator=(const hashtable& other) {
        if (this != &other) {
            clear();
            deallocate_buckets(buckets_, bucket_count_);
            bucket_count_ = other.bucket_count_;
            max_load_factor_ = other.max_load_factor_;
            hash_ = other.hash_;
            equals_ = other.equals_;
            buckets_ = allocate_buckets(bucket_count_);
            for (size_type i = 0; i < other.bucket_count_; ++i) {
                node_ptr curr = other.buckets_[i];
                while (curr != nullptr) {
                    insert(curr->data);
                    curr = curr->next;
                }
            }
        }
        return *this;
    }

    hashtable(hashtable&& other) noexcept 
        : buckets_(other.buckets_), bucket_count_(other.bucket_count_), size_(other.size_), 
          max_load_factor_(other.max_load_factor_), hash_(std::move(other.hash_)), equals_(std::move(other.equals_)) {
        other.buckets_ = nullptr;
        other.bucket_count_ = 0;
        other.size_ = 0;
    }

    hashtable& operator=(hashtable&& other) noexcept {
        if (this != &other) {
            clear();
            deallocate_buckets(buckets_, bucket_count_);
            buckets_ = other.buckets_;
            bucket_count_ = other.bucket_count_;
            size_ = other.size_;
            max_load_factor_ = other.max_load_factor_;
            hash_ = std::move(other.hash_);
            equals_ = std::move(other.equals_);
            other.buckets_ = nullptr;
            other.bucket_count_ = 0;
            other.size_ = 0;
        }
        return *this;
    }

    iterator begin() {
        for (size_type i = 0; i < bucket_count_; ++i) {
            if (buckets_[i] != nullptr) return iterator(buckets_[i], this, i);
        }
        return end();
    }
    const_iterator begin() const {
        for (size_type i = 0; i < bucket_count_; ++i) {
            if (buckets_[i] != nullptr) return const_iterator(buckets_[i], this, i);
        }
        return end();
    }
    iterator end() { return iterator(nullptr, this, bucket_count_); }
    const_iterator end() const { return const_iterator(nullptr, this, bucket_count_); }

    void rehash(size_type new_count) {
        if (new_count <= bucket_count_) return;
        node_ptr* new_buckets = allocate_buckets(new_count);
        for (size_type i = 0; i < bucket_count_; ++i) {
            node_ptr curr = buckets_[i];
            while (curr != nullptr) {
                node_ptr next_node = curr->next;
                size_type new_idx = bucket_index(curr->data, new_count);
                
                curr->next = new_buckets[new_idx];
                new_buckets[new_idx] = curr;
                
                curr = next_node;
            }
        }

        deallocate_buckets(buckets_, bucket_count_);
        buckets_ = new_buckets;
        bucket_count_ = new_count;
    }

    void insert(const T& value) {
        if (load_factor() >= max_load_factor_) {
            rehash(bucket_count_ * 2); 
        }

        size_type idx = bucket_index(value, bucket_count_);
        node_ptr curr = buckets_[idx];

        
        while (curr != nullptr) {
            if (equals_(curr->data, value)) return; 
            curr = curr->next;
        }

        node_ptr new_node = create_node(value);
        new_node->next = buckets_[idx];
        buckets_[idx] = new_node;
        ++size_;
    }

    void erase(const T& value) {
        size_type idx = bucket_index(value, bucket_count_);
        node_ptr curr = buckets_[idx];
        node_ptr prev = nullptr;

        while (curr != nullptr) {
            if (equals_(curr->data, value)) {
                if (prev == nullptr) {
                    buckets_[idx] = curr->next; 
                } else {
                    prev->next = curr->next;
                }
                destroy_node(curr);
                --size_;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    iterator find(const T& value) const {
        size_type idx = bucket_index(value, bucket_count_);
        node_ptr curr = buckets_[idx];

        while (curr != nullptr) {
            if (equals_(curr->data, value)) {
                return iterator(curr, this, idx);
            }
            curr = curr->next;
        }
        return end();
    }

    void clear() {
        for (size_type i = 0; i < bucket_count_; ++i) {
            node_ptr curr = buckets_[i];
            while (curr != nullptr) {
                node_ptr next_node = curr->next;
                destroy_node(curr);
                curr = next_node;
            }
            buckets_[i] = nullptr;
        }
        size_ = 0;
    }
};

} 

#endif