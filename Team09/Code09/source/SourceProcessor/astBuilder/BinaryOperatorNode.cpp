#include "BinaryOperatorNode.h"

void BinaryOperatorNode::setLeftSubtree(TNode::VARIABLE_NODE_PTR leftSubtree) {
    this->leftSubtree = leftSubtree;
}

void BinaryOperatorNode::setRightSubtree(TNode::CONSTANT_NODE_PTR rightSubtree) {
    this->rightSubtree = rightSubtree;
}
