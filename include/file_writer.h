// Copyright 2025 Jimmy Padilla (oJimmy05o@gmail.com)
#pragma once

#include <string>

#include "graph.h"

class FileWriter {
   public:
    static void writeRibs(const Graph& g, const std::string& path);
};
