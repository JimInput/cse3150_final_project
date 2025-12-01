#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>

#include "announcement.h"

class BGP {
    std::unordered_map<std::string, Announcement> RIB_;
    std::unordered_map<std::string, std::vector<Announcement>> recieved_queue_;
    bool rov_;

   public:
    BGP() : RIB_(), recieved_queue_(), rov_(false) {}

    void add_to_RIB(const std::string& str, const Announcement& ann) {
        RIB_.try_emplace(str, ann);
    }

    void add_to_queue(const std::string& str, Announcement ann) {
        if (!recieved_queue_.try_emplace(str, std::vector<Announcement>{ann}).second)
            recieved_queue_.at(str).push_back(std::move(ann));
    }

    bool is_rov() const {
        return rov_;
    }

    void set_rov(bool rov) {
        rov_ = rov;
    }

    void queue_to_rib();

    bool more_appropriate_announcement(const Announcement& a, const Announcement& b);

    std::unordered_map<std::string, Announcement>& get_RIB() {
        return RIB_;
    }

    const std::unordered_map<std::string, Announcement>& get_RIB() const {
        return RIB_;
    }
};
