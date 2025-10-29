#include <iostream>
#include <memory>
#include <utility>

#include "file_reader.h"
#include "graph.h"
#include "node.h"

int main() {
    try {
        // Graph g = FileReader::readGraphFromCSV("data/20250901.as-rel2.txt");
        Graph g = FileReader::readGraphFromCSV("data/test.txt");
        std::cout << g << std::endl;
        // std::cout << g.get_nodes().at(112) << std::endl;
        // std::cout << g.get_nodes().at(2153) << std::endl;
        return 0;
    } catch (std::exception) {
        return 1;
    }
}
