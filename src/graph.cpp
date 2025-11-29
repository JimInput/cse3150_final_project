// Copyright 2025 Jimmy Padilla (oJimmy05o@gmail.com)
#include "graph.h"

#include <cstdint>
#include <ostream>
#include <queue>
#include <set>
#include "helpers.h"

#include "node.h"

void Graph::add_node(uint32_t AS) {
    auto [it, inserted] = nodes_.try_emplace(AS, AS);
    if (inserted) ++num_nodes_;
}

void Graph::print(std::ostream& os) const {
    os << "Graph():\n\tnum_nodes_=" << num_nodes_ << "\n\tnum_edges_=" << num_edges_
       << "\n\tnum_customer_provider=" << num_customer_provider_ << "\n\tnum_peers_=" << num_peers_
       << "\n";
    os << "largest_provider=" << nodes_.at(largest_provider_.first) << '\n';
    os << "largest_customer=" << nodes_.at(largest_customer_.first) << '\n';
        
}

void Graph::upwards_propagate() {
    // start by iterating through rank 0 nodes and enqueue them for processing
    std::queue<uint32_t> processing_queue;
    for (int rank = 0; rank < DAG_.size(); rank++) {
        for (auto& AS : DAG_[rank]) {
            if (!nodes_.at(AS).get_policy().get_RIB().empty()) {
                processing_queue.push(AS);
            }
        }

        std::queue<uint32_t> nodes_to_update;
        while (!processing_queue.empty()) {
            uint32_t customer_AS = processing_queue.front();
            processing_queue.pop();
            Node& customer_node = nodes_.at(customer_AS);

            for (auto& [prefix, announcement] : customer_node.get_policy().get_RIB()) {
                for (int i = 0; i < customer_node.get_providers().size(); i++) {
                    auto& providers = customer_node.get_providers();
                    nodes_.at(providers[i]).get_policy().add_to_queue(prefix, announcement);
                    nodes_to_update.push(providers[i]);
                }
            }
        }

        while(!nodes_to_update.empty()) {
            uint32_t as = nodes_to_update.front();
            nodes_to_update.pop();
            nodes_.at(as).get_policy().queue_to_rib(as, 1);
        }


    }
    

    
}

void Graph::downwards_propagate() {}

void Graph::cross_propagate() {}

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

    if (nodes_.at(customer).get_providers().size() > largest_customer_.second) {
        largest_customer_.first = customer;
        largest_customer_.second = nodes_.at(customer).get_customers().size();
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
/*
This function constructs the vector of vectors for the DAG object in the graph. 
This function also appropriately updates the respective rank values for the Node objects in the Graph.
This function returns true if the graph contains a cycle, otherwise false.
*/
bool Graph::construct_dag() {
    std::queue<uint32_t> q;
    std::unordered_map<uint32_t, uint32_t> in_degree;

    uint32_t visited = 0;

    uint32_t dag_size = 0;
    DAG_.push_back({});

    // 1) start by defining the in_degree for each node (looking for "pure customers")
    for (auto it = nodes_.begin(); it != nodes_.end(); it++) {
        in_degree.try_emplace((*it).first, (*it).second.get_customers().size());
        if ((*it).second.get_customers().size() == 0) {  // if a node has in_degree 0 this means is is just a customer (lowest level customer)
            q.push((*it).first);
            // if this node has no customers they belong in the first layer of the DAG
            (*it).second.set_propagation_rank(0);
            DAG_[0].push_back((*it).first);
            dag_size++;
        }
    }

    // 2) Kahn's Algorithm: topological bfs, updating the in_degree for each node.
    // when a node reaches in_degree = 0, treat it as the next node to layer.
    while (!q.empty()) {
        uint32_t u = q.front();
        q.pop();
        visited++;

        for (auto& v : nodes_.at(u).get_providers()) {
            in_degree.at(v)--;
            if (in_degree.at(v) == 0) {
                q.push(v);
            }
        }
    }

    if (visited != num_nodes_)
        return true; // this means that the graph failed to construct (cycle) and we are done

    //otherwise we should construct the dag (iterate through each node of rank 0 (pure providers) and obtain next rank, etc., etc.)
    
    int current_order = 0;
    while (dag_size != num_nodes_) {
        DAG_.push_back({}); // add a new layer
        for (auto& as : DAG_[current_order]) {
            for (auto& next_as : nodes_.at(as).get_providers()) {
                if (nodes_.at(next_as).get_propagation_rank() != -1) continue;
                DAG_[current_order + 1].push_back(next_as);
                nodes_.at(next_as).set_propagation_rank(current_order+1);
                dag_size++; 
            }
        }
        current_order++;
    }

    return false;
}

void Graph::add_peer(uint32_t peer1, uint32_t peer2) {
    add_node(peer1);
    add_node(peer2);
    nodes_.at(peer1).add_peer(peer2);
    nodes_.at(peer2).add_peer(peer1);
    num_edges_++;
    num_peers_++;
}

void Graph::seed_announcement(uint32_t AS, const Announcement& ann) {
    nodes_.at(AS).get_policy().add_to_RIB(ann.get_prefix(), ann);
}

std::ostream& operator<<(std::ostream& os, const Graph& g) {
    g.print(os);
    return os;
}
