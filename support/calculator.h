#pragma once
#include <stdexcept>

class Calculator {
public:
    double add(double a, double b) const {
        return a + b;
    }
    double divide(double a, double b) const {
        if (b == 0) throw std::invalid_argument("Division by zero");
        return a / b;
    }
};
