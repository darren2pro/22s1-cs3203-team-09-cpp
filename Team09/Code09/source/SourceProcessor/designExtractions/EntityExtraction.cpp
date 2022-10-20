#include "EntityExtraction.h"
#include <stack>
#include <memory>

EntityExtraction::EntityExtraction(PKB::PKBStorage* pkb) : pkbStorage(pkb) {};

EntityExtraction::~EntityExtraction() {}

//line numbers
void EntityExtraction::createLineNumbers(const std::shared_ptr<ProgramNode> astRoot) {
    for (const auto& proc : astRoot->procList) {
        createLineNumbers(proc);
    }
}
void EntityExtraction::createLineNumbers(const std::shared_ptr<ProcedureNode> proc) {
    traverseLineNumbers(proc->stmtList, proc->procName);
}
void EntityExtraction::createLineNumbers(const Stmt stmts, const PKB::Procedure procName) {
    const PKB::LineNum lnNum = pkbStorage->storeLine(stmts);
    //pkbStorage->getNodeFromLine(lnNum); (useless statement)
    pkbStorage->storeLineToProcedure(lnNum, procName);
    std::visit(
            [this, procName](const auto& s) {
                using T = std::decay_t<decltype(s)>;
                if constexpr (std::is_same_v<T, std::shared_ptr<IfNode>>) {
                    traverseLineNumbers(s->thenStmtList, procName);
                    traverseLineNumbers(s->elseStmtList, procName);
                }
                else if constexpr (std::is_same_v<T, std::shared_ptr<WhileNode>>) {
                    traverseLineNumbers(s->stmtList, procName);
                }
            },
            stmts);
}
void EntityExtraction::traverseLineNumbers(const std::vector<Stmt> stmts, const PKB::Procedure procName) {
    for (const auto& stmt : stmts) {
        createLineNumbers(stmt, procName);
    }
}

