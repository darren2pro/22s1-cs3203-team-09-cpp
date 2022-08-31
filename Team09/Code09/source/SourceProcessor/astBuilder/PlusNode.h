#pragma once

#include "TNode.h"

using namespace std;

class PlusNode : public TNode {
private:
    VARIABLE_NODE_PTR leftSubtree;

    CONSTANT_NODE_PTR rightSubtree;

public:
    string toString() override;

    void setLeftSubtree(VARIABLE_NODE_PTR leftSubtree);

    void setRightSubtree(CONSTANT_NODE_PTR rightSubtree);

    bool isDesignEntity() override;

    virtual bool isAssignmentNode() override;
};
