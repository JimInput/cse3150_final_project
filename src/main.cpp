// Copyright 2025 Jimmy Padilla (oJimmy05o@gmail.com)
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "file_reader.h"
#include "graph.h"
#include "helpers.h"
#include "node.h"
#include "bgp.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: ./program.out [FILE_PATH]";
        return -1;
    }

    try {
        // Graph g = FileReader::readGraphFromCSV("data/20250901.as-rel2.txt");
        Graph g = FileReader::readGraphFromCSV(argv[1]);
        std::cout << "successfully built graph" << std::endl;
        BGP policy;

        int rank = 0;

        for (auto& vec : g.get_DAG()) {
            std::cout << rank << " " << vec.size() << std::endl;
            print_container(std::cout, vec);
            rank++;
        }

        std::cout << g.get_nodes().at(17456).get_customers().size() << std::endl;

        // std::cout << g << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
