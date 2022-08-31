#include "ConstantNode.h"

ConstantNode::ConstantNode(string value) {
    this->value = value;
}

string ConstantNode::toString() {
    return std::string();
}

bool ConstantNode::isDesignEntity() {
    return true;
}

bool ConstantNode::isAssignmentNode() {
    return false;
}
