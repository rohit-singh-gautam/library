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

#include <tree.hh>
#include <string>
#include <unordered_map>

namespace rohit {

template <typename value_type>
struct default_value;

template<>
struct default_value<bool> {
    static constexpr bool value = false;
};

template<>
struct default_value<char> {
    static constexpr char value = '\0';
};

template<>
struct default_value<const char> {
    static constexpr char value = '\0';
};

template<>
struct default_value<int> {
    static constexpr int value = 0;
};

template<>
struct default_value<double> {
    static constexpr double value = 0.0;
};

template <typename value_type, typename TLIST>
struct tries_node {
    typedef tries_node* iterator;
    value_type value = default_value<value_type>::value;

    TLIST children;

    auto end() { return children.end(); }
}; // class tries_node

template <typename key_type, typename value_type = bool>
struct tries_unordered_map {
    using key_char_type = key_type::value_type;
    using node_type = tries_node<value_type, tries_unordered_map>;
    using iterator = node_type::iterator;

    std::unordered_map<key_char_type, node_type *> list;

    auto find(const key_char_type &key_char) {
        auto result = list.find(key_char);
        if (result == list.end()) {
            return end();
        }

        return result->second;
    }

    auto insert(const key_char_type& key_char) {
        auto child = new node_type();
        list.insert(make_pair(key_char, child));
        return child;
    }

    iterator end() {
        return nullptr;
    }
};

template <typename key_type, typename value_type = bool, blancing_type impl = blancing_type::none>
struct tries_tree {
    using key_char_type = key_type::value_type;
    using node_type = tries_node<value_type, tries_tree>;
    using iterator = node_type::iterator;

    rohit::bst<key_char_type, node_type *, impl> list;

    iterator find(const key_char_type &key_char) {
        auto result = list.find(key_char);
        if (result == list.end()) {
            return end();
        }

        return result->value;
    }

    auto insert(const key_char_type& key_char) {
        auto child = new node_type();
        list.insert(key_char, child);
        return child;
    }

    iterator end() {
        return nullptr;
    }
};

template <typename key_type, typename value_type, typename TLIST>
class tries {
    using node_type = tries_node<value_type, TLIST>;
    using iterator = tries_node<value_type, TLIST>::iterator;

    node_type root;

public:
    tries() {}

    void insert(const key_type& key, value_type&& value) {
        iterator itr = &root;
        auto key_itr = key.begin();
        for(;key_itr != key.end(); ++key_itr) {
            iterator child_itr = itr->children.find(*key_itr);
            if (child_itr == itr->children.end()) {
                itr = itr->children.insert(*key_itr++);
                for(;key_itr != key.end(); ++key_itr) {
                    itr = itr->children.insert(*key_itr);
                }
                break;
            }
            itr = child_itr;
        }
        itr->value = value;
    }

    auto search(const key_type& key) {
        iterator itr = &root;
        for(auto key_ch: key) {
            itr = itr->children.find(key_ch);
            if (itr == itr->children.end()) {
                return end();
            }
        }

        return itr;
    }

    bool contains(const key_type& key) {
        auto result = search(key);
        if (result == end()) {
            return false;
        }

        return result->value != default_value<value_type>::value;
    }

    auto end() {
        return root.end();
    }

};

template <typename key_type, typename TLIST>
class tries_set : public tries<key_type, bool, TLIST> {
    using tries_type = tries<key_type, bool, TLIST>;
public:
    void insert(key_type& key) {
        tries_type::insert(key, true);
    }
};

} // namespace rohit