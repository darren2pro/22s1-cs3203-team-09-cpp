#pragma once

#include <string>
#include <memory>
#include <vector>
#include "ProcedureNode.h"

using namespace std;

class TNode {
public:
    typedef vector<shared_ptr<ProcedureNode>> procedureList;

    virtual ~TNode() = default;
    virtual string toString() = 0;
};
