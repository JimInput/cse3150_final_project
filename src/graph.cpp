#include "graph.h"

#include <cstdint>
#include <ostream>

void Graph::add_node(uint32_t AS) {
    if (nodes_.find(AS) == nodes_.end()) {
        nodes_[AS] = std::make_unique<Node>(AS);
        num_nodes_++;
     }
}

void Graph::print(std::ostream& os) const {
    os << "Graph():\n\tnum_nodes_=" << num_nodes_ << "\n\tnum_edges_=" << num_edges_
       << "\n\tnum_customer_provider=" << num_customer_provider_ << "\n\tnum_peers_=" << num_peers_ << "\n"
       << "{\n";
    int node_count = 0;
    for (auto& pair : nodes_) {
        os << *pair.second << std::endl;
    }
    os << "}";
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

std::ostream& operator<<(std::ostream& os, const Graph& g) {
    g.print(os);
    return os;
}
