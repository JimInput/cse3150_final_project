#include "node.h"
#include "graph.h"
#include <memory>
#include <iostream>
#include <utility>

int main() {
    std::unique_ptr<Graph> g = std::make_unique<Graph>();
    g->add_peer(777, 4);
    g->add_customer_provider(3, 4);
    g->add_customer_provider(666, 4);

    std::cout << *g << std::endl;
    return 0;
}
