#pragma once

#include "TNode.h"
#include <string>
#include <memory>

using namespace std;

class StmtLstNode : public TNode {
private:
    STATEMENT_PTR_LST statementLstPtrs;

public:
    StmtLstNode();

    string toString() override;

    void addStatement(STATEMENT_NODE_PTR statement);

    bool isDesignEntity() override;
};
