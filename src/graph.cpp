#include "graph.h"

#include <cstdint>

void Graph::add_node(uint32_t AS) {
    if (nodes_.find(AS) == nodes_.end()) {
        nodes_[AS] = std::make_unique<Node>(AS);
        num_nodes_++;
     }
}

void Graph::add_customer_provider(uint32_t customer, uint32_t provider) {
    add_node(customer);
    add_node(provider);
    nodes_[customer]->add_provider(provider);
    nodes_[provider]->add_customer(customer);
    num_edges_++;
    num_customer_provider_++;
}

void Graph::add_peer(uint32_t peer1, uint32_t peer2) {
    add_node(peer1);
    add_node(peer2);
    nodes_[peer1]->add_peer(peer2);
    nodes_[peer2]->add_peer(peer1);
    num_edges_++;
    num_peers_++;
}
