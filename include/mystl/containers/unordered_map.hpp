#ifndef MYSTL_UNORDERED_MAP_HPP
#define MYSTL_UNORDERED_MAP_HPP

#include "../core/hashtable.hpp"
#include "../core/functional.hpp"
#include <utility>

namespace mystl {

template <typename Key, typename T, typename Hash = mystl::hash<Key>, typename Equal = mystl::equal_to<Key>, typename Allocator = mystl::allocator<std::pair<const Key, T>>>
class unordered_map {
public:
    using key_type       = Key;
    using mapped_type    = T;
    using value_type     = std::pair<const Key, T>; 
    using hasher         = Hash;
    using key_equal      = Equal;
    using size_type      = std::size_t;
    using allocator_type = Allocator;

private:
    struct value_hasher {
        hasher h;
        size_type operator()(const value_type& val) const {
            return h(val.first); 
        }
    };

    struct value_equal {
        key_equal eq;
        bool operator()(const value_type& lhs, const value_type& rhs) const {
            return eq(lhs.first, rhs.first); 
        }
    };

    using table_type = hashtable<value_type, value_hasher, value_equal, allocator_type>;
    table_type ht_;

public:
    using iterator       = typename table_type::iterator;
    using const_iterator = typename table_type::const_iterator;

    unordered_map() : ht_(8) {}
    ~unordered_map() = default;
    unordered_map(const unordered_map&) = default;
    unordered_map& operator=(const unordered_map&) = default;
    unordered_map(unordered_map&&) noexcept = default;
    unordered_map& operator=(unordered_map&&) noexcept = default;

    iterator begin() { return ht_.begin(); }
    iterator end()   { return ht_.end(); }
    const_iterator begin() const { return ht_.begin(); }
    const_iterator end() const   { return ht_.end(); }

    bool empty() const { return ht_.empty(); }
    size_type size() const { return ht_.size(); }
    float load_factor() const { return ht_.load_factor(); }

    void insert(const value_type& value) { 
        ht_.insert(value); 
    }

    void clear() { 
        ht_.clear(); 
    }

    void erase(const key_type& key) { 
        ht_.erase(value_type(key, mapped_type())); 
    }

    iterator find(const key_type& key) {
        return ht_.find(value_type(key, mapped_type()));
    }

    const_iterator find(const key_type& key) const {
        return ht_.find(value_type(key, mapped_type()));
    }

    mapped_type& operator[](const key_type& key) {
        iterator it = find(key);
        if (it == end()) {
            insert(value_type(key, mapped_type()));
            it = find(key);
        }
        return it->second;
    }
};

} 

#endif