//CFG
void EntityExtraction::createCFG(const std::shared_ptr<ProgramNode> astRoot) {
    for (const auto& proc : astRoot->procList) {
         createCFG(proc);
    }
}
void EntityExtraction::createCFG(const std::shared_ptr<ProcedureNode> proc) {
    const std::shared_ptr<std::unordered_map<PKB::LineNum, std::unordered_set<PKB::LineNum>>> cache =
       std::make_shared<std::unordered_map<PKB::LineNum, std::unordered_set<PKB::LineNum>>>();
    traverseCFG(proc->stmtList, cache);
}
void EntityExtraction::traverseCFG(
    const std::vector<Stmt> stmts,
    const std::shared_ptr<std::unordered_map<PKB::LineNum, std::unordered_set<PKB::LineNum>>> cache) {
    for (const auto& stmt : stmts) {
        std::visit([this, cache](const auto& s) { createCFG(s, cache); }, stmt);
    }

    for (std::size_t i = 0; i < stmts.size() - 1; i++) {
        auto terminating = extractTerminatingLines(stmts[i], cache);
        const PKB::LineNum lnNum = pkbStorage->getLineFromNode(stmts[i + 1]);

        for (const auto& line : terminating) {
            pkbStorage->storeRelations(Relation::Next, line, lnNum);
        }
    }
}
const std::unordered_set<PKB::LineNum> EntityExtraction::extractTerminatingLines(
    const std::shared_ptr<IfNode> ifNode,
    const std::shared_ptr<std::unordered_map<PKB::LineNum, std::unordered_set<PKB::LineNum>>> cache) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(ifNode);
    if (cache->find(lnNum) != cache->end()) {
        return cache->at(lnNum);
    }
    auto lastIf = ifNode->thenStmtList.back();
    auto lastElse = ifNode->elseStmtList.back();
    auto terminatingThen = extractTerminatingLines(lastIf, cache);
    auto terminatingElse = extractTerminatingLines(lastElse, cache);
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
const std::unordered_set<PKB::LineNum> EntityExtraction::extractTerminatingLines(
    const std::shared_ptr<WhileNode> whileNode,
    const std::shared_ptr<std::unordered_map<PKB::LineNum, std::unordered_set<PKB::LineNum>>> cache) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(whileNode);
    if (cache->find(lnNum) != cache->end()) {
        return cache->at(lnNum);
    }
    std::unordered_set<PKB::LineNum> terminating_lines;
    terminating_lines.emplace(lnNum);
    cache->emplace(lnNum, terminating_lines);

    return terminating_lines;
}
const std::unordered_set<PKB::LineNum> EntityExtraction::extractTerminatingLines(
    const std::shared_ptr<ReadNode> readNode,
    const std::shared_ptr<std::unordered_map<PKB::LineNum, std::unordered_set<PKB::LineNum>>> cache) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(readNode);
    if (cache->find(lnNum) != cache->end()) {
        return cache->at(lnNum);
    }
    std::unordered_set<PKB::LineNum> terminating_lines;
    terminating_lines.emplace(lnNum);
    cache->emplace(lnNum, terminating_lines);
    return terminating_lines;
}
const std::unordered_set<PKB::LineNum> EntityExtraction::extractTerminatingLines(
    const std::shared_ptr<PrintNode> printNode,
    const std::shared_ptr<std::unordered_map<PKB::LineNum, std::unordered_set<PKB::LineNum>>> cache) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(printNode);
    if (cache->find(lnNum) != cache->end()) {
        return cache->at(lnNum);
    }
    std::unordered_set<PKB::LineNum> terminating_lines;
    terminating_lines.emplace(lnNum);
    cache->emplace(lnNum, terminating_lines);
    return terminating_lines;
}
const std::unordered_set<PKB::LineNum> EntityExtraction::extractTerminatingLines(
    const std::shared_ptr<AssignmentNode> assignNode,
    const std::shared_ptr<std::unordered_map< PKB::LineNum, std::unordered_set< PKB::LineNum>>> cache) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(assignNode);
    if (cache->find(lnNum) != cache->end()) {
        return cache->at(lnNum);
    }
    std::unordered_set<PKB::LineNum> terminating_lines;
    terminating_lines.emplace(lnNum);
    cache->emplace(lnNum, terminating_lines);
    return terminating_lines;
}
const std::unordered_set<PKB::LineNum> EntityExtraction::extractTerminatingLines(
    const std::shared_ptr<CallNode> callNode,
    const std::shared_ptr<std::unordered_map< PKB::LineNum, std::unordered_set<PKB::LineNum>>> cache) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(callNode);
    if (cache->find(lnNum) != cache->end()) {
        return cache->at(lnNum);
    }
    std::unordered_set<PKB::LineNum> terminating_lines;
    terminating_lines.emplace(lnNum);
    cache->emplace(lnNum, terminating_lines);
    return terminating_lines;
}
const std::unordered_set<PKB::LineNum> EntityExtraction::extractTerminatingLines(
    const Stmt stmt,
    const std::shared_ptr<std::unordered_map<PKB::LineNum, std::unordered_set<PKB::LineNum>>> cache) {
            return std::visit(
                [this, cache](const auto& n) { return extractTerminatingLines(n, cache); },
                stmt);
}
 void EntityExtraction::createCFG(
            const std::shared_ptr<IfNode> ifNode,
            std::shared_ptr<std::unordered_map< PKB::LineNum, std::unordered_set< PKB::LineNum>>> cache) {
            const PKB::ParentLine parent = pkbStorage->getLineFromNode(ifNode);
            const PKB::ChildLine thenChild =
                pkbStorage->getLineFromNode(ifNode->thenStmtList.front());
            const PKB::ChildLine elseChild =
                pkbStorage->getLineFromNode(ifNode->elseStmtList.front());
            pkbStorage->storeRelations(Relation::Next, parent, thenChild);
            pkbStorage->storeRelations(Relation::Next, parent, elseChild);

            traverseCFG(ifNode->thenStmtList, cache);
            traverseCFG(ifNode->elseStmtList, cache);
  }
 void EntityExtraction::createCFG(
     const std::shared_ptr<WhileNode> whileNode,
     std::shared_ptr<std::unordered_map< PKB::LineNum, std::unordered_set< PKB::LineNum>>> cache) {
     const PKB::ParentLine parent = pkbStorage->getLineFromNode(whileNode);
     const PKB::ChildLine child = pkbStorage->getLineFromNode(whileNode->stmtList.front());
     auto terminating = extractTerminatingLines(whileNode->stmtList.back(), cache);
     for (const auto& line : terminating) {
         pkbStorage->storeRelations(Relation::Next, line, parent);
     }
     pkbStorage->storeRelations(Relation::Next, parent, child);
     traverseCFG(whileNode->stmtList, cache);
 }
 void EntityExtraction::createCFG(
     const std::shared_ptr<ReadNode>,
     std::shared_ptr<std::unordered_map< PKB::LineNum, std::unordered_set< PKB::LineNum>>>){}
  void EntityExtraction::createCFG(
     const std::shared_ptr<PrintNode>,
     std::shared_ptr<std::unordered_map< PKB::LineNum, std::unordered_set< PKB::LineNum>>>) {}
  void EntityExtraction::createCFG(
     const std::shared_ptr<AssignmentNode>,
     std::shared_ptr<std::unordered_map< PKB::LineNum, std::unordered_set< PKB::LineNum>>>) {}
 void EntityExtraction::createCFG(
     const std::shared_ptr<CallNode>,
     std::shared_ptr<std::unordered_map< PKB::LineNum, std::unordered_set< PKB::LineNum>>>) {}

 //design entity extraction
