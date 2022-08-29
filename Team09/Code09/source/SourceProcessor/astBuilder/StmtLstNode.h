#pragma once

#include "TNode.h"

using namespace std;

class StmtLstNode : public TNode {
private:
    STATEMENT_PTR_LST statementLstPtrs;

public:
    StmtLstNode();

    string toString() override;

    void addStatement(STATEMENT_NODE_PTR statement);
};
