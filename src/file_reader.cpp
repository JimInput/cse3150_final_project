#include "file_reader.h"
#include "graph.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <fstream>

Graph FileReader::readGraphFromCSV(const std::string& path) {
    Graph g;
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Failed to open input graph: " + path);

    try {
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            if (line[0] == '#') continue;

            int64_t outputs[3];

            for (int i = 0; i < 3; i++) {
                size_t barPos = line.find('|');
                if (barPos == std::string::npos)
                    throw std::runtime_error("Malformed CSV input");
                outputs[i] = std::stol(line.substr(0, barPos));
                line = line.substr(barPos+1);
            }

            switch (outputs[2]) {
                case 0:
                g.add_peer(static_cast<uint32_t>(outputs[0]), static_cast<uint32_t>(outputs[1]));
                break;
                case -1:
                g.add_customer_provider(static_cast<uint32_t>(outputs[1]), static_cast<uint32_t>(outputs[0]));
                break;
                case 1:
                g.add_customer_provider(static_cast<uint32_t>(outputs[0]), static_cast<uint32_t>(outputs[1]));
                break;
                default:
                break;
            }

        }
    } catch (std::runtime_error) {
        throw std::runtime_error("Malformed CSV input");
    } catch (std::invalid_argument) {
        throw std::invalid_argument("Invalid CSV input");
    }

    if (g.get_num_nodes() == 0)
        throw std::runtime_error("Empty or invalid CSV graph");
    
    return g;
};

