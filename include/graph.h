// Copyright 2025 Jimmy Padilla (oJimmy05o@gmail.com)
#pragma once

#include <cstdint>
#include <memory>
#include <ostream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "node.h"

class Graph {
    std::unordered_map<uint32_t, Node> nodes_;
    uint32_t num_nodes_;
    uint32_t num_customer_provider_;
    uint32_t num_peers_;
    uint32_t num_edges_;
    std::pair<uint32_t, uint32_t> largest_provider_;
    std::pair<uint32_t, uint32_t> largest_customer_;
    std::vector<std::vector<uint32_t>> DAG_;

    void add_node(uint32_t AS);
    void print(std::ostream& os) const;
    void upwards_propagate();
    void downwards_propagate();
    void cross_propagate();

   public:
    Graph()
        : num_nodes_(0),
          num_customer_provider_(0),
          num_peers_(0),
          num_edges_(0),
          largest_provider_{0, 0},
          largest_customer_{0, 0} {}

    void add_customer_provider(uint32_t customer, uint32_t provider);
    void add_peer(uint32_t peer1, uint32_t peer2);

    uint32_t get_num_edges() const {
        return num_edges_;
    }
    uint32_t get_num_nodes() const {
        return num_nodes_;
    }
    uint32_t get_num_customer_provider() const {
        return num_customer_provider_;
    }
    uint32_t get_num_peers() const {
        return num_peers_;
    }
    const std::pair<uint32_t, uint32_t> get_largest_provider() const {
        return largest_provider_;
    }
    const std::pair<uint32_t, uint32_t> get_largest_customer() const {
        return largest_customer_;
    }
    std::unordered_map<uint32_t, Node>& get_nodes() {
        return nodes_;
    }

    const std::unordered_map<uint32_t, Node>& get_nodes() const {
        return nodes_;
    }

    const std::vector<std::vector<uint32_t>>& get_DAG() const {
        return DAG_;
    }

    const std::vector<uint32_t> bfs(uint32_t start_vertex) const;

    bool construct_dag();

    void seed_announcement(uint32_t AS, const Announcement& ann);

    void propagate_announcements() {
        upwards_propagate();
        cross_propagate();
        downwards_propagate();

        std::cout << "announcements have fully propagated" << std::endl;
    }

    void update_rov(uint32_t AS, bool rov) {
        nodes_.at(AS).get_policy().set_rov(rov);
    }

    friend std::ostream& operator<<(std::ostream& os, const Graph& g);
};
