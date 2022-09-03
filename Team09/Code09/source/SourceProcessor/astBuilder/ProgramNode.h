#pragma once

#include "TNode.h"

using namespace std;

class ProgramNode : public TNode {
public:
    PROCEDURE_PTR_LST procedureList;

    ProgramNode();

    string toString() override;

    void addProcedure(PROCEDURE_NODE_PTR procedure);

    bool isDesignEntity() override;

    virtual bool isAssignmentNode() override;

    PROCEDURE_NODE_PTR getProcedureByIndex(int index);

    size_t getProcedureCount();
};
