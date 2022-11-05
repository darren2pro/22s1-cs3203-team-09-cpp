#include "ExtractEntitiesVisitor.h"

void ExtractEntitiesVisitor::storeLineCallsProc(PKB::LineNum lineNum, PKB::Procedure proc) {
    std::pair<LineNum, Procedure> pair = std::make_pair(lineNum, proc);
    lineCallsProcSet.insert(pair);
}

void ExtractEntitiesVisitor::visitProgramNode(std::shared_ptr<ProgramNode> node) {
    for (const auto& proc : node->procList) {
        proc->accept(this);
    }
}
void ExtractEntitiesVisitor::visitProcedureNode(std::shared_ptr<ProcedureNode> node) {
    pkbStorage->storeEntity(Declaration::Procedure, node->procName);
    visitStmtList(node->stmtList, this);
}
void ExtractEntitiesVisitor::visitAssignmentNode(std::shared_ptr<AssignmentNode> node) {
    const PKB::LineNum lnNum = getLineFromNode(node);
    pkbStorage->storeEntity(Declaration::Assignment, lnNum);
    node->var->accept(this);
    visitExpr(node->expr, this);
}
void ExtractEntitiesVisitor::visitReadNode(std::shared_ptr<ReadNode> node) {
    const PKB::LineNum lnNum = getLineFromNode(node);
    pkbStorage->storeEntity(Declaration::Read, lnNum, node->var->varName);
    node->var->accept(this);
}
void ExtractEntitiesVisitor::visitPrintNode(std::shared_ptr<PrintNode> node) {
    const PKB::LineNum lnNum = getLineFromNode(node);
    pkbStorage->storeEntity(Declaration::Print, lnNum, node->var->varName);
    node->var->accept(this);
}
void ExtractEntitiesVisitor::visitCallNode(std::shared_ptr<CallNode> node) {
    const PKB::LineNum lnNum = getLineFromNode(node);
    pkbStorage->storeEntity(Declaration::Call, lnNum, node->proc->procName);
    storeLineCallsProc(lnNum, node->proc->procName);
}
void ExtractEntitiesVisitor::visitIfNode(std::shared_ptr<IfNode> node) {
    const PKB::LineNum lnNum = getLineFromNode(node);
    pkbStorage->storeEntity(Declaration::If, lnNum);
    node->condExpr->accept(this);
    visitStmtList(node->thenStmtList, this);
    visitStmtList(node->elseStmtList, this);
}
void ExtractEntitiesVisitor::visitWhileNode(std::shared_ptr<WhileNode> node) {
    const PKB::LineNum lnNum = getLineFromNode(node);
    pkbStorage->storeEntity(Declaration::While, lnNum);
    node->condExpr->accept(this);
    visitStmtList(node->stmtList, this);
}
void ExtractEntitiesVisitor::visitBinOpNode(std::shared_ptr<BinOpNode> node) {
    visitExpr(node->leftExpr, this);
    visitExpr(node->rightExpr, this);
}
void ExtractEntitiesVisitor::visitCondExprNode(std::shared_ptr<CondExprNode> node) {
    if (node->relExpr != nullptr) {
        node->relExpr->accept(this);
    }
    if (node->leftCond != nullptr) {
        node->leftCond->accept(this);
    }
    if (node->rightCond != nullptr) {
        node->rightCond->accept(this);
    }
}
void ExtractEntitiesVisitor::visitRelExprNode(std::shared_ptr<RelExprNode> node) {
    visitExpr(node->leftRel, this);
    visitExpr(node->rightRel, this);
}
void ExtractEntitiesVisitor::visitVariableNode(std::shared_ptr<VariableNode> node) {
    pkbStorage->storeEntity(Declaration::Variable, node->varName);
}
void ExtractEntitiesVisitor::visitConstantNode(std::shared_ptr<ConstantNode> node) {
    pkbStorage->storeEntity(Declaration::Constant, node->value);
}