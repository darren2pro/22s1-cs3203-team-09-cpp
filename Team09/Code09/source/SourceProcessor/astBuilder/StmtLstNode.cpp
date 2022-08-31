#include "StmtLstNode.h"

StmtLstNode::StmtLstNode() {
}

string StmtLstNode::toString() {
    return std::string();
}

void StmtLstNode::addStatement(TNode::STATEMENT_NODE_PTR statement) {
    statementLstPtrs.push_back(statement);
}

bool StmtLstNode::isDesignEntity() {
    return false;
}

bool StmtLstNode::isAssignmentNode() {
    return false;
}
