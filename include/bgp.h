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

    void queue_to_rib() {
        for (auto& [prefix, announcement_list] : recieved_queue_) {
            int min_idx = -1;  // Start with no valid announcement
            // Find the best valid announcement
            for (int i = 0; i < static_cast<int>(announcement_list.size()); ++i) {
                if (rov_ && announcement_list[i].get_rov_invalid()) continue;
                if (min_idx == -1 || more_appropriate_announcement(announcement_list[i], announcement_list[min_idx])) {
                    min_idx = i;
                }
            }

            // Only store if we found a valid announcement
            if (min_idx != -1) {
                if (RIB_.find(prefix) == RIB_.end() || more_appropriate_announcement(announcement_list[min_idx], RIB_.at(prefix)))
                    RIB_[prefix] = announcement_list[min_idx];
            }
        }
        recieved_queue_.clear();
    }

    /* ORIGINAL queue_to_rib (no debug):
    void queue_to_rib() {
        for (auto& [prefix, announcement_list] : recieved_queue_) {
            int min_idx = 0;
            for (int i = 1; i < static_cast<int>(announcement_list.size()); ++i) {
                if (rov_ && announcement_list[i].get_rov_invalid()) continue;
                if (more_appropriate_announcement(announcement_list[i], announcement_list[min_idx]))
                    min_idx = i;
            }
            if (!(announcement_list[min_idx].get_rov_invalid() && rov_)) {
                if (RIB_.find(prefix) == RIB_.end() || more_appropriate_announcement(announcement_list[min_idx], RIB_.at(prefix)))
                    RIB_[prefix] = announcement_list[min_idx];
            }
        }
        recieved_queue_.clear();
    }
    */

    bool more_appropriate_announcement(const Announcement& a, const Announcement& b) {
        if (a.get_relation() < b.get_relation())
            return true;
        if (b.get_relation() < a.get_relation())
            return false;
        if (a.get_AS_path().size() < b.get_AS_path().size())
            return true;
        if (b.get_AS_path().size() < a.get_AS_path().size())
            return false;
        if (a.get_next_hop_AS() < b.get_next_hop_AS())
            return true;
        return false;
    }

    std::unordered_map<std::string, Announcement>& get_RIB() {
        return RIB_;
    }

    const std::unordered_map<std::string, Announcement>& get_RIB() const {
        return RIB_;
    }
};
