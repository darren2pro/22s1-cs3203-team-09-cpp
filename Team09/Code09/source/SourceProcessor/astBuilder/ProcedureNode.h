#pragma once

#include "TNode.h"

using namespace std;

class ProcedureNode : public TNode {
private:
    string name;

    STATEMENT_PTR_LST statementList;

public:
    explicit ProcedureNode(string name);

    string toString() override;

    void addStatement(STATEMENT_NODE_PTR statement);
};
