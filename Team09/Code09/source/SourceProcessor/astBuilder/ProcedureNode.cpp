#include "ProcedureNode.h"
#include "StmtLstNode.h"

using namespace std;

ProcedureNode::ProcedureNode(string name) {
    this->name = name;
    initializeStatementLst();
}

string ProcedureNode::toString() {
    return std::string();
}

void ProcedureNode::addStatement(STATEMENT_NODE_PTR statement) {
    statementListNode.addStatement(statement);
}

void ProcedureNode::initializeStatementLst() {
    StmtLstNode* stmtLstNode = make_shared<StmtLstNode>();
    this->statementListNode = stmtLstNode;
}
