#include "ExtractFollowsVisitor.h"

void ExtractFollowsVisitor::extractFollows(StmtLst stmts) {
    for (std::size_t i = 0; i < stmts.size() - 1; i++) {
        const PKB::LineNum lnNum = getLineFromNode(stmts[i]);
        const PKB::LineNum nextLn = getLineFromNode(stmts[i + 1]);
        pkbStorage->storeRelations(Relation::Follows, lnNum, nextLn);
    }

    for (std::size_t i = 0; i < stmts.size() - 1; i++) {
        for (std::size_t j = i + 1; j < stmts.size(); j++) {
            const PKB::LineNum lnNum = getLineFromNode(stmts[i]);
            const PKB::LineNum nextLn = getLineFromNode(stmts[j]);
            pkbStorage->storeRelations(Relation::FollowsT, lnNum, nextLn);
        }
    }

    for (const auto& stmt : stmts) {
        std::visit(
            [this](const auto& s) {
                using T = std::decay_t<decltype(s)>;
                if constexpr (std::is_same_v<T, std::shared_ptr<IfNode>> ||
                              std::is_same_v<T, std::shared_ptr<WhileNode>>) {
                    s->accept(this);
                }
            },
            stmt);
    }
}

void ExtractFollowsVisitor::visitProgramNode(std::shared_ptr<ProgramNode> node) {
    for (const auto& proc : node->procList) {
        proc->accept(this);
    }
}
void ExtractFollowsVisitor::visitProcedureNode(std::shared_ptr<ProcedureNode> node) {
    extractFollows(node->stmtList);
}
void ExtractFollowsVisitor::visitIfNode(std::shared_ptr<IfNode> node) {
    extractFollows(node->thenStmtList);
    extractFollows(node->elseStmtList);
}
void ExtractFollowsVisitor::visitWhileNode(std::shared_ptr<WhileNode> node) {
    extractFollows(node->stmtList);
}