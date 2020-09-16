#include "Compiler.h"

#include <utility>

Compiler::Compiler(std::vector<Token*> tokens) : root(nullptr), status(COMPILATION_SUCCESSFUL) {
    this->tokens = reverseVector(std::move(tokens)); // We could make this work in reverse.
    this->orderOfOperations = (OperationTuple*) malloc(PRECEDENCE_LEVEL_COUNT * sizeof(OperationTuple));
    this->orderOfOperations[0] = {OP_ADD, OP_MUL};
    this->orderOfOperations[1] = {OP_MIN, OP_DIV};
    this->orderOfOperations[2] = {OP_EXP, OP_EXP};
}

Expression *Compiler::getRoot() {
    return this->root;
}

Expression *Compiler::compileBasicExpression(Token *token) {
    switch (token->getTokenType()) {
        case TOKEN_CONSTANT:
            return new Constant(dynamic_cast<ConstantToken*>(token)->getValue());
        case TOKEN_VARIABLE:
            return new Variable(dynamic_cast<VariableToken*>(token)->getName());
        default:
            return nullptr;
    }
}

Expression *Compiler::compile(int tokenStartIndex, int tokenStopIndex, int precedenceLevel) {
    if (tokenStartIndex + 1 == tokenStopIndex) { // Only a single token left, termination condition.
        return compileBasicExpression(this->tokens[tokenStartIndex]);
    } else if (precedenceLevel > PRECEDENCE_LEVEL_COUNT) { // We are looking for an operator that doesn't exist.
        this->status = COMPILATION_FAILED; // Set the status to failed.
        return nullptr; // return nullptr.
    }
    Token *token;
    OperationType currentOpType;
    for (int i = tokenStartIndex; i < tokenStopIndex; i++) {
        token = tokens[i];
        if (token->getTokenType() == TOKEN_OPERATOR) {
            // If it is an operator check if it is in the precedence level we want.
            // First, get its operation type.
            currentOpType = dynamic_cast<OperatorToken*>(token)->getOperationType();
            if (this->orderOfOperations[precedenceLevel].isOneOf(currentOpType)) {
                // Left and right is reversed since we are working on a reverse list.
                return new Operation(compile(i, tokenStopIndex, 0), compile(tokenStartIndex, i, 0), currentOpType);
            }
        }
    }
    // If we couldn't find any operator with this precedence level in the current expression,
    // Try compiling with the next precedence level.
    return compile(tokenStartIndex, tokenStopIndex, precedenceLevel + 1);

}

CompilationStatus Compiler::compile() {
    this->root = compile(0, tokens.size(), 0);
    return this->status;
}
