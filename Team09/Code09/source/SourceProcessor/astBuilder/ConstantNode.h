#pragma once

#include "TNode.h"

using namespace std;

class ConstantNode : public TNode {
private:
    string value;

public:
    explicit ConstantNode(string value);

    string toString() override;

    bool isDesignEntity() override;

    virtual bool isAssignmentNode() override;
};
