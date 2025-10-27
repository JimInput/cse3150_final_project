#pragma once

#include "node.h"
#include <map>
#include <memory>
#include <cstdint>

class Graph {
    std::map<uint32_t, std::unique_ptr<Node>> nodes_;
    uint32_t num_nodes_;
    uint32_t num_customer_provider_;
    uint32_t num_peers_;
    uint32_t num_edges_;

    void add_node(uint32_t AS);

public:
    Graph(): nodes_(), num_nodes_(0), num_customer_provider_(0), num_peers_(0), num_edges_(0) {}

    void add_customer_provider(uint32_t customer, uint32_t provider);
    void add_peer(uint32_t peer1, uint32_t peer2);

    const uint32_t get_num_edges() const {return num_edges_;}
    const uint32_t get_num_nodes() const {return num_nodes_;}
    const uint32_t get_num_customer_provider_() const {return num_customer_provider_;}
    const uint32_t get_num_peers_() const {return num_peers_;}
    const std::map<uint32_t, std::unique_ptr<Node>>& get_nodes() const {return nodes_;}
};
