#pragma once

#include "TNode.h"
#include "StmtLstNode.h"

using namespace std;

class ProcedureNode : public TNode {
public:
    explicit ProcedureNode(string name);

    string toString() override;

    void addStatement(STATEMENT_NODE_PTR statement);

    bool isDesignEntity() override;

    virtual bool isAssignmentNode() override;

    string name;

    STMT_LST_NODE_PTR statementListNode;

    void initializeStatementLst();
};