void EntityExtraction::extractEntities(const std::shared_ptr<ProgramNode> astRoot) {
    for (std::shared_ptr<ProcedureNode> proc : astRoot->procList) {
        extractEntities(proc);
    }
}
void EntityExtraction::extractEntities(const std::shared_ptr<ProcedureNode> proc) {
    pkbStorage->storeEntity(Declaration::Procedure, proc->procName);
    extractStatements(proc -> stmtList);
}
void EntityExtraction::extractEntities(const std::shared_ptr<AssignmentNode> assign) {
    const PKB::LineNum lnNum = pkbStorage -> getLineFromNode(assign);
    pkbStorage->storeEntity(Declaration::Assignment, lnNum);
    extractEntities(assign->var);
    extractEntities(assign->expr);
}
void EntityExtraction::extractEntities(const Expr exp) {
    std::visit([this](const auto& e) { extractEntities(e); }, exp);
}
void EntityExtraction::extractEntities(const std::shared_ptr<CondExprNode> cond) {
    if (cond == nullptr) {
        return;
    }
    extractEntities(cond->relExpr);
    extractEntities(cond->leftCond);
    extractEntities(cond->rightCond);
}
void EntityExtraction::extractEntities(const std::shared_ptr<RelExprNode> rel) {
    if (rel == nullptr) {
        return;
    }
    extractEntities(rel->leftRel);
    extractEntities(rel->rightRel);
}
void EntityExtraction::extractEntities(const std::shared_ptr<BinOpNode> bin) {
    extractEntities(bin->leftExpr);
    extractEntities(bin->rightExpr);
}
void EntityExtraction::extractEntities(const std::shared_ptr<VariableNode> var) {
    pkbStorage->storeEntity(Declaration::Variable, var->varName);
}
void EntityExtraction::extractEntities(const std::shared_ptr<ConstantNode> cons) {
    pkbStorage->storeEntity(Declaration::Constant, cons->value);
}
void EntityExtraction::extractEntities(const std::shared_ptr<IfNode> ifNode) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(ifNode);
    pkbStorage->storeEntity(Declaration::If, lnNum);
    extractEntities(ifNode->condExpr);
    extractStatements(ifNode->thenStmtList);
    extractStatements(ifNode->elseStmtList);
}
void EntityExtraction::extractEntities(const std::shared_ptr<WhileNode> whileNode) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(whileNode);
    pkbStorage->storeEntity(Declaration::While, lnNum);
    extractEntities(whileNode->condExpr);
    extractStatements(whileNode->stmtList);
}
void EntityExtraction::extractEntities(const std::shared_ptr<ReadNode> readNode) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(readNode);
    pkbStorage->storeEntity(Declaration::Read, lnNum, readNode->var->varName);
    extractEntities(readNode->var);
}
void EntityExtraction::extractEntities(const std::shared_ptr<PrintNode> printNode) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(printNode);
    pkbStorage->storeEntity(Declaration::Print, lnNum, printNode->var->varName);
    extractEntities(printNode->var);

}
void EntityExtraction::extractEntities(const std::shared_ptr<CallNode> callNode) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(callNode);
     pkbStorage->storeEntity(Declaration::Call, lnNum, callNode->proc->procName);
     pkbStorage->storeLineCallsProc(lnNum, callNode->proc->procName);
}
void EntityExtraction::extractStatements(const std::vector<Stmt> stmts) {
    for (const auto& stmt : stmts) {
        std::visit([this](const auto& s) { extractEntities(s); }, stmt);
    }
}

