#include "SetLineNumberVisitor.h"

PKB::LineNum SetLineNumberVisitor::getCurrLineNumber() {
    return std::to_string(lineNum);
}

void SetLineNumberVisitor::incrementCurrLineNumber() {
    lineNum += 1;
}

PKB::LineNum SetLineNumberVisitor::storeLine(Stmt node) {
    const LineNum currLineNum = getCurrLineNumber();
    pkbStorage->storeEntity(Declaration::Statement, currLineNum);
    incrementCurrLineNumber();
    std::visit(
        [this, currLineNum](const auto& s) {
            nodeToLineMap[s] = currLineNum;
        },
        node);

    return currLineNum;
}

void SetLineNumberVisitor::storeLineToProcedure(const PKB::LineNum lineNum, const PKB::Procedure proc) {
    lineToProcMap[lineNum] = proc;
}

void SetLineNumberVisitor::visitProgramNode(std::shared_ptr<ProgramNode> node) {
    for (const auto& proc : node->procList) {
        this->proc = proc->procName;
        proc->accept(this);
    }
}
void SetLineNumberVisitor::visitProcedureNode(std::shared_ptr<ProcedureNode> node) {
    visitStmtList(node->stmtList, this);
}
void SetLineNumberVisitor::visitAssignmentNode(std::shared_ptr<AssignmentNode> node) {
    PKB::LineNum lnNum = storeLine(node);
    storeLineToProcedure(lnNum, proc);
}
void SetLineNumberVisitor::visitReadNode(std::shared_ptr<ReadNode> node) {
    PKB::LineNum lnNum = storeLine(node);
    storeLineToProcedure(lnNum, proc);
}
void SetLineNumberVisitor::visitPrintNode(std::shared_ptr<PrintNode> node) {
    PKB::LineNum lnNum = storeLine(node);
    storeLineToProcedure(lnNum, proc);
}
void SetLineNumberVisitor::visitCallNode(std::shared_ptr<CallNode> node) {
    PKB::LineNum lnNum = storeLine(node);
    storeLineToProcedure(lnNum, proc);
}
void SetLineNumberVisitor::visitIfNode(std::shared_ptr<IfNode> node) {
    PKB::LineNum lnNum = storeLine(node);
    storeLineToProcedure(lnNum, proc);
    visitStmtList(node->thenStmtList, this);
    visitStmtList(node->elseStmtList, this);
}
void SetLineNumberVisitor::visitWhileNode(std::shared_ptr<WhileNode> node) {
    PKB::LineNum lnNum = storeLine(node);
    storeLineToProcedure(lnNum, proc);
    visitStmtList(node->stmtList, this);
}