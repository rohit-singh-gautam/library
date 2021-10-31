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
#include <tree_display.hh>

int main(int argc, char *argv[]) {
    constexpr int values[] = {
        80, 60, 90, 33, 125, 22, 88, 66, 24, 11, 13, 111, 215, 86, 25, 35, 86, 12, 13, 113, 118, 18,
        1, 201, 2, 202, 302, 3, 203, 303, 403, 4, 104, 204, 304, 404, 5, 105, 205, 305, 405,
        6, 406, 306, 206, 106, 107, 7, 207, 407, 307, 8, 408, 108, 208, 308,
        501, 502, 503, 504, 505, 506, 507, 508, 509,
        601, 602, 603, 604, 605, 606, 607, 608, 609, 610,
        701, 702, 703, 704, 705, 706, 707, 708, 709, 710
    };

    rohit::bst<int, bool, rohit::blancing_type::red_black> tree;
    size_t count = 0;

    for(auto value: values) {
        std::cout << "Count: " << ++count << "; adding: " << value << std::endl;
        tree.insert(value, true);
        std::cout << "Inorder: ";
        rohit::display_inorder(tree);
        std::cout << "\nPostorder: ";
        rohit::display_postorder(tree);
        std::cout << std::endl;
    }

    return 0;
}