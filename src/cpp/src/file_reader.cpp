// Copyright 2025 Jimmy Padilla (oJimmy05o@gmail.com)
#include "file_reader.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "graph.h"

Graph FileReader::readGraphFromCSV(const std::string& path) {
    Graph g;
    std::ifstream file(path);
    if (!file.is_open()) throw std::runtime_error("Failed to open input graph: " + path);

    try {
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            if (line[0] == '#') continue;

            int64_t outputs[3];

            for (int i = 0; i < 3; i++) {
                size_t barPos = line.find('|');
                if (barPos == std::string::npos) throw std::runtime_error("Malformed CSV input");
                outputs[i] = std::stol(line.substr(0, barPos));
                line = line.substr(barPos + 1);
            }

            switch (outputs[2]) {
                case 0:
                    g.add_peer(static_cast<uint32_t>(outputs[0]),
                               static_cast<uint32_t>(outputs[1]));
                    break;
                case -1:
                    g.add_customer_provider(static_cast<uint32_t>(outputs[1]),
                                            static_cast<uint32_t>(outputs[0]));
                    break;
                default:
                    break;
            }
        }
    } catch (std::runtime_error&) {
        throw std::runtime_error("Malformed CSV input");
    } catch (std::invalid_argument&) {
        throw std::invalid_argument("Invalid CSV input");
    }
    if (g.get_num_nodes() == 0) throw std::runtime_error("Empty or invalid CSV graph");
    if (g.construct_dag()) throw std::runtime_error("Invalid CSV input (has_cycle)");

    std::cout << "successfully built graph" << std::endl;

    return g;
}

void FileReader::updateROV(Graph& g, const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) throw std::runtime_error("Failed to open input rov file: " + path);

    try {
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            uint32_t val = static_cast<uint32_t>(std::stoi(line));
            g.update_rov(val, true);
        }
    } catch (std::runtime_error&) {
        throw std::runtime_error("Malformed CSV input");
    } catch (std::invalid_argument&) {
        throw std::invalid_argument("Invalid CSV input");
    }

    std::cout << "successfully updated ROV" << std::endl;
}
void FileReader::seedAnnouncements(Graph& g, const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Failed to open input announcements file: " + path);

    try {
        std::string line;
        bool run1 = true;
        while (std::getline(file, line)) {
            if (run1) {
                run1 = false;
                continue;
            }

            uint32_t asn;
            std::string prefix;
            bool invalid;

            size_t pos[2];

            pos[0] = line.find(',');
            if (pos[0] == std::string::npos) throw std::runtime_error("Malformed CSV input");
            pos[1] = line.find(',', pos[0] + 1);
            if (pos[1] == std::string::npos) throw std::runtime_error("Malformed CSV input");

            asn = std::stoul(line.substr(0, pos[0]));
            prefix = line.substr(pos[0] + 1, pos[1] - (pos[0] + 1));
            std::string value = line.substr(pos[1] + 1);
            while (!value.empty() && (value.back() == '\r' || value.back() == '\n' ||
                                      value.back() == ' ' || value.back() == '\t')) {
                value.pop_back();
            }
            invalid = (value == "True");

            g.seed_announcement(asn, Announcement(prefix, asn, 0, invalid));
        }
    } catch (std::runtime_error&) {
        throw std::runtime_error("Malformed CSV input");
    } catch (std::invalid_argument&) {
        throw std::invalid_argument("Invalid CSV input");
    }

    std::cout << "successfully seeded announcements" << std::endl;
};
