// Copyright 2025 Jimmy Padilla (oJimmy05o@gmail.com)
#include "announcement.h"

#include <cstdint>

Announcement Announcement::next_node(uint32_t reciever_as, uint32_t sender_as,
                                     short relation) const {
    std::vector<uint32_t> new_AS_path_(AS_path_);
    new_AS_path_.push_back(reciever_as);  // Add receiver to path
    return Announcement(prefix_, new_AS_path_, sender_as, relation,
                        rov_invalid_);  // Set next_hop to sender
}