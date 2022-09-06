#pragma once

#include "BinaryOperatorNode.h"

using namespace std;

class PlusNode : public BinaryOperatorNode {
public:
    string toString() override;

    bool isDesignEntity() override;

    bool isAssignmentNode() override;
};
