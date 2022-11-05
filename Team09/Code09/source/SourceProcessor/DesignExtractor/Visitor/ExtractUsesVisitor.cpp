#include "ExtractUsesVisitor.h"

void ExtractUsesVisitor::extractIndirectUses() {
    for (const auto& lineProc : getLineCallsProc()) {
        PKB::LineNum lnNum = lineProc.first;
        PKB::Procedure proc = lineProc.second;

        if (pkbStorage->relationContainsFirst(Relation::UsesP, proc)) {
            for (const auto& var : pkbStorage->getRelationSecondFromFirst(Relation::UsesP, proc)) {
                pkbStorage->storeRelations(Relation::UsesS, lnNum, var);

                if (pkbStorage->relationContainsSecond(Relation::ParentT, lnNum)) {
                    for (const auto& elem : pkbStorage->getRelationFirstFromSecond(Relation::ParentT, lnNum)) {
                        pkbStorage->storeRelations(Relation::UsesS, elem, var);
                    }
                }
            }
        }
    }
}

void ExtractUsesVisitor::visitProgramNode(std::shared_ptr<ProgramNode> node) {
    for (const auto& proc : node->procList) {
        proc->accept(this);
    }
    extractIndirectUses();
}
void ExtractUsesVisitor::visitProcedureNode(std::shared_ptr<ProcedureNode> node) {
    for (const auto& stmt : node->stmtList) {
        this->stmt = stmt;
        std::visit(
            [this](const auto& s) {
                s->accept(this);
            },
            stmt);
    }
}
void ExtractUsesVisitor::visitIfNode(std::shared_ptr<IfNode> node) {
    node->condExpr->accept(this);
    for (const auto& stmt : node->thenStmtList) {
        this->stmt = stmt;
        std::visit(
            [this](const auto& s) {
                s->accept(this);
            },
            stmt);
    }
    for (const auto& stmt : node->elseStmtList) {
        this->stmt = stmt;
        std::visit(
            [this](const auto& s) {
                s->accept(this);
            },
            stmt);
    }
}
void ExtractUsesVisitor::visitWhileNode(std::shared_ptr<WhileNode> node) {
    node->condExpr->accept(this);
    for (const auto& stmt : node->stmtList) {
        this->stmt = stmt;
        std::visit(
            [this](const auto& s) {
                s->accept(this);
            },
            stmt);
    }
}
void ExtractUsesVisitor::visitAssignmentNode(std::shared_ptr<AssignmentNode> node) {
    visitExpr(node->expr, this);
}
void ExtractUsesVisitor::visitPrintNode(std::shared_ptr<PrintNode> node) {
    node->var->accept(this);
}
void ExtractUsesVisitor::visitVariableNode(std::shared_ptr<VariableNode> node) {
    const PKB::LineNum lnNum = getLineFromNode(stmt);
    const PKB::Procedure proc = getProcedureFromLine(lnNum);
    pkbStorage->storeRelations(Relation::UsesS, lnNum, node->varName);
    pkbStorage->storeRelations(Relation::UsesP, proc, node->varName);

    if (pkbStorage->relationContainsSecond(Relation::ParentT, lnNum)) {
        for (const auto& elem : pkbStorage->getRelationFirstFromSecond(Relation::ParentT, lnNum)) {
            pkbStorage->storeRelations(Relation::UsesS, elem, node->varName);
        }
    }

    if (pkbStorage->relationContainsSecond(Relation::CallsT, proc)) {
        for (const auto& elem : pkbStorage->getRelationFirstFromSecond(Relation::CallsT, proc)) {
            pkbStorage->storeRelations(Relation::UsesP, elem, node->varName);
        }
    }
}
void ExtractUsesVisitor::visitBinOpNode(std::shared_ptr<BinOpNode> node) {
    visitExpr(node->leftExpr, this);
    visitExpr(node->rightExpr, this);
}
void ExtractUsesVisitor::visitCondExprNode(std::shared_ptr<CondExprNode> node) {
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
void ExtractUsesVisitor::visitRelExprNode(std::shared_ptr<RelExprNode> node) {
    visitExpr(node->leftRel, this);
    visitExpr(node->rightRel, this);
}