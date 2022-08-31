#pragma once

#include "TNode.h"

using namespace std;

class VariableNode {
private:
    string variableName;

public:
    explicit VariableNode(string variableName);

    string toString();
};
