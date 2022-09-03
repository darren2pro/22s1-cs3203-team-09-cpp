#include "VariableNode.h"

VariableNode::VariableNode(string variableName) {
    this->variableName = variableName;
}

string VariableNode::toString() {
    return std::string();
}

bool VariableNode::isDesignEntity() {
    return true;
}

bool VariableNode::isAssignmentNode() {
    return false;
}
