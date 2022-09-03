#include "ProcedureNode.h"
#include "StmtLstNode.h"

using namespace std;

ProcedureNode::ProcedureNode(string name) {
    this->name = name;
    initializeStatementLst();
}

string ProcedureNode::toString() {
    return string();
}

void ProcedureNode::addStatement(STATEMENT_NODE_PTR statement) {
    statementListNode->addStatement(statement);
}

void ProcedureNode::initializeStatementLst() {
    STMT_LST_NODE_PTR stmtLstNode = make_shared<StmtLstNode>();
    this->statementListNode = stmtLstNode;
}

bool ProcedureNode::isDesignEntity() {
    return true;
}

bool ProcedureNode::isAssignmentNode() {
    return false;
}

TNode::ASSIGNMENT_NODE_PTR ProcedureNode::getStatementByIndex(int index) {
    return statementListNode->getStatementByIndex(index);
}

size_t ProcedureNode::getStatementCount() {
    return statementListNode->getStatementCount();
}
