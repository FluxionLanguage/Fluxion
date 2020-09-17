#include <string>
#include <iostream>
#include "fluxion.h"

int main() {
    std::string line;
    do {
        std::cout << "?: ";
        getline(std::cin, line);
        std::cout << fluxion::interpret(line.c_str()) << std::endl;
    } while (line != "exit");
}