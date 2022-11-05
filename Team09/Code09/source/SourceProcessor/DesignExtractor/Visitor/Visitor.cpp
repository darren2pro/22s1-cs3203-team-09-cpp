#include "Visitor.h"

Visitor::Visitor(PKB::PKBStorage* pkb) : pkbStorage(pkb) {};
Visitor::~Visitor() {}
void Visitor::reset() {
    nodeToLineMap.clear();
    lineToProcMap.clear();
    lineCallsProcSet.clear();
}

std::unordered_map<std::shared_ptr<TNode>, PKB::LineNum> Visitor::nodeToLineMap;
std::unordered_map<PKB::LineNum, PKB::Procedure> Visitor::lineToProcMap;
std::unordered_set<std::pair<PKB::LineNum, PKB::Procedure>, PKB::pairHash> Visitor::lineCallsProcSet;

PKB::LineNum Visitor::getLineFromNode(Stmt stmt) {
    return std::visit(
        [this](const auto& s) {
            if (nodeToLineMap.find(s) != nodeToLineMap.end()) {
                return nodeToLineMap.at(s);
            }
        },
        stmt);
}

PKB::Procedure Visitor::getProcedureFromLine(const PKB::LineNum lineNum) {
    if (Visitor::lineToProcMap.find(lineNum) != Visitor::lineToProcMap.end()) {
        return Visitor::lineToProcMap.at(lineNum);
    }
}

std::unordered_set<std::pair<PKB::LineNum, PKB::Procedure>, PKB::pairHash> Visitor::getLineCallsProc() {
    return Visitor::lineCallsProcSet;
}

void Visitor::visitProgramNode(std::shared_ptr<ProgramNode>) {};
void Visitor::visitProcedureNode(std::shared_ptr<ProcedureNode>) {};
void Visitor::visitConstantNode(std::shared_ptr<ConstantNode>) {};
void Visitor::visitVariableNode(std::shared_ptr<VariableNode>) {};
void Visitor::visitAssignmentNode(std::shared_ptr<AssignmentNode>) {};
void Visitor::visitPrintNode(std::shared_ptr<PrintNode>) {};
void Visitor::visitCallNode(std::shared_ptr<CallNode>) {};
void Visitor::visitReadNode(std::shared_ptr<ReadNode>) {};
void Visitor::visitIfNode(std::shared_ptr<IfNode>) {};
void Visitor::visitWhileNode(std::shared_ptr<WhileNode>) {};
void Visitor::visitBinOpNode(std::shared_ptr<BinOpNode>) {};
void Visitor::visitCondExprNode(std::shared_ptr<CondExprNode>) {};
void Visitor::visitRelExprNode(std::shared_ptr<RelExprNode>) {};

void Visitor::visitStmtList(StmtLst stmtList, Visitor* v) {
    for (const auto& stmt : stmtList) {
        std::visit(
            [v](const auto& s) {
                s->accept(v);
            },
            stmt);
    }
}

void Visitor::visitExpr(Expr expr, Visitor* v) {
    std::visit(
        [v](const auto& e) {
            e->accept(v);
        },
        expr);
}