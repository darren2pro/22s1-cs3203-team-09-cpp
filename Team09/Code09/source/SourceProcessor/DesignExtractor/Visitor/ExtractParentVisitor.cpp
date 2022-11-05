#include "ExtractParentVisitor.h"

void ExtractParentVisitor::extractParent(StmtLst stmts, const std::vector<PKB::ParentLine> parent) {
    if (parent.size() > 0) {
        const PKB::LineNum lastP = parent.back();
        for (const auto& stmt : stmts) {
            const PKB::LineNum lnNum = getLineFromNode(stmt);
            pkbStorage->storeRelations(Relation::Parent, lastP, lnNum);

            for (const auto& currP : parent) {
                pkbStorage->storeRelations(Relation::ParentT, currP, lnNum);
            }
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

void ExtractParentVisitor::visitProgramNode(std::shared_ptr<ProgramNode> node) {
    for (const auto& proc : node->procList) {
        proc->accept(this);
    }
}
void ExtractParentVisitor::visitProcedureNode(std::shared_ptr<ProcedureNode> node) {
    this->parent = std::vector<PKB::ParentLine>();
    extractParent(node->stmtList, parent);
}
void ExtractParentVisitor::visitIfNode(std::shared_ptr<IfNode> node) {
    const PKB::LineNum lnNum = getLineFromNode(node);
    parent.push_back(lnNum);
    const std::vector<PKB::ParentLine> copy = parent;
    extractParent(node->thenStmtList, copy);
    extractParent(node->elseStmtList, copy);
    parent.pop_back();
}
void ExtractParentVisitor::visitWhileNode(std::shared_ptr<WhileNode> node) {
    const PKB::LineNum lnNum = getLineFromNode(node);
    parent.push_back(lnNum);
    const std::vector<PKB::ParentLine> copy = parent;
    extractParent(node->stmtList, copy);
    parent.pop_back();
}