#include "StmtLstNode.h"

StmtLstNode::StmtLstNode() {
}

string StmtLstNode::toString() {
    return std::string();
}

void StmtLstNode::addStatement(ASSIGNMENT_NODE_PTR statement) {
    statementLstPtrs.push_back(statement);
}

bool StmtLstNode::isDesignEntity() {
    return false;
}

bool StmtLstNode::isAssignmentNode() {
    return false;
}

TNode::ASSIGNMENT_NODE_PTR StmtLstNode::getStatementByIndex(int index) {
    return statementLstPtrs[index];
}
