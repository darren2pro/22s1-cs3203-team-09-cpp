#pragma once

#include "TNode.h"

using namespace std;

class BinaryOperatorNode : public TNode {
public:
    VARIABLE_NODE_PTR leftSubtree;

    CONSTANT_NODE_PTR rightSubtree;

    void setLeftSubtree(VARIABLE_NODE_PTR leftSubtree);

    void setRightSubtree(CONSTANT_NODE_PTR rightSubtree);
};
