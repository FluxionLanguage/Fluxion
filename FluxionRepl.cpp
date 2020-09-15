#include <string>
#include <iostream>
#include "fluxion.h"

int main() {
    std::string line;
    do {
        std::cout << "?: ";
        getline(std::cin, line);
        fluxion::interpret(line.c_str());
    } while (line != "exit");
}