#pragma once

#include "TNode.h"

using namespace std;

class ConstantNode {
private:
    string value;

public:
    explicit ConstantNode(string value);

    string toString();
};
