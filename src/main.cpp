// Copyright 2025 Jimmy Padilla (oJimmy05o@gmail.com)
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "file_reader.h"
#include "graph.h"
#include "announcement.h"
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
        g.seed_announcement(777, Announcement("1.2.0.0/16", 777, 0));
        // print_container(std::cout, g.get_nodes().at(1).get_policy().get_RIB().at(std::string("1.2.0.0/16")).get_AS_path());
        g.propagate_announcements();
        print_container(std::cout, g.get_nodes().at(4).get_policy().get_RIB().at("1.2.0.0/16").get_AS_path());


        

        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
