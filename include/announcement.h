#pragma once

#include <string>
#include <cstdint>

class Announcement {
    std::string prefix_; // use a prefix_id here (has the prefixes we need)
    std::vector<uint32_t> AS_path_; // instead use arena path (max as path is around 16)
    // uint32_t start_index;
    // uint32_t end_index;
    // have some other vector std::vector<int> arena_as_path; (all paths can share this arena!!)
    uint32_t next_hop_AS_;
    short recieved_from_relationship_;
public:
    Announcement(std::string prefix, uint32_t curr_AS, short relation): 
                 prefix_(prefix), AS_path_({curr_AS}), 
                 next_hop_AS_(curr_AS), recieved_from_relationship_(relation) {}

    Announcement(std::string prefix, std::vector<uint32_t> AS_path, uint32_t curr_AS, short relation):
                prefix_(prefix), AS_path_(AS_path), next_hop_AS_(curr_AS), recieved_from_relationship_(relation) {}

    const std::string& get_prefix() const {
        return prefix_;
    }

    std::vector<uint32_t>& get_AS_path() {
        return AS_path_;
    }

    uint32_t get_next_hop_AS() const {
        return next_hop_AS_;
    }

    short get_relation() const {
        return recieved_from_relationship_;
    }
    
    // 0 = origin , 1 = customer
    Announcement next_node(uint32_t as, short relation) const {
        std::vector<uint32_t> new_AS_path_(AS_path_);
        new_AS_path_.push_back(as);
        return Announcement(prefix_, new_AS_path_, as, relation);
    }

};
