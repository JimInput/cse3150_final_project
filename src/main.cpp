#include "node.h"
#include "graph.h"
#include <memory>
#include <iostream>
#include <utility>

int main() {
    std::unique_ptr<Graph> g = std::make_unique<Graph>();
    g->add_customer_provider(0, 2);
    g->add_customer_provider(3, 0);

    for (auto& val : g->get_nodes()) {
        std::cout << *val.second << std::endl; 
    }

    return 0;
}
