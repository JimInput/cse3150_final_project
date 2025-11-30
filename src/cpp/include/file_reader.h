// Copyright 2025 Jimmy Padilla (oJimmy05o@gmail.com)
#pragma once

#include <string>

#include "graph.h"

class FileReader {
   public:
    static Graph readGraphFromCSV(const std::string& path);
    static void updateROV(Graph& g, const std::string& path);
    static void seedAnnouncements(Graph& g, const std::string& path);
};
