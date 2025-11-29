// Copyright 2025 Jimmy Padilla (oJimmy05o@gmail.com)
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "announcement.h"
#include "bgp.h"
#include "file_reader.h"
#include "graph.h"
#include "helpers.h"
#include "node.h"
#include "file_writer.h"

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cout << "Usage: ./program.out [GRAPH_FILE_PATH] [ANNOUNCEMENT_FILE_PATH] [ROV_AS_PATH] [RIBS_OUT_PATH]\n";
        return -1;
    }

    try {
        Graph g = FileReader::readGraphFromCSV(argv[1]);
        FileReader::updateROV(g, argv[3]);
        FileReader::seedAnnouncements(g, argv[2]);
        g.propagate_announcements();
        FileWriter::writeRibs(g, argv[4]);

        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
