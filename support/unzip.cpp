#include <cstdlib>
#include <iostream>

int main() {
    int result = std::system("bunzip2 -f 20250901.as-rel2.txt.bz2"); // calls a command on command line
    if (result != 0) {
        std::cerr << "Failed to decompress!" << std::endl;
        return 1;
    }

    std::cout << "working" << std::endl;
    return 0;
}
