#ifndef MYSTL_UNORDERED_SET_HPP
#define MYSTL_UNORDERED_SET_HPP

#include "../core/hashtable.hpp"
#include "../core/functional.hpp"

namespace mystl {

template <typename Key, typename Hash = mystl::hash<Key>, typename Equal = mystl::equal_to<Key>, typename Allocator = mystl::allocator<Key>>
class unordered_set {
public:
    using key_type       = Key;
    using value_type     = Key; 
    using size_type      = std::size_t;
    using hasher         = Hash;
    using key_equal      = Equal;
    using allocator_type = Allocator;

private:
    using table_type = hashtable<value_type, hasher, key_equal, allocator_type>;
    table_type ht_;

public:
    using iterator       = typename table_type::const_iterator;
    using const_iterator = typename table_type::const_iterator;

    unordered_set() : ht_(8) {}
    ~unordered_set() = default;

    iterator begin() const { return ht_.begin(); }
    iterator end() const   { return ht_.end(); }
    const_iterator cbegin() const { return ht_.begin(); }
    const_iterator cend() const   { return ht_.end(); }

    bool empty() const { return ht_.empty(); }
    size_type size() const { return ht_.size(); }
    
    float load_factor() const { return ht_.load_factor(); }

    void insert(const value_type& value) { 
        ht_.insert(value); 
    }

    void erase(const value_type& value) { 
        ht_.erase(value); 
    }

    void clear() { 
        ht_.clear(); 
    }

    iterator find(const key_type& key) const {
        return ht_.find(key);
    }
};

} 

#endif