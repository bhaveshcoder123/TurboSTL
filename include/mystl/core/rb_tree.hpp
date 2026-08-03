#ifndef MYSTL_RB_TREE_HPP
#define MYSTL_RB_TREE_HPP

#include "memory.hpp"
#include "iterator.hpp"
#include "functional.hpp"
#include <cstddef>
#include <utility>

namespace mystl {

enum class rb_color { RED, BLACK };

template <typename T>
struct rb_tree_node {
    T data;
    rb_color color;
    rb_tree_node* parent;
    rb_tree_node* left;
    rb_tree_node* right;

    template <typename... Args>
    rb_tree_node(Args&&... args) 
        : data(std::forward<Args>(args)...), color(rb_color::RED), 
          parent(nullptr), left(nullptr), right(nullptr) {}
};

template <typename T, typename Pointer, typename Reference>
struct rb_tree_iterator {
    using iterator_category = bidirectional_iterator_tag;
    using value_type        = T;
    using difference_type   = std::ptrdiff_t;
    using pointer           = Pointer;
    using reference         = Reference;

    using node_ptr = rb_tree_node<T>*;
    using self     = rb_tree_iterator<T, Pointer, Reference>;

    node_ptr node;
    node_ptr root;

    rb_tree_iterator() : node(nullptr), root(nullptr) {}
    rb_tree_iterator(node_ptr n, node_ptr r) : node(n), root(r) {}
    template <typename U_Ptr, typename U_Ref>
    rb_tree_iterator(const rb_tree_iterator<T, U_Ptr, U_Ref>& other) 
        : node(other.node), root(other.root) {}

    reference operator*() const { return node->data; }
    pointer operator->() const  { return &(node->data); }

    self& operator++() {
        if (node->right != nullptr) {
            node = node->right;
            while (node->left != nullptr) node = node->left;
        } else {
            node_ptr p = node->parent;
            while (p != nullptr && node == p->right) {
                node = p;
                p = p->parent;
            }
            node = p;
        }
        return *this;
    }

    bool operator==(const self& other) const { return node == other.node; }
    bool operator!=(const self& other) const { return node != other.node; }
};


template <typename T, typename Compare = mystl::less<T>, typename Allocator = mystl::allocator<T>>
class rb_tree {
public:
    using node_type      = rb_tree_node<T>;
    using node_ptr       = node_type*;
    using node_allocator = typename Allocator::template rebind<node_type>::other;
    using iterator       = rb_tree_iterator<T, T*, T&>;
    using const_iterator = rb_tree_iterator<T, const T*, const T&>;
    using size_type      = std::size_t;

private:
    node_allocator alloc_;
    node_ptr root_;
    size_type size_;
    Compare comp_;

    node_ptr create_node(const T& value) {
        node_ptr n = alloc_.allocate(1);
        alloc_.construct(n, value);
        return n;
    }

    void destroy_node(node_ptr n) {
        alloc_.destroy(n);
        alloc_.deallocate(n, 1);
    }

    rb_color get_color(node_ptr n) const {
        return n == nullptr ? rb_color::BLACK : n->color;
    }

    void set_color(node_ptr n, rb_color c) {
        if (n != nullptr) n->color = c;
    }

    node_ptr minimum(node_ptr n) const {
        while (n->left != nullptr) n = n->left;
        return n;
    }

    void left_rotate(node_ptr x) {
        node_ptr y = x->right;
        x->right = y->left;
        if (y->left != nullptr) y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr) root_ = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void right_rotate(node_ptr y) {
        node_ptr x = y->left;
        y->left = x->right;
        if (x->right != nullptr) x->right->parent = y;
        x->parent = y->parent;
        if (y->parent == nullptr) root_ = x;
        else if (y == y->parent->right) y->parent->right = x;
        else y->parent->left = x;
        x->right = y;
        y->parent = x;
    }

    void transplant(node_ptr u, node_ptr v) {
        if (u->parent == nullptr) root_ = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        if (v != nullptr) v->parent = u->parent;
    }

