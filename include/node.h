// Copyright 2025 Jimmy Padilla (oJimmy05o@gmail.com)
#pragma once

#include <cstdint>
#include <ostream>
#include <vector>
#include "bgp.h"

class Node {
    uint32_t AS_;
    std::vector<uint32_t> providers_;
    std::vector<uint32_t> customers_;
    std::vector<uint32_t> peers_;
    int propagation_rank_; // this is defined in the construct dag function called in the file_reader.cpp
    BGP policy_;

    void print(std::ostream& os) const;

   public:
    Node(uint32_t AS) : AS_(AS), providers_(), customers_(), peers_(), propagation_rank_(-1), policy_() {}

    void add_provider(uint32_t AS);
    void add_customer(uint32_t AS);
    void add_peer(uint32_t AS);

    uint32_t get_AS() {
        return AS_;
    }
    const std::vector<uint32_t>& get_providers() const {
        return providers_;
    }
    const std::vector<uint32_t>& get_customers() const {
        return customers_;
    }
    const std::vector<uint32_t>& get_peers() const {
        return peers_;
    }

    void set_propagation_rank(int rank) {
        propagation_rank_ = rank;
    }

    int get_propagation_rank() const {
        return propagation_rank_;
    }

    BGP& get_policy() {
        return policy_;
    }

    friend std::ostream& operator<<(std::ostream& os, const Node n);
};