//! Modify relations
void EntityExtraction::extractModifyRls(const std::shared_ptr<ProgramNode> astRoot) {
    for (const auto& proc : astRoot->procList) {
        extractModifyRls(proc);
    }

    extractIndirectModifyRls();
}
void EntityExtraction::extractIndirectModifyRls() {
    for (const auto& lineProc : pkbStorage->getLineCallsProc()) {
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
void EntityExtraction::extractModifyRls(const std::shared_ptr<ProcedureNode> proc) {
    extractModifyStmts(proc->stmtList);

}
void EntityExtraction::extractModifyStmts(const std::vector<Stmt> stmts) {
    for (const auto& stmt : stmts) {
        std::visit([this](const auto& s) { extractModifyRls(s); }, stmt);
    }
}
void EntityExtraction::extractModifyRls(const std::shared_ptr<AssignmentNode> assign) {
    extractModifyHelper(assign->var, assign);
}
void EntityExtraction::extractModifyRls(const std::shared_ptr<PrintNode> print) {}
void EntityExtraction::extractModifyRls(const std::shared_ptr<IfNode> ifNode) {
    extractModifyStmts(ifNode->thenStmtList);
    extractModifyStmts(ifNode->elseStmtList);
}
void EntityExtraction::extractModifyRls(const std::shared_ptr<WhileNode> whileNode) {
    extractModifyStmts(whileNode->stmtList);
}
void EntityExtraction::extractModifyRls(const std::shared_ptr<ReadNode> readNode) {
    extractModifyHelper(readNode->var, readNode);
}
void EntityExtraction::extractModifyHelper(const std::shared_ptr<VariableNode> var, const Stmt stmt) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(stmt);
    const PKB::Procedure proc = pkbStorage->getProcedureFromLine(lnNum);
    pkbStorage->storeRelations(Relation::ModifiesS, lnNum, var->varName);
    pkbStorage->storeRelations(Relation::ModifiesP, proc, var->varName);

    if (pkbStorage->relationContainsSecond(Relation::ParentT, lnNum)) {
        for (const auto& elem : pkbStorage->getRelationFirstFromSecond(Relation::ParentT, lnNum)) {
            pkbStorage->storeRelations(Relation::ModifiesS, elem, var->varName);
        }
    }
    if (pkbStorage->relationContainsSecond(Relation::CallsT, proc)) {
        for (const auto& elem : pkbStorage->getRelationFirstFromSecond(Relation::CallsT, proc)) {
            pkbStorage->storeRelations(Relation::ModifiesP, elem, var->varName);
        }
    }
}
void EntityExtraction::extractModifyRls(const std::shared_ptr<CallNode>) {}


//! Uses relations
void EntityExtraction::extractUsesRls(const std::shared_ptr<ProgramNode> astRoot) {
    for (const auto& proc : astRoot->procList) {
        extractUsesRls(proc);
    }

    extractIndirectUsesRls();
}
void EntityExtraction::extractIndirectUsesRls() {
    for (const auto& lineProc : pkbStorage->getLineCallsProc()) {
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
void EntityExtraction::extractUsesRls(const std::shared_ptr<ProcedureNode> proc) {
    extractUsesStmts(proc->stmtList);

}
void EntityExtraction::extractUsesStmts(const std::vector<Stmt> stmts) {
    for (const auto& stmt : stmts) {
        std::visit([this](const auto& s) { extractUsesRls(s); }, stmt);
    }
}
void EntityExtraction::extractUsesRls(const std::shared_ptr<AssignmentNode> assign) {
    extractUsesHelper(assign->expr, assign);
}
void EntityExtraction::extractUsesRls(const std::shared_ptr<PrintNode> print) {
    extractUsesHelper(print->var, print);
}
void EntityExtraction::extractUsesRls(const std::shared_ptr<IfNode> ifNode) {
    extractUsesHelper(ifNode->condExpr, ifNode);
    extractUsesStmts(ifNode->thenStmtList);
    extractUsesStmts(ifNode->elseStmtList);
}
void EntityExtraction::extractUsesRls(const std::shared_ptr<WhileNode> whileNode) {
    extractUsesHelper(whileNode->condExpr, whileNode);
    extractUsesStmts(whileNode->stmtList);
}
void EntityExtraction::extractUsesRls(const std::shared_ptr<ReadNode> node) {}
void EntityExtraction::extractUsesRls(const std::shared_ptr<CallNode>) {}
void EntityExtraction::extractUsesHelper(const std::shared_ptr<BinOpNode> bin, const Stmt stmt) {
    extractUsesHelper(bin->leftExpr, stmt);
    extractUsesHelper(bin->rightExpr, stmt);
}
void EntityExtraction::extractUsesHelper(const std::shared_ptr<CondExprNode> cond, const Stmt stmt) {
    if (cond == nullptr) {
        return;
    }
    extractUsesHelper(cond->relExpr, stmt);
    extractUsesHelper(cond->leftCond, stmt);
    extractUsesHelper(cond->rightCond, stmt);
}
void EntityExtraction::extractUsesHelper(const std::shared_ptr<RelExprNode> rel, const Stmt stmt) {
    if (rel == nullptr) {
        return;
    }
    extractUsesHelper(rel->leftRel, stmt);
    extractUsesHelper(rel->rightRel, stmt);
}
void EntityExtraction::extractUsesHelper(const Expr node, const Stmt stmt) {
    std::visit(
            [this, stmt](const auto& n) { extractUsesHelper(n, stmt); },
            node);
}
void EntityExtraction::extractUsesHelper(const std::shared_ptr<VariableNode> var, const Stmt stmt) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(stmt);
    const PKB::Procedure proc = pkbStorage->getProcedureFromLine(lnNum);
    pkbStorage->storeRelations(Relation::UsesS, lnNum, var->varName);
    pkbStorage->storeRelations(Relation::UsesP, proc, var->varName);

    if (pkbStorage->relationContainsSecond(Relation::ParentT, lnNum)) {
        for (const auto& elem : pkbStorage->getRelationFirstFromSecond(Relation::ParentT, lnNum)) {
            pkbStorage->storeRelations(Relation::UsesS, elem, var->varName);
        }
    }

    if (pkbStorage->relationContainsSecond(Relation::CallsT, proc)) {
        for (const auto& elem : pkbStorage->getRelationFirstFromSecond(Relation::CallsT, proc)) {
            pkbStorage->storeRelations(Relation::UsesP, elem, var->varName);
        }
    }
}
void EntityExtraction::extractUsesHelper(const std::shared_ptr<ConstantNode> constant, const Stmt stmt) {}

//Follows relations
void EntityExtraction::extractFollowsRls(const std::shared_ptr<ProgramNode> astRoot) {
    for (const auto& proc : astRoot->procList) {
        extractFollowsRls(proc);
    }
}
void EntityExtraction::extractFollowsRls(const std::shared_ptr<ProcedureNode> proc) {
    extractFollowsStmts(proc->stmtList);

}
void EntityExtraction::extractFollowsStmts(const std::vector<Stmt> stmts) {
    for (std::size_t i = 0; i < stmts.size() - 1; i++) {
        const PKB::LineNum lnNum = pkbStorage->getLineFromNode(stmts[i]);
        const PKB::LineNum nextLn = pkbStorage->getLineFromNode(stmts[i + 1]);
        pkbStorage->storeRelations(Relation::Follows, lnNum, nextLn);
    }

    for (std::size_t i = 0; i < stmts.size() - 1; i++) {
        for (std::size_t j = i + 1; j < stmts.size(); j++) {
            const PKB::LineNum lnNum = pkbStorage->getLineFromNode(stmts[i]);
            const PKB::LineNum nextLn = pkbStorage->getLineFromNode(stmts[j]);
            pkbStorage->storeRelations(Relation::FollowsT, lnNum, nextLn);
        }
    }

    for (const auto& stmt : stmts) {
        std::visit(
                [this](const auto& s) {
                    using T = std::decay_t<decltype(s)>;
                    if constexpr (std::is_same_v<T, std::shared_ptr<ProcedureNode>> ||
                                  std::is_same_v<T, std::shared_ptr<IfNode>> ||
                                  std::is_same_v<T, std::shared_ptr<WhileNode>>) {
                        extractFollowsRls(s);
                    }
                },
                stmt);
    }
}
void EntityExtraction::extractFollowsRls(const std::shared_ptr<IfNode> ifNode) {
    extractFollowsStmts(ifNode->thenStmtList);
    extractFollowsStmts(ifNode->elseStmtList);
}
void EntityExtraction::extractFollowsRls(const std::shared_ptr<WhileNode> whileNode) {
    extractFollowsStmts(whileNode->stmtList);
}

//Parents relations
void EntityExtraction::extractParentsRls(const std::shared_ptr<ProgramNode> astRoot) {
    for (const auto& proc : astRoot->procList) {
        extractParentsRls(proc);
    }
}
void EntityExtraction::extractParentsRls(const std::shared_ptr<ProcedureNode> proc) {
    std::vector<PKB::ParentLine> parent;
    extractParentsHelper(proc, parent);
}
void  EntityExtraction::extractParentsHelper(const std::shared_ptr<ProcedureNode> proc, const std::vector<PKB::ParentLine> parent) {
    extractParentsStmts(proc->stmtList, parent);
}
void EntityExtraction::extractParentsStmts(const std::vector<Stmt> stmts, const std::vector<PKB::ParentLine> parent) {
    if (parent.size() > 0) {
        const PKB::LineNum lastP = parent.back();
        for (const auto& stmt : stmts) {
            const PKB::LineNum lnNum = pkbStorage->getLineFromNode(stmt);
            pkbStorage->storeRelations(Relation::Parent, lastP, lnNum);

            for (const auto& currP : parent) {
                pkbStorage->storeRelations(Relation::ParentT, currP, lnNum);
            }
        }
    }
    for (const auto& stmt : stmts) {
        std::visit(
                [this, parent](const auto& s) {
                    using T = std::decay_t<decltype(s)>;
                    if constexpr (std::is_same_v<T, std::shared_ptr<ProcedureNode>> ||
                                  std::is_same_v<T, std::shared_ptr<IfNode>> ||
                                  std::is_same_v<T, std::shared_ptr<WhileNode>>) {
                        extractParentsHelper(s, parent);
                    }
                },
                stmt);
    }
}
void EntityExtraction::extractParentsHelper(const std::shared_ptr<IfNode> ifNode, std::vector<PKB::ParentLine> parent) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(ifNode);
    parent.push_back(lnNum);
    extractParentsStmts(ifNode->thenStmtList, parent);
    extractParentsStmts(ifNode->elseStmtList, parent);
}
void EntityExtraction::extractParentsHelper(const std::shared_ptr<WhileNode> whileNode, std::vector<PKB::ParentLine> parent) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(whileNode);
    parent.push_back(lnNum);
    extractParentsStmts(whileNode->stmtList, parent);
}

