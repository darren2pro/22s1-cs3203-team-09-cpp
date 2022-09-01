#pragma once

#include "TNode.h"

using namespace std;

class VariableNode : public TNode {
public:
    explicit VariableNode(string variableName);

    string toString() override;

    bool isDesignEntity() override;

    virtual bool isAssignmentNode() override;

    string variableName;
};
