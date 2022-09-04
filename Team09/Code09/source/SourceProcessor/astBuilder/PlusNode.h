#pragma once

#include "TNode.h"

using namespace std;

class PlusNode : public TNode {
public:
    VARIABLE_NODE_PTR leftSubtree;

    CONSTANT_NODE_PTR rightSubtree;

    string toString() override;

    void setLeftSubtree(VARIABLE_NODE_PTR leftSubtree);

    void setRightSubtree(CONSTANT_NODE_PTR rightSubtree);

    bool isDesignEntity() override;

    virtual bool isAssignmentNode() override;
};
