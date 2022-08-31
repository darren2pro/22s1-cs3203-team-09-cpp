#include "PlusNode.h"

string PlusNode::toString() {
    return std::string();
}

void PlusNode::setLeftSubtree(TNode::VARIABLE_NODE_PTR leftSubtree) {
    this->leftSubtree = leftSubtree;
}

void PlusNode::setRightSubtree(TNode::CONSTANT_NODE_PTR rightSubtree) {
    this->rightSubtree = rightSubtree;
}
