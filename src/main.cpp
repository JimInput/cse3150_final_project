#include "node.h"
#include "graph.h"
#include "file_reader.h"
#include <memory>
#include <iostream>
#include <utility>

int main() {
    try {
        Graph g = FileReader::readGraphFromCSV("data/20250901.as-rel2.txt");
        std::cout << g << std::endl;
        return 0;
    } catch (std::exception) {
        return 1;
    }
}
