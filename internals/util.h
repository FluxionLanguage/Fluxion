#ifndef FLUXION_UTIL_H
#define FLUXION_UTIL_H

#include <cstddef>
#include <string>

std::string hashValue(double value);
std::string hashValue(const std::string &value);
/**
 * Check if str equals to one of the chars.
 * @param chars
 * @param str
 * @return
 */
bool contains(const char *chars, const std::string& str);

namespace typing {
    bool isNumber(const char *c_str);
    bool isIdentifier(const char *c_str);
    bool isOperator(const char *c_str);
    bool isWhiteSpace(const char *c_str);
}

#endif //FLUXION_UTIL_H
