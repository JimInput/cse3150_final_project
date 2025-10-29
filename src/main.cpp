// Copyright 2025 Jimmy Padilla (oJimmy05o@gmail.com)
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "file_reader.h"
#include "graph.h"
#include "helpers.h"
#include "node.h"

int main() {
    try {
        Graph g = FileReader::readGraphFromCSV("data/20250901.as-rel2.txt");
        // Graph g = FileReader::readGraphFromCSV("tests/test2.txt");
        std::cout << g << std::endl;
        std::vector<uint32_t> path = g.bfs(42);
        print_container(std::cout, path);
        std::cout << g.has_cycle() << std::endl;
        // std::cout << g.get_nodes().at(112) << std::endl;
        // std::cout << g.get_nodes().at(2153) << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