// Calls relations
void EntityExtraction::extractCallsRls(const std::shared_ptr<ProgramNode> astRoot) {
    for (const auto& proc : astRoot->procList) {
        extractCallsRls(proc);
    }

    extractCallsTRls();
}
void EntityExtraction::extractCallsTRls() {
    for (const auto& calls : pkbStorage->getRelationSet(Relation::Calls)) {
        PKB::CallerProc caller = calls.first;

        std::vector<PKB::CalleeProc> list;
        list.push_back(caller);
        while (!list.empty()) {
            PKB::CalleeProc currCallee = list.back();
            list.pop_back();
            for (const auto& callee : pkbStorage->getRelationSecondFromFirst(Relation::Calls, currCallee)) {
                pkbStorage->storeRelations(Relation::CallsT, caller, callee);
                if (pkbStorage->relationContainsFirst(Relation::Calls, callee)) {
                    list.push_back(callee);
                }
            }
        }
    }
}
void EntityExtraction::extractCallsRls(const std::shared_ptr<ProcedureNode> proc) {
    extractCallsStmts(proc->stmtList);
}
void EntityExtraction::extractCallsStmts(const std::vector<Stmt> stmts) {
    for (const auto& stmt : stmts) {
        std::visit([this](const auto& s) { extractCallsRls(s); }, stmt);
    }
}
void EntityExtraction::extractCallsRls(const std::shared_ptr<IfNode> ifNode) {
    extractCallsStmts(ifNode->thenStmtList);
    extractCallsStmts(ifNode->elseStmtList);
}
void EntityExtraction::extractCallsRls(const std::shared_ptr<WhileNode> whileNode) {
    extractCallsStmts(whileNode->stmtList);
}
void EntityExtraction::extractCallsRls(const std::shared_ptr<ReadNode>) {}
void EntityExtraction::extractCallsRls(const std::shared_ptr<PrintNode>) {}
void EntityExtraction::extractCallsRls(const std::shared_ptr<AssignmentNode>) {}
void EntityExtraction::extractCallsRls(const std::shared_ptr<CallNode> call) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(call);
    const PKB::Procedure caller = pkbStorage-> getProcedureFromLine(lnNum);
    pkbStorage->storeRelations(Relation::Calls, caller, call->proc->procName);
}

