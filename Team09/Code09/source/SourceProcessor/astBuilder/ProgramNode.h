#pragma once

#include "TNode.h"

using namespace std;

class ProgramNode : public TNode {
public:
    ProgramNode();

    string toString() override;

    void addProcedure(PROCEDURE_NODE_PTR procedure);

    bool isDesignEntity() override;

    virtual bool isAssignmentNode() override;

    PROCEDURE_PTR_LST procedureList;
};
