#pragma once

#include "TNode.h"
#include <string>
#include <memory>

using namespace std;

class StmtLstNode : public TNode {
public:
    STATEMENT_PTR_LST statementLstPtrs;

    StmtLstNode();

    string toString() override;

    void addStatement(ASSIGNMENT_NODE_PTR statement);

    bool isDesignEntity() override;

    virtual bool isAssignmentNode() override;

    ASSIGNMENT_NODE_PTR getStatementByIndex(int index);

    size_t getStatementCount();
};
