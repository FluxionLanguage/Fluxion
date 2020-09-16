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
    OP_EXP,
    OP_ERR // Error, undefined operation.
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
    virtual std::string getString();
};


/**
 * This represents constants, a number literal.
 */
class Constant : public Expression {
    double value; // If it has numerical value, if not NaN.
public:
    explicit Constant(double value);
    double getValue();
    std::string getString() override;
};


/**
 * Represents a symbolic variable.
 */
class Variable : public Expression {
    std::string name;
public:
    explicit Variable(const std::string& name);
    std::string getVariableName();
    std::string getString() override;
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
    /**
     * If both left and right operations are identical than the expresion may be
     * reduced further trivially. ie: (a - b) - (a -b) can be reduced to 0.
     *
     * @param leftOp Left operation
     * @param rightOp right operation
     * @return new expression.
     */
    Expression *reduceIdenticalOperationExpr(Operation *leftOp, Operation *rightOp);
    /**
     * If both sides of the operation are operations and if they share a common denominator,
     * than they can be factored in, for instance 3(a - b) + 4(a - b) can easily be turned to
     * 7(a - b) or 4a * 3a = 12 * a ^ 2.
     *
     * @param leftOp Left operation
     * @param rightOp
     * @return
     */
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
    std::string getString() override;
};

#endif //FLUXION_EXPRESSION_H
