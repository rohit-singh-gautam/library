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

#include <assert.h>

namespace rohit {

enum class blancing_type {
    none,
    red_black,
};

template <typename key_type, typename value_type, blancing_type impl>
struct bst_node;

template <typename key_type, typename value_type, blancing_type impl>
class bst;

template <typename key_type, typename value_type, blancing_type impl>
struct bst_node {
    key_type key;
    value_type value;

    bst_node *left = nullptr;
    bst_node *right = nullptr;

    bst_node(const key_type &key, const value_type &value) : key(key), value(value) { }
}; // class bst_node

template <typename key_type, typename value_type>
class bst<key_type, value_type, blancing_type::none>  {
    using node_type = bst_node<key_type, value_type, blancing_type::none>;
    using iterator = node_type *;
private:
    node_type *root = nullptr;

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

    iterator end() {
        return nullptr;
    }

}; // class bst<key_type, value_type, blancing_type::none>

template <typename key_type, typename value_type>
struct bst_node<key_type, value_type, blancing_type::red_black> {
    key_type key;
    value_type value;

    bool red = true; // By default it will start with red

    bst_node *left = nullptr;
    bst_node *right = nullptr;

    bst_node(const key_type &key, const value_type &value) : key(key), value(value) { }
}; // struct bst_node<key_type, value_type, blancing_type::red_black>

template <typename key_type, typename value_type>
class bst<key_type, value_type, blancing_type::red_black>  {
public:
    using node_type = bst_node<key_type, value_type, blancing_type::red_black>;
    using iterator = node_type *;

private:
    iterator root = nullptr;

    iterator insert_recursive(iterator root, const key_type &key, const value_type &value) {
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
                    if (left->left->red) {
                        assert(!root->red);
                        // left -> left rotation
                        root->left = left->right;
                        left->right = root;
                        left->left->red = false;
                        root = left;
                    } else if (left->right->red) {
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
                    if (right->right->red) {
                        assert(!root->red);
                        // right -> right rotation
                        root->right = right->left;
                        right->left = root;
                        root = right;
                        right->right->red = false;
                    } else if (right->left->red) {
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
            root->left->red = root->right->red = true;
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

    iterator end() {
        return nullptr;
    }
}; // class bst<key_type, value_type, blancing_type::red_black>


} // namespace rohit