//Assign pattern relations
void EntityExtraction::extractAssignPattern(const std::shared_ptr<ProgramNode> astRoot) {
    for (const auto& proc : astRoot-> procList) {
        extractAssignPattern(proc);
    }
}
void EntityExtraction::extractAssignPattern(const std::shared_ptr<ProcedureNode> proc) {
    extractAssignStmts(proc->stmtList);
}
void EntityExtraction::extractAssignStmts(const std::vector<Stmt> stmts) {
    for (const auto& stmt : stmts) {
        std::visit([this](const auto& s) { extractAssignPattern(s); }, stmt);
    }
}
void EntityExtraction::extractAssignPattern(const std::shared_ptr<AssignmentNode> assign) {
    const PKB::Variable varName = assign->var->varName;
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(assign);
    const PKB::ExprStr  exprs =
        std::visit([this](const auto& s) { return s->toString(); }, assign->expr);
    pkbStorage->storePatterns(Pattern::Assign, varName, lnNum, exprs);
}
void EntityExtraction::extractAssignPattern(const std::shared_ptr<IfNode> ifNode) {
    extractAssignStmts(ifNode->thenStmtList);
    extractAssignStmts(ifNode->elseStmtList);
}
void EntityExtraction::extractAssignPattern(const std::shared_ptr<WhileNode> whileNode) {
    extractAssignStmts(whileNode->stmtList);
}
void EntityExtraction::extractAssignPattern(const std::shared_ptr<ReadNode>) {}
void EntityExtraction::extractAssignPattern(const std::shared_ptr<CallNode>) {}
void EntityExtraction::extractAssignPattern(const std::shared_ptr<PrintNode>) {}
