// Copyright 2025 Jimmy Padilla (oJimmy05o@gmail.com)
#include "node.h"

#include <cstdint>
#include <iostream>
#include <vector>

#include "helpers.h"

void Node::print(std::ostream& os) const {
    os << "Node(" << AS_ << "):\n\tproviders={";
    print_container(os, providers_);
    os << "}\n\tcustomers={";
    print_container(os, customers_);
    os << "}\n\tpeers={";
    print_container(os, peers_);
    os << "}";
}

void Node::add_provider(uint32_t AS) {
    providers_.push_back(AS);
}

void Node::add_customer(uint32_t AS) {
    customers_.push_back(AS);
}

void Node::add_peer(uint32_t AS) {
    peers_.push_back(AS);
}

std::ostream& operator<<(std::ostream& os, const Node n) {
    n.print(os);
    return os;
}
