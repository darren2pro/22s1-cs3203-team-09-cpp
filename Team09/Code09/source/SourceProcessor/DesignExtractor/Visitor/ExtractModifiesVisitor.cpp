#include "ExtractModifiesVisitor.h"

void ExtractModifiesVisitor::extractIndirectModifies() {
    for (const auto& lineProc : getLineCallsProc()) {
        PKB::LineNum lnNum = lineProc.first;
        PKB::Procedure proc = lineProc.second;

        if (pkbStorage->relationContainsFirst(Relation::ModifiesP, proc)) {
            for (const auto& var : pkbStorage->getRelationSecondFromFirst(Relation::ModifiesP, proc)) {
                pkbStorage->storeRelations(Relation::ModifiesS, lnNum, var);

                if (pkbStorage->relationContainsSecond(Relation::ParentT, lnNum)) {
                    for (const auto& elem : pkbStorage->getRelationFirstFromSecond(Relation::ParentT, lnNum)) {
                        pkbStorage->storeRelations(Relation::ModifiesS, elem, var);
                    }
                }
            }
        }
    }
}

void ExtractModifiesVisitor::visitProgramNode(std::shared_ptr<ProgramNode> node) {
    for (const auto& proc : node->procList) {
        proc->accept(this);
    }
    extractIndirectModifies();
}
void ExtractModifiesVisitor::visitProcedureNode(std::shared_ptr<ProcedureNode> node) {
    for (const auto& stmt : node->stmtList) {
        this->stmt = stmt;
        std::visit(
            [this](const auto& s) {
                s->accept(this);
            },
            stmt);
    }
}
void ExtractModifiesVisitor::visitIfNode(std::shared_ptr<IfNode> node) {
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
void ExtractModifiesVisitor::visitWhileNode(std::shared_ptr<WhileNode> node) {
    for (const auto& stmt : node->stmtList) {
        this->stmt = stmt;
        std::visit(
            [this](const auto& s) {
                s->accept(this);
            },
            stmt);
    }
}
void ExtractModifiesVisitor::visitAssignmentNode(std::shared_ptr<AssignmentNode> node) {
    node->var->accept(this);
}
void ExtractModifiesVisitor::visitReadNode(std::shared_ptr<ReadNode> node) {
    node->var->accept(this);
}
void ExtractModifiesVisitor::visitVariableNode(std::shared_ptr<VariableNode> node) {
    const PKB::LineNum lnNum = getLineFromNode(stmt);
    const PKB::Procedure proc = getProcedureFromLine(lnNum);
    pkbStorage->storeRelations(Relation::ModifiesS, lnNum, node->varName);
    pkbStorage->storeRelations(Relation::ModifiesP, proc, node->varName);

    if (pkbStorage->relationContainsSecond(Relation::ParentT, lnNum)) {
        for (const auto& elem : pkbStorage->getRelationFirstFromSecond(Relation::ParentT, lnNum)) {
            pkbStorage->storeRelations(Relation::ModifiesS, elem, node->varName);
        }
    }
    if (pkbStorage->relationContainsSecond(Relation::CallsT, proc)) {
        for (const auto& elem : pkbStorage->getRelationFirstFromSecond(Relation::CallsT, proc)) {
            pkbStorage->storeRelations(Relation::ModifiesP, elem, node->varName);
        }
    }
}