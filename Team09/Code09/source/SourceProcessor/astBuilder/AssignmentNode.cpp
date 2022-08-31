#include "AssignmentNode.h"

string AssignmentNode::toString() {
    return std::string();
}

void AssignmentNode::addVariable(TNode::VARIABLE_NODE_PTR variableNode) {
    this->variableNode = variableNode;
}

void AssignmentNode::addPlus(TNode::PLUS_NODE_PTR plusNode) {
    this->plusNode = plusNode;
}
