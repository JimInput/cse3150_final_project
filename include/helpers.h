// Copyright 2025 Jimmy Padilla (oJimmy05o@gmail.com)
#pragma once

#include <iostream>
#include <utility>

template <typename Con>
void print_container(std::ostream& os, const Con& con) {
    if (con.size() == 0) return;
    os << "(";
    for (auto it = con.begin(); it != std::prev(con.end()); it++) {
        os << *it << ",";
    }
    os << *std::prev(con.end()) << ")";
}

template <typename Con>
void print_reverse_container(std::ostream& os, const Con& con) {
    if (con.size() == 0) return;
    os << "\"(";
    if (con.size() > 1) {
        for (auto it = con.rbegin(); it != std::prev(con.rend()); it++) {
            os << *it << ", ";
        }
        os << *std::prev(con.rend()) << ")\"";
    } else {
        os << *std::prev(con.rend()) << ",)\"";
    }
    
}
