// Copyright 2025 Jimmy Padilla (oJimmy05o@gmail.com)
#include "graph.h"

#include <algorithm>
#include <cstdint>
#include <ostream>
#include <queue>
#include <set>

#include "helpers.h"
#include "node.h"

/*
Adds node AS to the graph if it didn't already exist.
*/
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

/*
Propagates all announcements up the ranks.
*/
void Graph::upwards_propagate() {
    for (int rank = 0; rank < static_cast<int>(DAG_.size()); rank++) {
        // Step 1: All nodes at this rank send to their providers
        for (auto& AS : DAG_[rank]) {
            if (!nodes_.at(AS).get_policy().get_RIB().empty()) {
                Node& customer_node = nodes_.at(AS);
                for (auto& [prefix, announcement] : customer_node.get_policy().get_RIB()) {
                    for (auto& provider : customer_node.get_providers()) {
                        nodes_.at(provider).get_policy().add_to_queue(
                            prefix, announcement.next_node(provider, AS, 1));
                    }
                }
            }
        }

        // Step 2: Process the nodes which just recieved announcements from the providers of rank
        // rank.
        if (rank + 1 < static_cast<int>(DAG_.size())) {
            for (auto& AS : DAG_[rank + 1]) {
                nodes_.at(AS).get_policy().queue_to_rib();
            }
        }
    }
}

/*
Propagates announcements down the ranks.
*/
void Graph::downwards_propagate() {
    for (int rank = static_cast<int>(DAG_.size()) - 1; rank >= 0; rank--) {
        // Step 1: All nodes at this rank send to their customers
        for (auto& AS : DAG_[rank]) {
            if (!nodes_.at(AS).get_policy().get_RIB().empty()) {
                Node& provider_node = nodes_.at(AS);
                for (auto& [prefix, announcement] : provider_node.get_policy().get_RIB()) {
                    for (auto& customer : provider_node.get_customers()) {
                        nodes_.at(customer).get_policy().add_to_queue(
                            prefix, announcement.next_node(customer, AS, 3));
                    }
                }
            }
        }

        // Step 2: Process nodes that recieved announcements from the nodes of rank rank.
        if (rank - 1 >= 0) {
            for (auto& AS : DAG_[rank - 1]) {
                nodes_.at(AS).get_policy().queue_to_rib();
            }
        }
    }
}

/*
Propagates announcements across one peer connection.
*/
void Graph::cross_propagate() {
    // enqueue every node with an announcement first
    std::queue<uint32_t> processing_queue;
    for (int rank = 0; rank < static_cast<int>(DAG_.size()); rank++) {
        for (auto& AS : DAG_[rank]) {
            if (!nodes_.at(AS).get_policy().get_RIB().empty()) {
                processing_queue.push(AS);
            }
        }
    }

    std::queue<uint32_t> nodes_to_update;

    // now we process every node with an announcement, and all nodes that are now send an
    // announcement are enqueued onto nodes_to_update.
    while (!processing_queue.empty()) {
        uint32_t as = processing_queue.front();
        processing_queue.pop();
        Node& peer_node = nodes_.at(as);

        for (auto& [prefix, announcement] : peer_node.get_policy().get_RIB()) {
            for (int i = 0; i < static_cast<int>(peer_node.get_peers().size()); i++) {
                auto& peers = peer_node.get_peers();
                nodes_.at(peers[i]).get_policy().add_to_queue(
                    prefix, announcement.next_node(peers[i], as, 2)  // receiver=peer, sender=as
                );
                nodes_to_update.push(peers[i]);
            }
        }
    }

    // finally after sending all announcements update the ribs of all nodes which recieved
    // announcements.
    while (!nodes_to_update.empty()) {
        uint32_t as = nodes_to_update.front();
        nodes_to_update.pop();
        nodes_.at(as).get_policy().queue_to_rib();
    }
}

/*
Adds a customer_provider edge to the graph, creating either the customer or provider nodes if they
didn't already exist.
*/
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

/*
Performs a breadth-first-search through the graph starting at start_vertex.
*/
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
This function also appropriately updates the respective rank values for the Node objects in the
Graph. This function returns true if the graph contains a cycle, otherwise false.
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
        if ((*it).second.get_customers().size() ==
            0) {  // if a node has in_degree 0 this means is is just a customer (lowest level
                  // customer)
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
        return true;  // this means that the graph failed to construct (cycle) and we are done

    // Each node's rank = MAX(all customer ranks) + 1
    // This ensures providers are always at higher ranks than ALL their customers
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto& [asn, node] : nodes_) {
            if (node.get_propagation_rank() == -1) continue;  // Skip unassigned nodes

            // For each provider of this node, ensure provider's rank > this node's rank
            for (auto& provider_asn : node.get_providers()) {
                int required_rank = node.get_propagation_rank() + 1;
                if (nodes_.at(provider_asn).get_propagation_rank() < required_rank) {
                    // Remove from old rank if already placed
                    int old_rank = nodes_.at(provider_asn).get_propagation_rank();
                    if (old_rank != -1) {
                        auto& old_vec = DAG_[old_rank];
                        old_vec.erase(std::remove(old_vec.begin(), old_vec.end(), provider_asn),
                                      old_vec.end());
                    } else {
                        dag_size++;
                    }

                    // Ensure DAG has enough ranks
                    while (static_cast<int>(DAG_.size()) <= required_rank) {
                        DAG_.push_back({});
                    }

                    // Assign to new rank
                    DAG_[required_rank].push_back(provider_asn);
                    nodes_.at(provider_asn).set_propagation_rank(required_rank);
                    changed = true;
                }
            }
        }
    }

    return false;
}

/*
Adds a peer connection to the graph, while also construction any nodes that didn't already exist.
*/
void Graph::add_peer(uint32_t peer1, uint32_t peer2) {
    add_node(peer1);
    add_node(peer2);
    nodes_.at(peer1).add_peer(peer2);
    nodes_.at(peer2).add_peer(peer1);
    num_edges_++;
    num_peers_++;
}

/*
Seeds an announcement in the graph for propagation.
*/
void Graph::seed_announcement(uint32_t AS, const Announcement& ann) {
    nodes_.at(AS).get_policy().add_to_RIB(ann.get_prefix(), ann);
}

/*
Propagates announcements upwards, then cross one gap, then finally downwards.
*/
void Graph::propagate_announcements() {
    int before = 0;
    for (const auto& [asn, node] : nodes_) {
        if (!node.get_policy().get_RIB().empty()) before++;
    }
    std::cout << "Before propagation: " << before << " nodes\n";

    upwards_propagate();

    int after_up = 0;
    for (const auto& [asn, node] : nodes_) {
        if (!node.get_policy().get_RIB().empty()) after_up++;
    }
    std::cout << "After upward: " << after_up << " nodes\n";

    cross_propagate();

    int after_cross = 0;
    for (const auto& [asn, node] : nodes_) {
        if (!node.get_policy().get_RIB().empty()) after_cross++;
    }
    std::cout << "After cross: " << after_cross << " nodes\n";

    downwards_propagate();

    int after_down = 0;
    for (const auto& [asn, node] : nodes_) {
        if (!node.get_policy().get_RIB().empty()) after_down++;
    }
    std::cout << "After downward: " << after_down << " nodes\n";
}

std::ostream& operator<<(std::ostream& os, const Graph& g) {
    g.print(os);
    return os;
}
