#ifndef FLUXION_PARSER_H
#define FLUXION_PARSER_H

#include <vector>
#include "Expression.h"
#include "util.h"

enum TokenType {
    TOKEN_CONSTANT,
    TOKEN_VARIABLE,
    TOKEN_OPERATOR,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_UNDEFINED, // Malformed tokens.
};

enum ParsingStatus {
    PARSING_FAILED,
    PARSING_IN_PROGRESS,
    PARSING_COMPLETED
};

/**
 * This class represents a Token.
 */
class Token {
private:
    const TokenType type;
    const int location; // Location in raw string.
    const int index; // Index among tokens.
public:
    TokenType getTokenType() const;
    int getLocation() const;
    int getIndex() const;
    virtual ~Token() = default;
    Token(TokenType type, int location, int index);
};

class ConstantToken : public Token {
private:
    const double value;
public:
    ConstantToken(int location, int index, double value);
    double getValue() const;
};

class VariableToken : public Token {
private:
    const char *name; // Variables has single character names.
public:
    const char *getName() const;
    VariableToken(int location, int index, const char *name);
};

class OperatorToken : public Token {
private:
    const OperationType opType;
public:
    OperationType getOperationType() const;
    OperatorToken(int location, int index, OperationType operationType);
};

class Parser {
private:
    const char *source;
    char *instructionPointer;
    std::string currentToken;
    std::vector<Token*> tokens;
    /**
     * Peek at the current char.
     *
     * @return the current char.
     */
    char peek();
    /**
     * Increment the character to the currentToken.
     * the instruction pointer.
     */
    void pop();
    /**
     * Consume the current char without
     * returning it.
     */
    void consume();
    /**
     * Check if the current character is a terminator,
     * that is, the border of which a token is finalised.
     *
     * @return true if the character is either an operator,
     * left or right paranthesis or whitespace.
     */
    bool isTerminator();
    OperationType determineOperatorType();
    TokenType determineTokenType();
    /**
     * Consume a token, filling the current token
     * with characters, return the token type.
     * @return token type of the current token.
     */
    TokenType consumeToken();
    /**
     * Consume reduntant terminal characters such as
     * whitespace.
     *
     * @return
     */
    void consumeRedundant();
    /**
     * Parse a single token, add it to the tokens vector.
     * @return the current parsing status.
     */
    ParsingStatus parseToken();
public:
    /**
     * Parse the entire source code.
     *
     * @return Parsing completed or parsing failed.
     */
    ParsingStatus parse();
    /**
     * This must be ran after a successful parse.
     * @return the tokens vector.
     */
    std::vector<Token*> getTokens();
    explicit Parser(const char *source);
};

#endif //FLUXION_PARSER_H
