#pragma once

#include "TNode.h"
#include <string>
#include <memory>

using namespace std;

class StmtLstNode : public TNode {
public:
    StmtLstNode();

    string toString() override;

    void addStatement(STATEMENT_NODE_PTR statement);

    bool isDesignEntity() override;

    virtual bool isAssignmentNode() override;

    STATEMENT_PTR_LST statementLstPtrs;
};
