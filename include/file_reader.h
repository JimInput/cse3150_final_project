#pragma once

#include <string>

#include "graph.h"

class FileReader {
   public:
    static Graph readGraphFromCSV(const std::string& path);
};
