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

#include <tries.hh>
#include <string>
#include <vector>
#include <iostream>

typedef rohit::tries_set<std::string, rohit::tries_unordered_map<std::string>> string_tries_old;
typedef rohit::tries_set<std::string, rohit::tries_tree<std::string, bool, rohit::blancing_type::none>> string_tries_old1;
typedef rohit::tries_set<std::string, rohit::tries_tree<std::string, bool, rohit::blancing_type::red_black>> string_tries;

void insert_all(string_tries &set_tries, const std::vector<std::string> &data) {
    for(auto value: data) {
        set_tries.insert(value);
    }
}

void search_all(string_tries &set_tries, const std::vector<std::string> &data) {
    for(auto value: data) {
        std::cout << "Search: " << value << " - ";
        if (set_tries.contains(value)) {
            std::cout << "true";
        } else {
            std::cout << "false";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char *argv[]) {
    string_tries set_tries;

    std::vector<std::string> data = {
        "This",
        "Rohit",
        "Classical",
        "Rohit Singh",
        "Thakur",
        "Thakurana",
        "Questions",
        "Quest",
        "Classic",
        "unknown"
    };

    insert_all(set_tries, data);

    search_all(set_tries, data);

    std::vector<std::string> data_bad = {
        "Great",
        "Hero",
        "Plus Zero",
        "Do not crash",
        "This is too bad",
    };

    search_all(set_tries, data_bad);

    return 0;
}