// Copyright 2025 Jimmy Padilla (oJimmy05o@gmail.com)
#include "file_writer.h"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "graph.h"
#include "helpers.h"

void FileWriter::writeRibs(const Graph& g, const std::string& path) {
    std::ofstream outputFile(path, std::ios::out | std::ios::trunc);

    // Check if the file was opened successfully
    if (!outputFile.is_open()) throw std::runtime_error("Failed to open output file: " + path);

    // Write header row
    outputFile << "asn,prefix,as_path" << std::endl;

    // 1. Extract keys into a vector
    std::vector<uint32_t> keys;
    for (const auto& pair : g.get_nodes()) {
        keys.push_back(pair.first);
    }

    // 2. Sort the vector of keys
    std::sort(keys.begin(), keys.end());

    // 3. Iterate through the sorted keys and access map elements
    for (const uint32_t& key : keys) {
        std::vector<std::string> prefixes;
        const std::unordered_map<std::string, Announcement>& rib_map =
            g.get_nodes().at(key).get_policy().get_RIB();

        for (const auto& pair : rib_map) {
            prefixes.push_back(pair.first);
        }
        std::sort(prefixes.begin(), prefixes.end());
        for (const std::string& prefix : prefixes) {
            outputFile << key << "," << prefix << ",";
            print_reverse_container(outputFile, rib_map.at(prefix).get_AS_path());
            outputFile << '\n';
        }
    }

    // Close the file stream
    outputFile.close();

    std::cout << "Data written to " + path + " successfully." << std::endl;
}
