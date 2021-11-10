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
#include <display.hh>
#include <tree_traversal.hh>
#include <vector>
#include <string>
#include <iostream>

namespace rohit {

template <typename key_type, typename value_type, blancing_type impl>
void display_inorder(bst_node<key_type, value_type, impl> *root) {
    if (root == nullptr) return;
    display_inorder(root->left);
    std::cout << root->key << " ";
    display_inorder(root->right);
}

template <typename key_type, typename value_type, blancing_type impl>
void display_inorder(bst<key_type, value_type, impl> &bst) {
    auto list = inorder(bst);
    std::vector<key_type> key_list;
    std::ranges::copy(std::views::transform(list, [](auto &val) { return val.first; }), std::back_inserter(key_list));
    std::cout << key_list;
}

template <typename key_type, typename value_type, blancing_type impl>
void display_postorder(bst_node<key_type, value_type, impl> *root) {
    if (root == nullptr) return;
    display_postorder(root->left);
    display_postorder(root->right);
    std::cout << root->key << " ";
}

template <typename key_type, typename value_type, blancing_type impl>
void display_postorder(bst<key_type, value_type, impl> &bst) {
    auto list = postorder(bst);
    std::vector<key_type> key_list;
    std::ranges::copy(std::views::transform(list, [](auto &val) { return val.first; }), std::back_inserter(key_list));
    std::cout << key_list;
}

template <typename key_type, typename value_type, blancing_type impl>
void display_preorder(bst<key_type, value_type, impl> &bst) {
    auto list = preorder(bst);
    std::vector<key_type> key_list;
    std::ranges::copy(std::views::transform(list, [](auto &val) { return val.first; }), std::back_inserter(key_list));
    std::cout << key_list;
}

template <typename key_type, typename value_type, blancing_type impl>
size_t display_tree(bst_node<key_type, value_type, impl> *root, std::vector<std::string> &lines, size_t depth, size_t maxdepth, size_t minwidth) {
    if (root == nullptr) {
        while(depth < maxdepth) {
            auto &line = lines[depth++];
            line.append(minwidth, ' ');
        }
        return minwidth;
    }
    std::string curr_str = std::to_string(root->key);
    if constexpr (impl == blancing_type::avl) {
        curr_str += ":" + std::to_string(root->count);
    }
    auto width = std::max(curr_str.size() + 2, minwidth);
    auto left_width = display_tree(root->left, lines, depth + 1, maxdepth, (width + 1)/2);
    auto right_width = display_tree(root->right, lines, depth + 1, maxdepth, width/2);
    width = left_width + right_width;

    auto &line = lines[depth];
    auto dash_width = width - curr_str.size() - 2;
    line.append(1, ' ');
    line.append((dash_width + 1)/2, '-');
    if constexpr (impl == blancing_type::red_black || impl == blancing_type::red_black_leftleaning) {
        if (root->red == true) {
            line.append("\033[0;31m");
        }
    }
    line.append(curr_str);
    if constexpr (impl == blancing_type::red_black || impl == blancing_type::red_black_leftleaning) {
        if (root->red == true) {
            line.append("\033[0m");
        }
    }
    line.append(dash_width/2, '-');
    line.append(1, ' ');
    return width;
}

template <typename key_type, typename value_type, blancing_type impl>
void display_tree(bst<key_type, value_type, impl> &bst) {
    auto depth = bst.depth();
    std::vector<std::string> lines;
    for(size_t count = 0; count < depth; ++count) {
        lines.push_back("");
    }

    display_tree(bst.root, lines, 0, depth, 0);

    for(auto line: lines) {
        std::cout << line << std::endl;
    }
    std::cout << std::endl;
}

} // namespace rohit