    void insert_fixup(node_ptr z) {
        while (z->parent != nullptr && z->parent->color == rb_color::RED) {
            if (z->parent == z->parent->parent->left) {
                node_ptr y = z->parent->parent->right;
                if (get_color(y) == rb_color::RED) {
                    set_color(z->parent, rb_color::BLACK);
                    set_color(y, rb_color::BLACK);
                    set_color(z->parent->parent, rb_color::RED);
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        left_rotate(z);
                    }
                    set_color(z->parent, rb_color::BLACK);
                    set_color(z->parent->parent, rb_color::RED);
                    right_rotate(z->parent->parent);
                }
            } else {
                node_ptr y = z->parent->parent->left;
                if (get_color(y) == rb_color::RED) {
                    set_color(z->parent, rb_color::BLACK);
                    set_color(y, rb_color::BLACK);
                    set_color(z->parent->parent, rb_color::RED);
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        right_rotate(z);
                    }
                    set_color(z->parent, rb_color::BLACK);
                    set_color(z->parent->parent, rb_color::RED);
                    left_rotate(z->parent->parent);
                }
            }
        }
        root_->color = rb_color::BLACK;
    }

    void erase_fixup(node_ptr x, node_ptr x_parent) {
        while (x != root_ && get_color(x) == rb_color::BLACK) {
            if (x == (x_parent ? x_parent->left : nullptr)) {
                node_ptr w = x_parent->right;
                if (get_color(w) == rb_color::RED) {
                    set_color(w, rb_color::BLACK);
                    set_color(x_parent, rb_color::RED);
                    left_rotate(x_parent);
                    w = x_parent->right;
                }
                if (get_color(w->left) == rb_color::BLACK && get_color(w->right) == rb_color::BLACK) {
                    set_color(w, rb_color::RED);
                    x = x_parent;
                    x_parent = x->parent;
                } else {
                    if (get_color(w->right) == rb_color::BLACK) {
                        set_color(w->left, rb_color::BLACK);
                        set_color(w, rb_color::RED);
                        right_rotate(w);
                        w = x_parent->right;
                    }
                    set_color(w, get_color(x_parent));
                    set_color(x_parent, rb_color::BLACK);
                    set_color(w->right, rb_color::BLACK);
                    left_rotate(x_parent);
                    x = root_; 
                }
            } else {
                node_ptr w = x_parent->left;
                if (get_color(w) == rb_color::RED) {
                    set_color(w, rb_color::BLACK);
                    set_color(x_parent, rb_color::RED);
                    right_rotate(x_parent);
                    w = x_parent->left;
                }
                if (get_color(w->right) == rb_color::BLACK && get_color(w->left) == rb_color::BLACK) {
                    set_color(w, rb_color::RED);
                    x = x_parent;
                    x_parent = x->parent;
                } else {
                    if (get_color(w->left) == rb_color::BLACK) {
                        set_color(w->right, rb_color::BLACK);
                        set_color(w, rb_color::RED);
                        left_rotate(w);
                        w = x_parent->left;
                    }
                    set_color(w, get_color(x_parent));
                    set_color(x_parent, rb_color::BLACK);
                    set_color(w->left, rb_color::BLACK);
                    right_rotate(x_parent);
                    x = root_;
                }
            }
        }
        set_color(x, rb_color::BLACK);
    }

    void clear_recursive(node_ptr n) {
        if (n == nullptr) return;
        clear_recursive(n->left);
        clear_recursive(n->right);
        destroy_node(n);
    }

public:
    rb_tree() : root_(nullptr), size_(0) {}

    ~rb_tree() { clear(); }

    size_type size() const { return size_; }
    bool empty() const { return size_ == 0; }

    void clear() {
        clear_recursive(root_);
        root_ = nullptr;
        size_ = 0;
    }

    iterator begin() {
        node_ptr curr = root_;
        if (curr != nullptr) {
            while (curr->left != nullptr) curr = curr->left;
        }
        return iterator(curr, root_);
    }

    iterator end() { return iterator(nullptr, root_); }

    const_iterator begin() const {
        node_ptr curr = root_;
        if (curr != nullptr) {
            while (curr->left != nullptr) curr = curr->left;
        }
        return const_iterator(curr, root_);
    }
    const_iterator end() const { return const_iterator(nullptr, root_); }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }

    iterator find(const T& value) {
        node_ptr curr = root_;
        while (curr != nullptr) {
            if (comp_(value, curr->data)) {
                curr = curr->left;
            } else if (comp_(curr->data, value)) {
                curr = curr->right;
            } else {
                return iterator(curr, root_);
            }
        }
        return end(); 
    }

    const_iterator find(const T& value) const {
        node_ptr curr = root_;
        while (curr != nullptr) {
            if (comp_(value, curr->data)) {
                curr = curr->left;
            } else if (comp_(curr->data, value)) {
                curr = curr->right;
            } else {
                return const_iterator(curr, root_);
            }
        }
        return end();
    }

    void insert(const T& value) {
        node_ptr z = create_node(value);
        node_ptr y = nullptr;
        node_ptr x = root_;

        while (x != nullptr) {
            y = x;
            if (comp_(z->data, x->data)) x = x->left;
            else if (comp_(x->data, z->data)) x = x->right;
            else {
                destroy_node(z); 
                return;
            }
        }

        z->parent = y;
        if (y == nullptr) root_ = z;
        else if (comp_(z->data, y->data)) y->left = z;
        else y->right = z;

        insert_fixup(z);
        size_++;
    }

    void erase(const T& value) {
        node_ptr z = root_;
        while (z != nullptr) {
            if (comp_(value, z->data)) z = z->left;
            else if (comp_(z->data, value)) z = z->right;
            else break; 
        }

        if (z == nullptr) return; 

        node_ptr y = z;
        rb_color y_original_color = y->color;
        node_ptr x = nullptr;
        node_ptr x_parent = nullptr;

        if (z->left == nullptr) {
            x = z->right;
            x_parent = z->parent;
            transplant(z, z->right);
        } else if (z->right == nullptr) {
            x = z->left;
            x_parent = z->parent;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            
            if (y->parent == z) {
                x_parent = y;
            } else {
                x_parent = y->parent;
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        destroy_node(z);
        size_--;

        if (y_original_color == rb_color::BLACK) {
            erase_fixup(x, x_parent);
        }
    }
};

}

#endif