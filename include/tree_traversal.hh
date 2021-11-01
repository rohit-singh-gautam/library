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
#include <tree.hh>
#include <vector>
#include <stack>
#include <ranges>

namespace rohit {

template <typename key_type, typename value_type, blancing_type impl>
std::vector<std::pair<key_type, value_type>> inorder(bst_node<key_type, value_type, impl> *root) {
    std::vector<std::pair<key_type, value_type>> results;
    std::stack<bst_node<key_type, value_type, impl> *> st;
    
    while(root) {
        st.push(root);
        root = root->left;
    }

    while(!st.empty()) {
        auto top = st.top();
        results.push_back(std::make_pair(top->key, top->value));
        st.pop();
        auto root = top->right;
        while(root) {
            st.push(root);
            root = root->left;
        }
    }
    return results;    
}

template <typename key_type, typename value_type, blancing_type impl>
std::vector<std::pair<key_type, value_type>> inorder(bst<key_type, value_type, impl> &bst) {
    return inorder(bst.root);
}

} // namespace rohit