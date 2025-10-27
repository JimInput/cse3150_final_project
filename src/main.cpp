#include "node.h"
#include "graph.h"
#include <memory>
#include <iostream>
#include <utility>

int main() {
    Graph g;
    g.add_peer(777, 4);
    g.add_customer_provider(3, 4);
    g.add_customer_provider(666, 4);
    for (int i = 5; i < 7; i++) {
        g.add_customer_provider(i, 4);
    }

    std::cout << g << std::endl;
    return 0;
}
