#include "util.h"

std::string hashValue(double value) {
    return std::to_string(std::hash<double>{}(value));
}

std::string hashValue(const std::string& value) {
    return std::to_string(std::hash<std::string>{}(value));
}
