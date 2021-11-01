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

} // namespace rohit