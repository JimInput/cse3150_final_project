// Copyright 2025 Jimmy Padilla (oJimmy05o@gmail.com)
#include "bgp.h"

/*
Iterates through all recieved announcements and updates rib with the best announcements for each
prefix.
*/
void BGP::queue_to_rib() {
    for (auto& [prefix, announcement_list] : recieved_queue_) {
        int min_idx = -1;  // Start with no valid announcement
        // Find the best valid announcement
        for (int i = 0; i < static_cast<int>(announcement_list.size()); ++i) {
            if (rov_ && announcement_list[i].get_rov_invalid())
                continue;  // DO NOT ADD ANNOUNCEMENT IF WE ARE ROV AND IT IS ROV INVALID.
            if (min_idx == -1 ||
                more_appropriate_announcement(announcement_list[i], announcement_list[min_idx])) {
                min_idx = i;
            }
        }

        // Only store if we found a valid announcement
        if (min_idx != -1) {
            if (RIB_.find(prefix) == RIB_.end() ||
                more_appropriate_announcement(announcement_list[min_idx], RIB_.at(prefix)))
                RIB_[prefix] = announcement_list[min_idx];
        }
    }
    recieved_queue_.clear();
}

/*
Computes which announcement is better starting with relationship, then moving to length of path, and
finally which has the lower next_hop_AS.
*/
bool BGP::more_appropriate_announcement(const Announcement& a, const Announcement& b) {
    if (a.get_relation() < b.get_relation()) return true;
    if (b.get_relation() < a.get_relation()) return false;
    if (a.get_AS_path().size() < b.get_AS_path().size()) return true;
    if (b.get_AS_path().size() < a.get_AS_path().size()) return false;
    if (a.get_next_hop_AS() < b.get_next_hop_AS()) return true;
    return false;
}