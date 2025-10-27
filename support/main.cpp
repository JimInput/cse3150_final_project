#include "calculator.h"
#include <iostream>

int main() {
    Calculator c;
    std::cout << "2 + 3 = " << c.add(2, 3) << std::endl;
    std::cout << "10 / 2 = " << c.divide(10, 2) << std::endl;

    try {
        std::cout << "10 / 0 = " << c.divide(10, 0) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
    }

    return 0;
}
