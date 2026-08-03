#ifndef MYSTL_SET_HPP
#define MYSTL_SET_HPP

#include "../core/rb_tree.hpp"
#include "../core/functional.hpp"

namespace mystl {

template <typename Key, typename Compare = mystl::less<Key>, typename Allocator = mystl::allocator<Key>>
class set {
public:
    using key_type       = Key;
    using value_type     = Key; 
    using size_type      = std::size_t;
    using key_compare    = Compare;
    using value_compare  = Compare;
    using allocator_type = Allocator;
    
    using tree_type      = rb_tree<value_type, key_compare, allocator_type>;
    using iterator       = typename tree_type::const_iterator;
    using const_iterator = typename tree_type::const_iterator;

private:
    tree_type tree_;

public:
    set() = default;
    ~set() = default;

    iterator begin() { return tree_.begin(); }
    iterator end()   { return tree_.end(); }

    bool empty() const { return tree_.empty(); }
    size_type size() const { return tree_.size(); }

    void insert(const value_type& value) {
        tree_.insert(value);
    }

    void erase(const value_type& value) {
        tree_.erase(value);
    }

    void clear() {
        tree_.clear();
    }

    iterator find(const key_type& key) const {
        return tree_.find(key);
    }
};

} 

#endif