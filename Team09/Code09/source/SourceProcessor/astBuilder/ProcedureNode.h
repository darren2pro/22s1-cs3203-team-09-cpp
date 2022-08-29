#pragma once

#include "TNode.h"
#include "StmtLstNode.h"

using namespace std;

class ProcedureNode : public TNode {
private:
    string name;

    StmtLstNode* statementListNode;

    void initializeStatementLst();

public:
    explicit ProcedureNode(string name);

    string toString() override;

    void addStatement(STATEMENT_NODE_PTR statement);
};
