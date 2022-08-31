#pragma once

#include "TNode.h"

using namespace std;

class AssignmentNode : public TNode {
private:
    VARIABLE_NODE_PTR variableNode;

    // TODO: Change from plusNode to incorporate other types of operators or constant
    PLUS_NODE_PTR plusNode;

public:
    string toString() override;

    void addVariable(VARIABLE_NODE_PTR variableNode);

    void addPlus(PLUS_NODE_PTR plusNode);
};
