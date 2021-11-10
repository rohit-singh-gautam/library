/* @ Rohit Jairaj Singh - rohit@singh.org.in
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <assert.h>
#include <algorithm>
#include <concepts>

namespace rohit {

enum class blancing_type {
    none,
    red_black,
    red_black_leftleaning,
    avl
};

template <typename key_type, typename value_type, blancing_type impl>
    requires std::totally_ordered<key_type>
struct bst_node;

template <typename key_type, typename value_type, blancing_type impl>
    requires std::totally_ordered<key_type>
class bst;

template <typename key_type, typename value_type>
    requires std::totally_ordered<key_type>
struct bst_node_base {
    key_type key;
    value_type value;

    bst_node_base(const key_type &key, const value_type &value) : key(key), value(value) { }
}; // class bst_node_base

template <typename key_type, typename value_type>
    requires std::totally_ordered<key_type>
struct bst_node<key_type, value_type, blancing_type::none> :
    public bst_node_base<key_type, value_type>
{
    using base_type = bst_node_base<key_type, value_type>;

    bst_node *left = nullptr;
    bst_node *right = nullptr;

    bst_node(const key_type &key, const value_type &value) : base_type(key, value) { }
}; // struct bst_node<key_type, value_type, blancing_type::none>

template <typename key_type, typename value_type, blancing_type impl>
    requires std::totally_ordered<key_type>
class bst_base {
public:
    using node_type = bst_node<key_type, value_type, impl>;

    node_type *root = nullptr;

private:
    size_t depth(node_type * root) {
        if (root == nullptr) return 0;
        auto left_depth = depth(root->left);
        auto right_depth = depth(root->right);

        return std::max(left_depth, right_depth) + 1;
    }

public:
    auto find(const key_type &key) {
        auto curr = root;
        while(curr) {
            if (curr->key == key) {
                return curr;
            }

            if (key <= curr->key) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
        return end();
    }

    size_t depth() {
        return depth(root);
    }

    node_type * end() {
        return nullptr;
    }

};

template <typename key_type, typename value_type>
    requires std::totally_ordered<key_type>
class bst<key_type, value_type, blancing_type::none> :
    public bst_base<key_type, value_type, blancing_type::none> {
public:
    using base_type = bst_base<key_type, value_type, blancing_type::none>;
    using node_type = base_type::node_type;
    using base_type::root;

public:
    // Duplicate will be overridden
    void insert(const key_type &key, const value_type &value) {
        if (root == nullptr) {
            root = new node_type(key, value);
            return;
        }

        auto curr = root;
        while(true) {
            if (curr->key == key) {
                curr->value = value;
                break;
            }

            if (key <= curr->key) {
                if (curr->left == nullptr) {
                    curr->left = new node_type(key, value);
                    break;
                }
                curr = curr->left;
            } else {
                if (curr->right == nullptr) {
                    curr->right = new node_type(key, value);
                    break;
                }
                curr = curr->right;
            }
        }
    }

}; // class bst<key_type, value_type, blancing_type::none>

template <typename key_type, typename value_type>
    requires std::totally_ordered<key_type>
struct bst_node<key_type, value_type, blancing_type::red_black> :
    public bst_node_base<key_type, value_type>
{
    using base_type = bst_node_base<key_type, value_type>;
    bool red = true; // By default it will start with red

    bst_node *left = nullptr;
    bst_node *right = nullptr;

    bst_node(const key_type &key, const value_type &value) : base_type(key, value) { }
}; // struct bst_node<key_type, value_type, blancing_type::red_black>

template <typename key_type, typename value_type>
class bst<key_type, value_type, blancing_type::red_black> :
    public bst_base<key_type, value_type, blancing_type::red_black> {
public:
    using base_type = bst_base<key_type, value_type, blancing_type::red_black>;
    using node_type = base_type::node_type;
    using base_type::root;

private:
    node_type * insert_recursive(node_type * root, const key_type &key, const value_type &value) {
        if (root->key == key) {
            root->value = value;
            return root;
        }

        if (key <= root->key) {
            if (root->left == nullptr) {
                root->left = new node_type(key, value);
            } else {
                auto left = insert_recursive(root->left, key, value);
                if (left->red) {
                    if (left->left != nullptr && left->left->red) {
                        assert(!root->red);
                        // left -> left rotation
                        root->left = left->right;
                        left->right = root;
                        left->left->red = false;
                        root = left;
                    } else if (left->right != nullptr && left->right->red) {
                        assert(!root->red);
                        // left -> right rotation
                        auto leftright = left->right;
                        root->left = leftright->right;
                        left->right = leftright->left;
                        leftright->left = left;
                        leftright->right = root;
                        left->red = false;
                        root = leftright;
                    } else root->left = left;
                } // else root->left = left; // Not required
            }
        } else {
            if (root->right == nullptr) {
                root->right = new node_type(key, value);
            } else {
                auto right = insert_recursive(root->right, key, value);
                if (right->red) {
                    if (right->right != nullptr && right->right->red) {
                        assert(!root->red);
                        // right -> right rotation
                        root->right = right->left;
                        right->left = root;
                        root = right;
                        right->right->red = false;
                    } else if (right->left != nullptr && right->left->red) {
                        assert(!root->red);
                        // right -> left rotation
                        auto rightleft = right->left;
                        root->right = rightleft->left;
                        right->left = rightleft->right;
                        rightleft->right = right;
                        rightleft->left = root;
                        right->red = false;
                        root = rightleft;
                    } else root->right = right;
                } // else root->right = right; // Not required
            }
        }

        if (root->left != nullptr && root->right != nullptr && root->left->red && root->right->red) {
            assert(!root->red);
            root->red = true;
            root->left->red = root->right->red = false;
        }

        return root;
    }

public:
    // Duplicate will be overridden
    void insert(const key_type &key, const value_type &value) {
        if (root == nullptr) {
            root = new node_type(key, value);
            root->red = false;
            return;
        }

        auto newroot = insert_recursive(root, key, value);
        if (newroot->red) {
            root = newroot;
            root->red = false;
        }
    }
}; // class bst<key_type, value_type, blancing_type::red_black>

template <typename key_type, typename value_type>
    requires std::totally_ordered<key_type>
struct bst_node<key_type, value_type, blancing_type::red_black_leftleaning> :
    public bst_node_base<key_type, value_type>
{
    using base_type = bst_node_base<key_type, value_type>;
    bool red = true; // By default it will start with red

    bst_node *left = nullptr;
    bst_node *right = nullptr;

    bst_node(const key_type &key, const value_type &value) : base_type(key, value) { }
}; // struct bst_node<key_type, value_type, blancing_type::red_black_leftleaning>

template <typename key_type, typename value_type>
class bst<key_type, value_type, blancing_type::red_black_leftleaning> :
    public bst_base<key_type, value_type, blancing_type::red_black_leftleaning> {
public:
    using base_type = bst_base<key_type, value_type, blancing_type::red_black_leftleaning>;
    using node_type = base_type::node_type;
    using base_type::root;

private:

    bool is_red(node_type * _root) {
        if (_root == nullptr) return false;
        return _root->red;
    }

    void flip_color(node_type * _root) {
        assert(!_root->red);
        assert(_root->left->red);
        assert(_root->right->red);
        _root->red = true;
        _root->left->red = false;
        _root->right->red = false;
    }

    node_type * rotate_left(node_type * _root) {
        auto right = _root->right;
        _root->right = right->left;
        right->left = _root;
        right->red = _root->red;
        _root->red = true;

        assert(right != nullptr);
        return right;
    }

    node_type * rotate_right(node_type * _root) {
        assert(!_root->red);
        auto left = _root->left;
        _root->left = left->right;
        left->right = _root;
        left->red = true;
        left->left->red = false;

        assert(left != nullptr);
        return left;
    }

    node_type * insert_recursive(node_type * _root, const key_type &key, const value_type &value) {
        if (_root == nullptr) return new node_type(key, value);
        if (key < _root->key) _root->left = insert_recursive(_root->left, key, value);
        else if (key > _root->key) _root->right = insert_recursive(_root->right, key, value);
        else _root->value = value;

        if (is_red(_root->right) && !is_red(_root->left)) _root = rotate_left(_root);
        if (is_red(_root->left) && is_red(_root->left->left)) _root = rotate_right(_root);
        if (is_red(_root->left) && is_red(_root->right)) flip_color(_root);

        return _root;
    }

public:
    // Duplicate will be overridden
    void insert(const key_type &key, const value_type &value) {
        root = insert_recursive(root, key, value);
        root->red = false;
    }
}; // class bst<key_type, value_type, blancing_type::red_black_leftleaning>

template <typename key_type, typename value_type>
    requires std::totally_ordered<key_type>
struct bst_node<key_type, value_type, blancing_type::avl> :
    public bst_node_base<key_type, value_type>
{
    using base_type = bst_node_base<key_type, value_type>;
    int count = 1; // initial count is always one

    bst_node *left = nullptr;
    bst_node *right = nullptr;

    bst_node(const key_type &key, const value_type &value) : base_type(key, value) { }
}; // struct bst_node<key_type, value_type, blancing_type::red_black>


template <typename key_type, typename value_type>
class bst<key_type, value_type, blancing_type::avl> :
    public bst_base<key_type, value_type, blancing_type::avl> {
public:
    using base_type = bst_base<key_type, value_type, blancing_type::avl>;
    using node_type = base_type::node_type;
    using base_type::root;

private:
    int child_count_diff(node_type * _root) {
        int left = _root->left ? _root->left->count : 0;
        int right = _root->right ? _root->right->count : 0;
        return left - right;
    }
    void update_count(node_type * _root) {
        int left = _root->left ? _root->left->count : 0;
        int right = _root->right ? _root->right->count : 0;
        _root->count = std::max(left, right) + 1;
    }

    //      a              b
    //         b   =>   a     c
    //            c
    node_type * rotate_left(node_type * _root) {
        auto right = _root->right;
        _root->right = right->left;
        right->left = _root;
        update_count(_root);
        update_count(right);
        assert(right != nullptr);
        return right;
    }

    //          a           b
    //       b     =>    c     a
    //    c
    node_type * rotate_right(node_type * _root) {
        auto left = _root->left;
        _root->left = left->right;
        left->right = _root;
        update_count(root);
        update_count(left);
        assert(left != nullptr);
        return left;
    }

    //      a               c
    //         b   =>    a     b
    //      c
    node_type * rotate_right_left(node_type * _root) {
        auto newroot = _root->right->left;
        _root->right->left = newroot->right;
        newroot->right = _root->right;
        _root->right = newroot->left;
        newroot->left = _root;
        update_count(newroot->left);
        update_count(newroot->right);
        update_count(newroot);
        return newroot;
    }

    //          a           c
    //       b     =>    b     a
    //          c
    node_type * rotate_left_right(node_type * _root) {
        auto newroot = _root->left->right;
        _root->left->right = newroot->left;
        newroot->left = root->left;
        _root->left = newroot->right;
        newroot->right = _root;
        update_count(newroot->left);
        update_count(newroot->right);
        update_count(newroot);
        return newroot;
    }

    node_type * insert_recursive(node_type * _root, const key_type &key, const value_type &value) {
        if (_root == nullptr) return new node_type(key, value);
        if (key < _root->key) _root->left = insert_recursive(_root->left, key, value);
        else if (key > _root->key) _root->right = insert_recursive(_root->right, key, value);
        else _root->value = value;
    
        auto count_diff = child_count_diff(_root);
        if (count_diff <= -2) _root = rotate_left(_root);
        else if (count_diff >= 2) _root = rotate_right(_root);
        else update_count(_root);

        return _root;
    }

public:
    // Duplicate will be overridden
    void insert(const key_type &key, const value_type &value) {
        root = insert_recursive(root, key, value);
    }
}; // class bst<key_type, value_type, blancing_type::avl>


} // namespace rohit