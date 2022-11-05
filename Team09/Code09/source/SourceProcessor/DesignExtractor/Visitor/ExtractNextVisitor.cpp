#include "ExtractNextVisitor.h"

void ExtractNextVisitor::traverseStmtList(StmtLst stmtList, Visitor* v) {
    visitStmtList(stmtList, v);

    for (std::size_t i = 0; i < stmtList.size() - 1; i++) {
        auto terminating = getTerminatingLines(stmtList[i], cache);
        const PKB::LineNum lnNum = getLineFromNode(stmtList[i + 1]);

        for (const auto& line : terminating) {
            pkbStorage->storeRelations(Relation::Next, line, lnNum);
        }
    }
}

const std::unordered_set<PKB::LineNum> ExtractNextVisitor::getTerminatingLines(
    const Stmt stmt,
    const std::shared_ptr<std::unordered_map<PKB::LineNum, std::unordered_set<PKB::LineNum>>> cache) {
    return std::visit(
        [this, cache](const auto& n) {
            using T = std::decay_t<decltype(n)>;
            if constexpr (std::is_same_v<T, std::shared_ptr<IfNode>>) {
                return getIfTerminatingLines(n, cache);
            }
            else {
                return getOthersTerminatingLines(n, cache);
            }
        },
        stmt);
}

const std::unordered_set<PKB::LineNum> ExtractNextVisitor::getIfTerminatingLines(
    std::shared_ptr<IfNode> ifNode,
    const std::shared_ptr<std::unordered_map<PKB::LineNum, std::unordered_set<PKB::LineNum>>> cache) {
    const PKB::LineNum lnNum = getLineFromNode(ifNode);
    if (cache->find(lnNum) != cache->end()) {
        return cache->at(lnNum);
    }
    auto& lastIf = ifNode->thenStmtList.back();
    auto& lastElse = ifNode->elseStmtList.back();
    auto terminatingThen = getTerminatingLines(lastIf, cache);        
    auto terminatingElse = getTerminatingLines(lastElse, cache);
    std::unordered_set<PKB::LineNum> terminating;
    for (const auto& line : terminatingThen) {
        terminating.insert(line);
    }
    for (const auto& line : terminatingElse) {
        terminating.insert(line);
    }
    cache->emplace(lnNum, terminating);

    return terminating;
}

const std::unordered_set<PKB::LineNum> ExtractNextVisitor::getOthersTerminatingLines(
    Stmt node,
    const std::shared_ptr<std::unordered_map<PKB::LineNum, std::unordered_set<PKB::LineNum>>> cache) {
    const PKB::LineNum lnNum = getLineFromNode(node);
    if (cache->find(lnNum) != cache->end()) {
        return cache->at(lnNum);
    }
    std::unordered_set<PKB::LineNum> terminating_lines;
    terminating_lines.emplace(lnNum);
    cache->emplace(lnNum, terminating_lines);

    return terminating_lines;
}

void ExtractNextVisitor::visitProgramNode(std::shared_ptr<ProgramNode> node) {
    for (const auto& proc : node->procList) {
        proc->accept(this);
    }
}
void ExtractNextVisitor::visitProcedureNode(std::shared_ptr<ProcedureNode> node) {
    this->cache = std::make_shared<std::unordered_map<PKB::LineNum, std::unordered_set<PKB::LineNum>>>();
    traverseStmtList(node->stmtList, this);
}
void ExtractNextVisitor::visitIfNode(std::shared_ptr<IfNode> node) {
    const PKB::ParentLine parent = getLineFromNode(node);
    const PKB::ChildLine thenChild = getLineFromNode(node->thenStmtList.front());
    const PKB::ChildLine elseChild = getLineFromNode(node->elseStmtList.front());
    pkbStorage->storeRelations(Relation::Next, parent, thenChild);
    pkbStorage->storeRelations(Relation::Next, parent, elseChild);
    traverseStmtList(node->thenStmtList, this);
    traverseStmtList(node->elseStmtList, this);
}
void ExtractNextVisitor::visitWhileNode(std::shared_ptr<WhileNode> node) {
    const PKB::ParentLine parent = getLineFromNode(node);
    const PKB::ChildLine child = getLineFromNode(node->stmtList.front());
    auto terminating = getTerminatingLines(node->stmtList.back(), cache);
    for (const auto& line : terminating) {
        pkbStorage->storeRelations(Relation::Next, line, parent);
    }
    pkbStorage->storeRelations(Relation::Next, parent, child);
    traverseStmtList(node->stmtList, this);
}