#pragma once

#include "TNode.h"

using namespace std;

class BinaryOperatorNode : public TNode {
public:
    T_NODE_PTR leftSubtree;

    T_NODE_PTR rightSubtree;

    void setLeftSubtree(T_NODE_PTR leftSubtree);

    void setRightSubtree(T_NODE_PTR rightSubtree);

    bool isBinaryOperatorNode() override {
        return true;
    };
};
