#include "util.h"

std::string hashValue(double value) {
    return std::to_string(std::hash<double>{}(value));
}

std::string hashValue(const std::string& value) {
    return std::to_string(std::hash<std::string>{}(value));
}

/**
 * Check if str equals to one of the chars.
 * @param chars
 * @param str
 * @return
 */
bool contains(const char *chars, const std::string& str) {
    char searchFor = *str.c_str();
    for (char* c = (char*) chars; *c != '\0'; c++) {
        if (*c == searchFor) {
            return true;
        }
    }
    return false;
}


namespace typing {
    namespace { // PRIVATE functions of typing.
        /**
         * Check if a given cstring is empty.
         *
         * @param c_str Cstring to check.
         * @return True if empty.
         */
         inline bool isEmpty(const char *c_str) {
            return !*c_str; // \0 automatically becomes false.
         }

        bool isNumber(const char *cstr, bool decimalPart) {
            auto c = cstr;
            if (isEmpty(c)) {
                return false; // If empty or "-".
            }
            if (*c == '-' && !decimalPart) {
                c++; // In case the first part is negative.
                if (isEmpty(c)) {
                    return false; // "-".
                }
            }
            while (!isEmpty(c) && std::isdigit(*c)) {
                c++;
                if (*c == '.' && !decimalPart) { // Since there can't be more than one dot.
                    c++;
                    return isNumber(c, true); // Check if the after dot is numerical, don't allow -.
                }
            }
            return (*c == '\0'); // We have reached to the end of the string.
        }

    };

    bool isNumber(const char *cstr) {
        return isNumber(cstr, false);
    }

    bool isIdentifier(const char *c_str) {
        char *c = const_cast<char *>(c_str);
        if (isEmpty(c) || (std::isdigit(*c))) {
            return false; // Empty.
        }
        while (!isEmpty(c) && std::isalnum(*c)) {
            c++;
        }
        return (*c == '\0');

    }

    bool isOperator(const char *c_str) {
        if (isEmpty(c_str)) {
            return false;
        } else {
            return c_str[1] == '\0' && contains("+-/*^", c_str); // c_str[1] makes sure about the length.
        }
    }

    bool isWhiteSpace(const char *c_str) {
        if (isEmpty(c_str)) {
            return false;
        } else {
            return contains("\n\t ", c_str);
        }
    }
};