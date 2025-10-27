#pragma once

#include <vector>
#include <cstdint>
#include <ostream>

class Node {
    uint32_t AS_;
    std::vector<uint32_t> providers_;
    std::vector<uint32_t> customers_;
    std::vector<uint32_t> peers_;

    void print(std::ostream& os) const;

public:
    Node(uint32_t AS): AS_(AS), providers_(), customers_(), peers_() {}

    void add_provider(uint32_t AS);
    void add_customer(uint32_t AS);
    void add_peer(uint32_t AS);

    uint32_t get_AS() {return AS_;}
    const std::vector<uint32_t>& get_providers() const {return providers_;}
    const std::vector<uint32_t>& get_customers() const {return customers_;}
    const std::vector<uint32_t>& get_peers() const {return peers_;}

    friend std::ostream& operator<<(std::ostream& os, const Node n);
};
