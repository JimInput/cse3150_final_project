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

    void queue_to_rib(uint32_t as, short relation) {
        for (auto& [prefix, announcement_list] : recieved_queue_) {
            short min_relation = announcement_list[0].get_relation();
            int min_length_path = announcement_list[0].get_AS_path().size();
            uint32_t min_next_hop_AS = announcement_list[0].get_next_hop_AS();
            int min_idx = 0;
            for (int i = 1; i < static_cast<int>(announcement_list.size()); ++i) {
                if (rov_ && announcement_list[i].get_rov_invalid()) continue;
                if (announcement_list[i].get_relation() > min_relation) continue;
                if (announcement_list[i].get_relation() < min_relation) {
                    min_relation = announcement_list[i].get_relation();
                    min_length_path = announcement_list[i].get_AS_path().size();
                    min_next_hop_AS = announcement_list[i].get_next_hop_AS();
                    min_idx = i;
                }
                if (static_cast<int>(announcement_list[i].get_AS_path().size()) > min_length_path) continue;
                if (static_cast<int>(announcement_list[i].get_AS_path().size()) < min_length_path) {
                    min_relation = announcement_list[i].get_relation();
                    min_length_path = announcement_list[i].get_AS_path().size();
                    min_next_hop_AS = announcement_list[i].get_next_hop_AS();
                    min_idx = i;
                }
                if (announcement_list[i].get_next_hop_AS() > min_next_hop_AS) continue;
                if (announcement_list[i].get_next_hop_AS() < min_next_hop_AS) {
                    min_relation = announcement_list[i].get_relation();
                    min_length_path = announcement_list[i].get_AS_path().size();
                    min_next_hop_AS = announcement_list[i].get_next_hop_AS();
                    min_idx = i;
                }
            }
            Announcement original_announcement = announcement_list[min_idx];
            if (!(original_announcement.get_rov_invalid() && rov_)) {
                Announcement updated_announcement = original_announcement.next_node(as, relation, original_announcement.get_rov_invalid());
                add_to_RIB(prefix, updated_announcement);
            }
        }
        recieved_queue_.clear();
    }

    std::unordered_map<std::string, Announcement>& get_RIB() {
        return RIB_;
    }

    const std::unordered_map<std::string, Announcement>& get_RIB() const {
        return RIB_;
    }
};
