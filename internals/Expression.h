#ifndef FLUXION_EXPRESSION_H
#define FLUXION_EXPRESSION_H
#include <string>
#include <cmath>
#include "util.h"

enum ExpressionType {
    EXPRESSION_CONSTANT,
    EXPRESSION_VARIABLE,
    EXPRESSION_OPERATION
};

enum OperationType {
    OP_ADD,
    OP_MIN,
    OP_DIV,
    OP_MUL,
    OP_EXP
};

/**
 * Represents a general symbol
 */
class Expression {
public:
    std::string _hash;
    ExpressionType type;
    Expression *evaluate();
    bool operator== (const Expression& other) const;
};


/**
 * This represents constants, a number literal.
 */
class Constant : public Expression {
    double value; // If it has numerical value, if not NaN.
public:
    Constant(double value);
    Expression *evaluate();
    double getValue();
};


/**
 * Represents a symbolic variable.
 */
class Variable : public Expression {
    std::string name;
public:
    Variable(const std::string& name);
    Expression *evaluate();
    std::string getVariableName();
};

/**
 * Represents an operation.
 */
class Operation : public Expression {
private:
    OperationType opType;
    /**
     * If the subnodes of left and right is ordered in a
     * specific way, such that the left and right are Operation
     * Expressions, and they have a child that evaluate to the
     * same Expression, an Operation Expression can be Factored
     * into another Operation Expression.
     *
     * @return true if two grandchildren from different parents
     * evaluate to the same.
     */
    bool isOperationFactorable(Operation *leftOp, Operation *rightOp);
    /**
     * If both left and right evaluations return constants, the
     * operation can be reduced.
     *
     * @param leftEvaluated Evaluated expression on the left.
     * @param rightEvaluated Evaluated expression on the right.
     * @return A constant expression.
     */
    Constant *reduceConstantExpr(Expression *leftEvaluated, Expression *rightEvaluated);
    /**
     * If both left and right evaluates to variables of the same type,
     * the expression may be reduced to constants (a/a = 1, a-a = 0) or Operation
     * Expression (a + a = 2 * a), (a*a = a^2) or itself (a^a = a^a).
     *
     * @param leftVar Left variable expression.
     * @param rightVar Right variable expression.
     * @return (possibly) reduced expression.
     */
    Expression *reduceVariableExpr(Variable *leftVar, Variable *rightVar);
    Expression *reduceIdenticalOperationExpr(Operation *leftOp, Operation *rightOp);
    Operation *reduceFactorableOperationExpr(Operation *leftOp, Operation *rightOp);
    /**
     * If evaluated expression pointers are *created* during evaluation
     * and not actually references to the existing objects, free them.
     *
     * @param leftEvaluated Left evaluated.
     * @param rightEvaluated Right Evaluated.
     */
    void freeEvaluationPointers(Expression *leftEvaluated, Expression *rightEvaluated);
public:
    Operation(Expression *left, Expression *right, OperationType opType);
    Expression *evaluate();

    Expression *left;
    Expression *right;
};

#endif //FLUXION_EXPRESSION_H
