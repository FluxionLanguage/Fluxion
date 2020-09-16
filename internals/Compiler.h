#ifndef FLUXION_COMPILER_H
#define FLUXION_COMPILER_H
#include "Parser.h"
#include "Expression.h"
#define PRECEDENCE_LEVEL_COUNT 3

enum CompilationStatus {
    COMPILATION_SUCCESSFUL,
    COMPILATION_FAILED
};

/**
 * Represents a tuple of operations of same precedence.
 */
class OperationTuple {
private:
    OperationType op1;
    OperationType op2;
public:
    OperationTuple(OperationType op1, OperationType op2) : op1(op1), op2(op2) {};
    /**
     * Check if the given type is one of the types inside the tuple.
     * @param type Type to check for
     * @return true if type is either op1 or op2.
     */
    inline bool isOneOf(OperationType type) {return type == op1 || type == op2;}
};

class Compiler {
private:
    Expression *root;
    std::vector<Token*> tokens;
    CompilationStatus status;
    OperationTuple *orderOfOperations;
    Expression *compileBasicExpression(Token *token);
    Expression *compile(int tokenStartIndex, int tokenStopIndex, int precedenceLevel);
public:
    CompilationStatus compile();
    Expression *getRoot();
    explicit Compiler(std::vector<Token*> tokens);

};

#endif //FLUXION_COMPILER_H
