#include "EntityExtraction.h"
#include <stack>
#include <memory>

EntityExtraction::EntityExtraction(std::shared_ptr<PKB::PKBStorage> pkb) : pkbStorage(pkb) {};

EntityExtraction::~EntityExtraction() {}

//line numbers
void EntityExtraction::createLineNumbers(const std::shared_ptr<ProgramNode> astRoot) {
    for (const auto& proc : astRoot->procList) {
        createLineNumbers(proc);
    }
}
void EntityExtraction::createLineNumbers(const std::shared_ptr<ProcedureNode> proc) {
    traverseLineNumbers(proc->stmtList);
}
void EntityExtraction::createLineNumbers( const Stmt stmts) {
    const PKB::LineNum lnNum = pkbStorage->storeLine(stmts);
    pkbStorage->getNodeFromLine(lnNum);
    std::visit(
            [this](const auto& s) {
                using T = std::decay_t<decltype(s)>;
                if constexpr (std::is_same_v<T, std::shared_ptr<IfNode>>) {
                    traverseLineNumbers(s->thenStmtList);
                    traverseLineNumbers(s->elseStmtList);
                }
                else if constexpr (std::is_same_v<T, std::shared_ptr<WhileNode>>) {
                    traverseLineNumbers(s->stmtList);
                }
            },
            stmts);
}
void EntityExtraction::traverseLineNumbers(const std::vector<Stmt> stmts) {
    for (const auto& stmt : stmts) {
        createLineNumbers(stmt);
    }
}

//design entity extraction
void EntityExtraction::extractEntities(const std::shared_ptr<ProgramNode> astRoot) {
    for (std::shared_ptr<ProcedureNode> proc : astRoot->procList) {
        extractEntities(proc);
    }
}
void EntityExtraction::extractEntities(const std::shared_ptr<ProcedureNode> proc) {
    pkbStorage->storeProcedure(proc->procName);
    extractStatements(proc -> stmtList);
}
void EntityExtraction::extractEntities(const std::shared_ptr<AssignmentNode> assign) {
    const PKB::LineNum lnNum = pkbStorage -> getLineFromNode(assign);
    pkbStorage->storeAssign(lnNum);
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
    pkbStorage->storeVariable(var->varName);
}
void EntityExtraction::extractEntities(const std::shared_ptr<ConstantNode> cons) {
    pkbStorage->storeConstant(cons->value);
}
void EntityExtraction::extractEntities(const std::shared_ptr<IfNode> ifNode) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(ifNode);
    pkbStorage->storeIf(lnNum);
    extractEntities(ifNode->condExpr);
    extractStatements(ifNode->thenStmtList);
    extractStatements(ifNode->elseStmtList);
}
void EntityExtraction::extractEntities(const std::shared_ptr<WhileNode> whileNode) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(whileNode);
    pkbStorage->storeWhile(lnNum);
    extractEntities(whileNode->condExpr);
    extractStatements(whileNode->stmtList);
}
void EntityExtraction::extractEntities(const std::shared_ptr<ReadNode> readNode) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(readNode);
    pkbStorage->storeRead(lnNum, readNode->var->varName);
    extractEntities(readNode->var);
}
void EntityExtraction::extractEntities(const std::shared_ptr<PrintNode> printNode) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(printNode);
    pkbStorage->storePrint(lnNum, printNode->var->varName);
    extractEntities(printNode->var);

}
void EntityExtraction::extractEntities(const std::shared_ptr<CallNode> callNode) {
    const PKB::LineNum lnNum = pkbStorage->getLineFromNode(callNode);
    // pkbStorage->storeCall(lnNum, callNode -> proc);
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
    pkbStorage->storeModifiesS(lnNum, var->varName);

    PKB::setStarFromBaseMap(pkbStorage->parentTChildToParentMap, pkbStorage->parentChildToParentMap, lnNum);
    if (pkbStorage->parentTChildToParentMap.find(lnNum) != pkbStorage->parentTChildToParentMap.end()) {
        for (const auto& elem : pkbStorage->parentTChildToParentMap.at(lnNum)) {
            pkbStorage->storeModifiesS(elem, var->varName);
        }
    }
}
void EntityExtraction::extractModifyRls(const std::shared_ptr<CallNode>) {}


//! Uses relations
void EntityExtraction::extractUsesRls(const std::shared_ptr<ProgramNode> astRoot) {
    for (const auto& proc : astRoot->procList) {
        extractUsesRls(proc);
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
    pkbStorage->storeUsesS(lnNum, var->varName);

    PKB::setStarFromBaseMap(pkbStorage->parentTChildToParentMap, pkbStorage->parentChildToParentMap, lnNum);
    if (pkbStorage->parentTChildToParentMap.find(lnNum) != pkbStorage->parentTChildToParentMap.end()) {
        for (const auto& elem : pkbStorage->parentTChildToParentMap.at(lnNum)) {
            pkbStorage->storeUsesS(elem, var->varName);
        }
    }
}

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
        pkbStorage->storeFollows(lnNum, nextLn);
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
            pkbStorage->storeParent(lastP, lnNum);
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
    pkbStorage->storeAssignPattern(varName, lnNum, exprs);
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
