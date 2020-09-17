#ifndef FLUXION_UTIL_H
#define FLUXION_UTIL_H

#include <cstddef>
#include <string>
#include <vector>

std::string hashValue(double value);
std::string hashValue(const std::string &value);
/**
 * Check if str equals to one of the chars.
 * @param chars
 * @param str
 * @return
 */
bool contains(const char *chars, const std::string& str);
template <typename T>
std::vector<T> reverseVector(std::vector<T> vector) {
    std::vector<T> newVector;
    for (int i = vector.size() - 1; i >= 0; i--) {
        newVector.push_back(vector[i]);
    }
    return newVector;
}
namespace typing {
    bool isNumber(const char *c_str);
    bool isIdentifier(const char *c_str);
    bool isOperator(const char *c_str);
    bool isWhiteSpace(const char *c_str);
    std::string prettyPrintNumber(double number);
}

#endif //FLUXION_UTIL_H
