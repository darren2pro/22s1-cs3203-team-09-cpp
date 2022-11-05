#include "ExtractPatternVisitor.h"

void ExtractPatternVisitor::visitProgramNode(std::shared_ptr<ProgramNode> node) {
    for (const auto& proc : node->procList) {
        proc->accept(this);
    }
}
void ExtractPatternVisitor::visitProcedureNode(std::shared_ptr<ProcedureNode> node) {
    for (const auto& stmt : node->stmtList) {
        this->stmt = stmt;
        std::visit(
            [this](const auto& s) {
                s->accept(this);
            },
            stmt);
    }
}
void ExtractPatternVisitor::visitIfNode(std::shared_ptr<IfNode> node) {
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
void ExtractPatternVisitor::visitWhileNode(std::shared_ptr<WhileNode> node) {
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
void ExtractPatternVisitor::visitAssignmentNode(std::shared_ptr<AssignmentNode> node) {
    const PKB::Variable varName = node->var->varName;
    const PKB::LineNum lnNum = getLineFromNode(node);
    const PKB::ExprStr exprs = std::visit([](const auto& s) { return s->toString(); }, node->expr);
    pkbStorage->storePatterns(Pattern::Assign, varName, lnNum, exprs);
}
void ExtractPatternVisitor::visitVariableNode(std::shared_ptr<VariableNode> node) {
    const PKB::LineNum lnNum = getLineFromNode(stmt);
    const PKB::Variable var = node->varName;

    std::visit(
        [this, lnNum, var](const auto& s) mutable {
            using T = std::decay_t<decltype(s)>;
            if constexpr (std::is_same_v<T, std::shared_ptr<IfNode>>) {
                const PKB::ExprStr exprs = s->condExpr->toString();
                pkbStorage->storePatterns(Pattern::If, var, lnNum, exprs);
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<WhileNode>>) {
                const PKB::ExprStr exprs = s->condExpr->toString();
                pkbStorage->storePatterns(Pattern::While, var, lnNum, exprs);
            }
        },
        stmt);
}
void ExtractPatternVisitor::visitBinOpNode(std::shared_ptr<BinOpNode> node) {
    visitExpr(node->leftExpr, this);
    visitExpr(node->rightExpr, this);
}
void ExtractPatternVisitor::visitCondExprNode(std::shared_ptr<CondExprNode> node) {
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
void ExtractPatternVisitor::visitRelExprNode(std::shared_ptr<RelExprNode> node) {
    visitExpr(node->leftRel, this);
    visitExpr(node->rightRel, this);
}