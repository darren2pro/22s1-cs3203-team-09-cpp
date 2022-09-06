#include "AssignmentNode.h"

string AssignmentNode::toString() {
    return std::string();
}

void AssignmentNode::addAssignedVariable(TNode::VARIABLE_NODE_PTR variableNode) {
    this->variableNode = variableNode;
}

void AssignmentNode::addExpressionRootNode(TNode::T_NODE_PTR expressionRootNode) {
    this->expressionRootNode = expressionRootNode;
}

bool AssignmentNode::isDesignEntity() {
    return true;
}

TNode::VARIABLE_NODE_PTR AssignmentNode::getAssignedVariableNode() {
    return this->variableNode;
}

bool AssignmentNode::isAssignmentNode() {
    return true;
}
