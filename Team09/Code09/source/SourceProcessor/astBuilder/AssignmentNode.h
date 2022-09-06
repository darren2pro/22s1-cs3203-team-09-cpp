#pragma once

#include "TNode.h"

using namespace std;

class AssignmentNode : public TNode {
public:
    VARIABLE_NODE_PTR variableNode;

    // TODO: Change from plusNode to incorporate other types of operators or constant
    T_NODE_PTR expressionRootNode;

    string toString() override;

    void addAssignedVariable(VARIABLE_NODE_PTR variableNode);

    void addExpressionRootNode(T_NODE_PTR expressionRootNode);

    bool isDesignEntity() override;

    VARIABLE_NODE_PTR getAssignedVariableNode();

    virtual bool isAssignmentNode() override;
};
