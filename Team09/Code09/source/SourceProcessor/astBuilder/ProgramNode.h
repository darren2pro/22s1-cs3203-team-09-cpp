#pragma once

#include "TNode.h"

using namespace std;

class ProgramNode : public TNode {
private:
    PROCEDURE_PTR_LST procedureList;

public:
    ProgramNode();

    string toString() override;

    void addProcedure(PROCEDURE_NODE_PTR procedure);
};
