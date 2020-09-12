#include <cmath>
#include "Expression.h"



bool Expression::operator==(const Expression &other) const {
    return this->_hash == other._hash;
}

Constant::Constant(double value) : value(value){
    this->type = EXPRESSION_VARIABLE;
    this->_hash = hashValue(value);
}

Variable::Variable(const std::string& name) : name(name) {
    this->type = EXPRESSION_VARIABLE;
    this->_hash = hashValue(name);
}

Expression * Expression::evaluate() {
    return this;
}

Operation::Operation(Expression *left, Expression *right, OperationType opType) : left(left), right(right), opType(opType) {
    this->type = EXPRESSION_OPERATION;
    // We use a Merkle-Tree like structure for the hashes of operations.
    if (opType == OP_MUL || opType == OP_ADD) {
        // Since * and + do not distinguish between position of their arguments, we eliminate the positioning thusly.
        this->_hash = hashValue(((left->_hash > right->_hash) ? left->_hash + right->_hash : right->_hash + left->_hash) + hashValue(this->opType));
        // We did this by concatenating bigger one to smaller one rather than preferring left or right.
    } else { // But -, / and ^ does care about which side which argument is, therefore we don't lose that information.
        this->_hash = hashValue(left->_hash + right->_hash + hashValue(this->opType));
        // We prefer adding right to left here.
    }
}

bool Operation::isOperationFactorable(Operation *leftOp, Operation *rightOp) {
    return (this->opType != OP_EXP // Since exponentiation isn't distributive.
            &&*leftOp->left == *rightOp->left
            || *leftOp->left == *rightOp->right
            || *leftOp->right == *rightOp->left
            || *leftOp->right == *rightOp->left);
}

Constant * Operation::reduceConstantExpr(Expression *leftEvaluated, Expression *rightEvaluated) {
    double newValue = 0;
    auto *leftC = (Constant*) leftEvaluated; // Casting them.
    auto *rightC = (Constant*) rightEvaluated; // Casting them.
    switch (this->opType) {
        case OP_ADD:
            newValue = leftC->getValue() + rightC->getValue();
            break;
        case OP_MIN:
            newValue = leftC->getValue() - rightC->getValue();
            break;
        case  OP_DIV:
            newValue = leftC->getValue() / rightC->getValue();
            break;
        case OP_MUL:
            newValue = leftC->getValue() + rightC->getValue();
            break;
        case OP_EXP:
            newValue = pow(leftC->getValue(), rightC->getValue());
            break;
        default:
            newValue = NAN;
            break;
    }
    auto *newConstant = new Constant(newValue);
    return newConstant;
}

Expression *Operation::reduceVariableExpr(Variable *leftVar, Variable *rightVar) {
    switch(this->opType) {
        case OP_ADD:
            return new Operation(new Constant(2), new Variable(leftVar->getVariableName()), OP_MUL);
        case OP_MIN:
            return new Constant(0);
        case OP_DIV:
            return new Constant(1);
        case OP_MUL:
            return new Operation(new Variable(leftVar->getVariableName()), new Constant(2), OP_EXP);
        default:
            return this;
    }
}

Expression *Operation::reduceIdenticalOperationExpr(Operation *leftVar, Operation *rightVar) {
    switch (this->opType) {
        case OP_ADD:
            return new Operation(new Constant(2), leftVar, OP_MUL);
        case OP_MIN:
            return new Constant(0);
        case OP_DIV:
            return new Constant(1);
        case OP_MUL:
            return new Operation(leftVar, new Constant(2), OP_EXP);
        case OP_EXP:
            return new Operation(leftVar, rightVar, OP_EXP); // This cannot be reduced further.
            break;
    }
}

Operation *Operation::reduceFactorableOperationExpr(Operation *leftOp, Operation *rightOp) {
    // Determine the different ones and the same ones.
    Expression* differentLeft;
    Expression* differentRight;
    Expression* same;
    same = (*leftOp->left == *rightOp->left || *leftOp->left == *rightOp->right) ? leftOp->left : leftOp->right;
    differentLeft = (*same == *leftOp->left) ? leftOp->right : leftOp->left;
    differentRight = (*same == *rightOp->left) ? rightOp->right : rightOp->left;
    // Build the multiplicand
    Operation *multiplicand = new Operation(differentLeft, differentRight, this->opType);
    // We should also evaluate the multiplicand since it may reduce further, in case grandchildren have a
    // Reducible relationship, ie 5x + 3x = (5 + 3)x = 8x or similar.
    Expression *multiplicandEvaluated = multiplicand->evaluate();
    delete multiplicand; // TODO: Check if this causes a memory error.
    // If the current operation is multiplication or division, than
    // the multiplier also changes.
    if (this->opType == OP_MUL) {
        same = new Operation(same, new Constant(2), OP_EXP);
    } else if (this->opType == OP_DIV) {
        same = new Constant(1);
    }
    // Now we can construct the new multiplication operation
    return new Operation(multiplicandEvaluated, same, OP_MUL);

}

void Operation::freeEvaluationPointers(Expression *leftEvaluated, Expression *rightEvaluated) {
    if (leftEvaluated->type != left->type) {
        delete leftEvaluated;
    }
    if (rightEvaluated->type != right->type) {
        delete rightEvaluated;
    }
}

Expression * Operation::evaluate() {
    Expression *leftEvaluated = this->left->evaluate();
    Expression *rightEvaluated = this->right->evaluate();
    Expression *newExpression = nullptr;
    // Only time an operation is reduced is when both sides
    // Evaluate to a numerical constant.
    if (leftEvaluated->type == rightEvaluated->type) { // If both are of the same type, they can be reduced.
        if (leftEvaluated->type == EXPRESSION_CONSTANT) {
            newExpression = reduceConstantExpr(leftEvaluated, rightEvaluated);
        } else if (leftEvaluated->type == EXPRESSION_VARIABLE)
        { // Variables, may also be reduced
            auto leftVar = (Variable *) leftEvaluated;
            auto rightVar = (Variable *) rightEvaluated;
            if (leftVar->getVariableName() == rightVar->getVariableName() && this->opType != OP_EXP) { // Here, reduction occurs in these conds.
                newExpression = reduceVariableExpr(leftVar, rightVar);
            }
        } else if (leftEvaluated->type == EXPRESSION_OPERATION) {
            auto leftOp = (Operation *) leftEvaluated;
            auto rightOp = (Operation *) rightEvaluated;
            if (leftEvaluated == rightEvaluated) {
                return reduceIdenticalOperationExpr(leftOp, rightOp);
                // We do not want to free the evaluation pointers.
            } else if (isOperationFactorable(leftOp, rightOp)) {
                return reduceFactorableOperationExpr(leftOp, rightOp);
                // Likewise, left and right pointers must be intact here.
            }
        }
        freeEvaluationPointers(leftEvaluated, rightEvaluated);
    } else {// Otherwise,  Return type will still be a operation, but one of the left or right sides might have reduced.
        newExpression = new Operation(leftEvaluated, rightEvaluated, this->opType);
    }
    return newExpression;
}


