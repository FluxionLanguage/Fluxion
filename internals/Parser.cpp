#include "Parser.h"

TokenType Token::getTokenType() const {
    return this->type;
}

int Token::getLocation() const {
    return this->location;
}

int Token::getIndex() const {
    return this->index;
}

Token::Token(TokenType type, int location, int index) : type(type), location(location), index(index) {

}


ConstantToken::ConstantToken(int location, int index, double value) : value(value), Token(TOKEN_CONSTANT, location, index) {

}

double ConstantToken::getValue() const {
    return this->value;
}


const char *VariableToken::getName() const {
    return this->name;
}

VariableToken::VariableToken(int location, int index, const char *name) : name(name), Token(TOKEN_VARIABLE, location, index) {
}


OperationType OperatorToken::getOperationType() const {
    return this->opType;
}

OperatorToken::OperatorToken(int location, int index, OperationType operationType) : opType(operationType), Token(TOKEN_OPERATOR, location, index) {

}

char Parser::peek() {
    return *instructionPointer;
}

void Parser::consume() {
    instructionPointer++;
}

void Parser::pop() {
    currentToken.append(instructionPointer);
    consume();
}

bool Parser::isTerminator() {
    return typing::isOperator(instructionPointer) || contains("()\t\n ", instructionPointer) || *instructionPointer == '\0';
}

OperationType Parser::determineOperatorType() {
    switch(*currentToken.c_str()) {
        case '+':
            return OP_ADD;
        case '-':
            return OP_MIN;
        case '*':
            return OP_MUL;
        case '^':
            return OP_EXP;
        case '/':
            return OP_DIV;
        default:
            return OP_ERR;
    }
}

TokenType Parser::determineTokenType() {
    if (typing::isNumber(currentToken.c_str())) {
        return TOKEN_CONSTANT;
    } else if (typing::isIdentifier(currentToken.c_str())) {
        return TOKEN_VARIABLE;
    } else if (currentToken == "(") {
        return TOKEN_LEFT_PAREN;
    } else if (currentToken == ")") {
        return TOKEN_RIGHT_PAREN;
    } else if (typing::isOperator(currentToken.c_str())) {
        return TOKEN_OPERATOR;
    } else {
        return TOKEN_UNDEFINED;
    }
}

TokenType Parser::consumeToken() {
    while (!isTerminator()) {
        pop();
    }
    return determineTokenType();
}

void Parser::consumeRedundant() {
    while(typing::isWhiteSpace(instructionPointer)) {
        consume();
    }
}

ParsingStatus Parser::parseToken() {
    TokenType currentTokenType = consumeToken(); // This sets the currentToken to the string rep of the token.
    Token *newToken;
    int index = tokens.size();
    int location = (long) (instructionPointer - source);
    // Time to generate a new token.
    switch(currentTokenType) {
        case TOKEN_CONSTANT:
            newToken = new ConstantToken(location, index, std::atof(currentToken.c_str()));
            break;
        case TOKEN_VARIABLE:
            newToken = new VariableToken(location, index, currentToken.c_str());
            break;
        case TOKEN_OPERATOR:
            newToken = new OperatorToken(location, index, determineOperatorType());
            break;
        case TOKEN_LEFT_PAREN:
        case TOKEN_RIGHT_PAREN:
            newToken = new Token(currentTokenType, location, index);
            break;
        case TOKEN_UNDEFINED:
            return PARSING_FAILED;
    }
    tokens.push_back(newToken);
    if (*instructionPointer == '\0') {
        return PARSING_COMPLETED; // At the end of the string.
    } else {
        return PARSING_IN_PROGRESS;
    }
}

ParsingStatus Parser::parse() {
    ParsingStatus currentStatus;
    do {
      currentStatus = parseToken();
      currentToken = "";
      consumeRedundant(); // If pointer is on whitespace, bring it to a usable character.
    } while(currentStatus == PARSING_IN_PROGRESS);
    return currentStatus;
}

std::vector<Token*> Parser::getTokens() {
    return this->tokens;
}

Parser::Parser(const char *source) {
    this->source = source;
    this->instructionPointer = (char *) source;
}