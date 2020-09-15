#ifndef FLUXION_DEBUG_H
#define FLUXION_DEBUG_H

#include <vector>
#include "Parser.h"

namespace debug {
    void printToken(Token *token);
    void printTokens(std::vector<Token*> tokens);
}

#endif //FLUXION_DEBUG_H
