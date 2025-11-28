#pragma once

#include "announcement.h"

#include <string>
#include <cstdint>
#include <unordered_map>

class BGP {
    std::unordered_map<std::string, Announcement> RIB_;
    std::unordered_map<std::string, std::vector<Announcement>> recieved_queue_;
public:
    BGP(): RIB_(), recieved_queue_() {}
};
