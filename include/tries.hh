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

template <typename key_type, typename value_type, typename TLIST>
struct tries_node {
    typedef tries_node* iterator;
    using key_char_type = key_type::value_type;

    value_type value;
    key_char_type key_char;

    tries_node(key_char_type key_char = default_value<key_char_type>::value) : key_char(key_char) { }

    TLIST children;

    auto end() { return children.end(); }

}; // class tries_node

template <typename key_type, typename value_type = bool>
struct tries_unordered_map {
    using key_char_type = key_type::value_type;
    using node_type = tries_node<key_type, value_type, tries_unordered_map>;
    using iterator = node_type::iterator;

    std::unordered_map<key_char_type, node_type *> list;

    auto find(const node_type::key_char_type &key_char) {
        auto result = list.find(key_char);
        if (result == list.end()) {
            return end();
        }

        return result->second;
    }

    auto insert(const key_char_type& key_char) {
        auto child = new node_type(key_char);
        list.insert(make_pair(key_char, child));
        return child;
    }

    iterator end() {
        return nullptr;
    }
};


template <typename key_type, typename value_type, typename TLIST>
class tries : private tries_node<key_type, value_type, TLIST> {
    using node_type = tries_node<key_type, value_type, TLIST>;
    using iterator = tries_node<key_type, value_type, TLIST>::iterator;
    using node_type::end;

public:
    tries() {}

    void insert(const key_type& key, value_type&& value) {
        iterator itr = this;
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
        iterator itr = this;
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