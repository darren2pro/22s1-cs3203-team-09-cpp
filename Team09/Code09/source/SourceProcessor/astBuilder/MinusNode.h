#pragma once

#include "BinaryOperatorNode.h"

class MinusNode : public BinaryOperatorNode {
public:
    string toString() override;

    bool isDesignEntity() override;

    bool isAssignmentNode() override;
};
