#include <iostream>
#include "debug.h"

void debug::printToken(Token *token) {
    char ops[] = {'+', '-', '/', '*', '^', '!'};
    switch (token->getTokenType()) {
        case TOKEN_CONSTANT:
            std::cout << "CONSTANT (" << dynamic_cast<ConstantToken*>(token)->getValue() << ")\n";
            break;
        case TOKEN_VARIABLE:
            std::cout << "VARIABLE (" << dynamic_cast<VariableToken*>(token)->getName() << ")\n";
            break;
        case TOKEN_OPERATOR:
            std::cout << "OPERATOR (" << ops[dynamic_cast<OperatorToken*>(token)->getOperationType()] << ")\n";
            break;
        case TOKEN_LEFT_PAREN:
            std::cout << "LEFT PAREN\n";
            break;
        case TOKEN_RIGHT_PAREN:
            std::cout << "RIGHT PAREN\n";
            break;
        case TOKEN_UNDEFINED:
            std::cout << "UNDEFINED!\n";
            break;
    }
}

void debug::printTokens(std::vector<Token*> tokens) {
    std::cout << "DEBUG\nINDEX | LOCATION |   TYPE   |" << "\n";
    for (auto token : tokens) {
        std::cout << token->getIndex() << "\t" << token->getLocation() << "\t";
        printToken(token);
    }
}