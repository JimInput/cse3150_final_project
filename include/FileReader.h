#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include "Deck.h"

class FileReader {
public:
    static Deck readDeckFromCSV(const std::string& path);
};

#endif
