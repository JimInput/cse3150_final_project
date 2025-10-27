#pragma once

#include <utility>
#include <iostream>

template <typename Con>
void print_container(std::ostream& os, const Con& con) {
    if (con.size() == 0) return;
    for (auto it = con.begin(); it != std::prev(con.end()); it++) {
        os << *it << ",";
    }
    os << *std::prev(con.end());
}
