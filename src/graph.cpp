// Copyright 2025 Jimmy Padilla (oJimmy05o@gmail.com)
#include "graph.h"

#include <cstdint>
#include <ostream>
#include <queue>
#include <set>

#include "node.h"

void Graph::add_node(uint32_t AS) {
    if (nodes_.find(AS) == nodes_.end()) {
        auto [it, inserted] = nodes_.try_emplace(AS, AS);
        if (inserted) ++num_nodes_;
    }
}

void Graph::print(std::ostream& os) const {
    os << "Graph():\n\tnum_nodes_=" << num_nodes_ << "\n\tnum_edges_=" << num_edges_
       << "\n\tnum_customer_provider=" << num_customer_provider_ << "\n\tnum_peers_=" << num_peers_
       << "\n"
       << "{\n";
    int node_count = 0;
    for (const auto& pair : nodes_) {
        if (node_count > 10) break;
        os << pair.second << std::endl;
        node_count++;
    }
    if (node_count > 10) os << "..." << std::endl;
    os << "}";
}

void Graph::add_customer_provider(uint32_t customer, uint32_t provider) {
    add_node(customer);
    add_node(provider);
    nodes_.at(customer).add_provider(provider);
    nodes_.at(provider).add_customer(customer);
    num_edges_++;
    num_customer_provider_++;

    if (nodes_.at(provider).get_customers().size() > largest_provider_.second) {
        largest_provider_.first = provider;
        largest_provider_.second = nodes_.at(provider).get_customers().size();
    }
}

const std::vector<uint32_t> Graph::bfs(uint32_t start_vertex) const {
    std::set<uint32_t> visited;
    std::queue<uint32_t> q;
    std::vector<uint32_t> path;

    // Start at some arbitrary vertex.
    visited.insert(start_vertex);
    path.push_back(start_vertex);
    q.push(start_vertex);

    while (!q.empty()) {
        uint32_t current_vertex = q.front();
        q.pop();

        for (uint32_t neighbor : nodes_.at(current_vertex).get_customers()) {
            if (visited.find(neighbor) == visited.end()) {
                path.push_back(neighbor);
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }
    return path;
}

const bool Graph::has_cycle() const {
    std::queue<uint32_t> q;
    std::unordered_map<uint32_t, uint32_t> in_degree;

    uint32_t visited = 0;

    // 1) start by defining the in_degree for each node (looking for "pure providers")
    for (auto it = nodes_.begin(); it != nodes_.end(); it++) {
        in_degree.try_emplace((*it).first, (*it).second.get_providers().size());
        if ((*it).second.get_providers().size() == 0) {  // if a node has in_degree 0 {
            q.push((*it).first);
        }
    }

    // 2) Kahn's Algorithm: topological bfs, updating the in_degree for each node.
    // when a node reaches in_degree = 0, treat it as the next node to layer.
    while (!q.empty()) {
        uint32_t u = q.front();
        q.pop();
        visited++;

        for (auto& v : nodes_.at(u).get_customers()) {
            in_degree.at(v)--;
            if (in_degree.at(v) == 0) {
                q.push(v);
            }
        }
    }

    return visited != num_nodes_;
}

void Graph::add_peer(uint32_t peer1, uint32_t peer2) {
    add_node(peer1);
    add_node(peer2);
    nodes_.at(peer1).add_peer(peer2);
    nodes_.at(peer2).add_peer(peer1);
    num_edges_++;
    num_peers_++;
}

std::ostream& operator<<(std::ostream& os, const Graph& g) {
    g.print(os);
    return os;
}
