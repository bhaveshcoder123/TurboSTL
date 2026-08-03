#ifndef MYSTL_MAP_HPP
#define MYSTL_MAP_HPP

#include "../core/rb_tree.hpp"
#include "../core/functional.hpp"
#include <utility>

namespace mystl {

template <typename Key, typename T, typename Compare = mystl::less<Key>, typename Allocator = mystl::allocator<std::pair<const Key, T>>>
class map {
public:
    using key_type       = Key;
    using mapped_type    = T;
    using value_type     = std::pair<const Key, T>; 
    using key_compare    = Compare;
    using size_type      = std::size_t;
    using allocator_type = Allocator;

   
    struct value_compare {
        Compare comp;
        value_compare() : comp() {}
        bool operator()(const value_type& lhs, const value_type& rhs) const {
            return comp(lhs.first, rhs.first);
        }
    };

    using tree_type      = rb_tree<value_type, value_compare, allocator_type>;
    using iterator       = typename tree_type::iterator;
    using const_iterator = typename tree_type::const_iterator;

private:
    tree_type tree_;

public:
    map() = default;
    ~map() = default;

    iterator begin() { return tree_.begin(); }
    iterator end()   { return tree_.end(); }
    const_iterator begin() const { return tree_.begin(); }
    const_iterator end() const   { return tree_.end(); }
    const_iterator cbegin() const { return tree_.cbegin(); }
    const_iterator cend() const   { return tree_.cend(); }

    bool empty() const { return tree_.empty(); }
    size_type size() const { return tree_.size(); }

    void insert(const value_type& value) { 
        tree_.insert(value); 
    }

    void clear() { 
        tree_.clear(); 
    }

    void erase(const key_type& key) { 
        tree_.erase(value_type(key, mapped_type())); 
    }

    iterator find(const key_type& key) {
        return tree_.find(value_type(key, mapped_type()));
    }

    const_iterator find(const key_type& key) const {
        return tree_.find(value_type(key, mapped_type()));
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