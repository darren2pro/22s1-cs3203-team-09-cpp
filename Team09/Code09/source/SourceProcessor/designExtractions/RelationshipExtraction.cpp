//#include "RelationshipExtraction.h"
//#include "TNode/TNode.h"
//
//using namespace PKB;
//
//RelationshipExtraction::RelationshipExtraction(PKBStorage* pkb) {
//    pkbFacade = pkb;
//}
//
//RelationshipExtraction::~RelationshipExtraction() {}


//void RelationshipExtraction::extractModifyRls(const std::shared_ptr<ProgramNode> astRoot) {
//    for (const auto& proc : astRoot->procList) {
//        extractModifyRls(proc);
//    }
//}
//void RelationshipExtraction::extractModifyRls(const std::shared_ptr<ProcedureNode> proc) {
//    extractModifyStmts(proc->stmtList);
//
//}
//void RelationshipExtraction::extractModifyStmts(const std::vector<Stmt> stmts) {
//    for (const auto& stmt : stmts) {
//        std::visit([this](const auto& s) { extractModifyRls(s); }, stmt);
//    }
//}
//void RelationshipExtraction::extractModifyRls(const std::shared_ptr<AssignmentNode> assign) {
//    extractModifyHelper(assign->var, assign);
//}
//void RelationshipExtraction::extractModifyRls(const std::shared_ptr<PrintNode> print) {}
//void RelationshipExtraction::extractModifyRls(const std::shared_ptr<IfNode> ifNode) {
//    extractModifyStmts(ifNode->thenStmtList);
//    extractModifyStmts(ifNode->elseStmtList);
//}
//void RelationshipExtraction::extractModifyRls(const std::shared_ptr<WhileNode> whileNode) {
//    extractModifyStmts(whileNode->stmtList);
//}
//void RelationshipExtraction::extractModifyRls(const std::shared_ptr<ReadNode> readNode) {
//    extractModifyHelper(readNode->var, readNode);
//}
//void RelationshipExtraction::extractModifyHelper(const std::shared_ptr<VariableNode> var, const Stmt stmt) {
//    const LineNum lnNum = pkbFacade->getLineFromNode(stmt);
//    pkbFacade->storeModifiesS(lnNum, var->varName);
//}
//void RelationshipExtraction::extractModifyRls(const std::shared_ptr<CallNode>) {}
//
//void RelationshipExtraction::extractUsesRls(const std::shared_ptr<ProgramNode> astRoot) {
//    for (const auto& proc : astRoot->procList) {
//        extractUsesRls(proc);
//    }
//}
//void RelationshipExtraction::extractUsesRls(const std::shared_ptr<ProcedureNode> proc) {
//    extractUsesStmts(proc->stmtList);
//
//}
//void RelationshipExtraction::extractUsesStmts(const std::vector<Stmt> stmts) {
//    for (const auto& stmt : stmts) {
//        std::visit([this](const auto& s) { extractUsesRls(s); }, stmt);
//    }
//}
//void RelationshipExtraction::extractUsesRls(const std::shared_ptr<AssignmentNode> assign) {
//    extractUsesHelper(assign->expr, assign);
//}
//void RelationshipExtraction::extractUsesRls(const std::shared_ptr<PrintNode> print) {
//    extractUsesHelper(print->var, print);
//}
//void RelationshipExtraction::extractUsesRls(const std::shared_ptr<IfNode> ifNode) {
//    extractUsesHelper(ifNode->condExpr, ifNode);
//    extractUsesStmts(ifNode->thenStmtList);
//    extractUsesStmts(ifNode->elseStmtList);
//}
//
//void RelationshipExtraction::extractUsesRls(const std::shared_ptr<WhileNode> whileNode) {
//    extractUsesHelper(whileNode->condExpr, whileNode);
//    extractUsesStmts(whileNode->stmtList);
//}
//void RelationshipExtraction::extractUsesRls(const std::shared_ptr<ReadNode> node) {}
//void RelationshipExtraction::extractUsesRls(const std::shared_ptr<CallNode>) {}
//
//
//
//void RelationshipExtraction::extractUsesHelper(const std::shared_ptr<BinOpNode> bin,const Stmt stmt) {
//    extractUsesHelper(bin->leftExpr, stmt);
//    extractUsesHelper(bin->rightExpr, stmt);
//}
//void RelationshipExtraction::extractUsesHelper(const std::shared_ptr<CondExprNode> cond, const Stmt stmt) {
//    if (cond == nullptr) {
//        return;
//    }
//    extractUsesHelper(cond->relExpr, stmt);
//    extractUsesHelper(cond->leftCond, stmt);
//    extractUsesHelper(cond->rightCond, stmt);
//}
//void RelationshipExtraction::extractUsesHelper(const std::shared_ptr<RelExprNode> rel, const Stmt stmt) {
//    if (rel == nullptr) {
//        return;
//    }
//    extractUsesHelper(rel->leftRel, stmt);
//    extractUsesHelper(rel->rightRel, stmt);
//}
//void RelationshipExtraction::extractUsesHelper(const Expr node, const Stmt stmt) {
//    std::visit(
//        [this, stmt](const auto& n) { extractUsesHelper(n, stmt); },
//        node);
//}
//void RelationshipExtraction::extractUsesHelper(const std::shared_ptr<VariableNode> var, const Stmt stmt) {
//    const LineNum lnNum = pkbFacade->getLineFromNode(stmt);
//    pkbFacade->storeUsesS(lnNum, var->varName);
//}
//void RelationshipExtraction::extractUsesHelper(const std::shared_ptr<ConstantNode>cons,const Stmt stmt) {};
//void RelationshipExtraction::extractUsesRls(const std::shared_ptr<WhileNode> whileNode) {
//    extractUsesHelper(whileNode->condExpr, whileNode);
//    extractUsesStmts(whileNode->stmtList);
//}
//void RelationshipExtraction::extractUsesRls(const std::shared_ptr<ReadNode> node) {}
//void RelationshipExtraction::extractUsesRls(const std::shared_ptr<CallNode>) {}
//
//
//
//void RelationshipExtraction::extractFollowsRls(const std::shared_ptr<ProgramNode> astRoot) {
//    for (const auto& proc : astRoot->procList) {
//        extractFollowsRls(proc);
//    }
//}
//void RelationshipExtraction::extractFollowsRls(const std::shared_ptr<ProcedureNode> proc) {
//    extractFollowsStmts(proc->stmtList);
//
//}
//void RelationshipExtraction::extractFollowsStmts(const std::vector<Stmt> stmts) {
//    for (std::size_t i = 0; i < stmts.size() - 1; i++) {
//        const Line lnNum = pkbFacade->getLineFromNode(stmts[i]);
//        const Line nextLn = pkbFacade->getLineFromNode(stmts[i + 1]);
//        pkbFacade->storeFollows(lnNum, nextLn);
//    }
//    for (const auto& stmt : stmts) {
//        std::visit(
//            [this](const auto& s) {
//                using T = std::decay_t<decltype(s)>;
//                if constexpr (std::is_same_v<T, std::shared_ptr<ProcedureNode>> ||
//                    std::is_same_v<T, std::shared_ptr<IfNode>> ||
//                    std::is_same_v<T, std::shared_ptr<WhileNode>>) {
//                    extractFollowsRls(s);
//                }
//            },
//            stmt);
//    }
//}
//
//void RelationshipExtraction::extractFollowsRls(const std::shared_ptr<IfNode> ifNode) {
//    extractFollowsStmts(ifNode->thenStmtList);
//    extractFollowsStmts(ifNode->elseStmtList);
//}
//void RelationshipExtraction::extractFollowsRls(const std::shared_ptr<WhileNode> whileNode) {
//    extractFollowsStmts(whileNode->stmtList);
//}
//
//
//void RelationshipExtraction::extractParentsRls(const std::shared_ptr<ProgramNode> astRoot) {
//    for (const auto& proc : astRoot->procList) {
//        extractParentsRls(proc);
//    }
//}
//
//void RelationshipExtraction::extractParentsRls(const std::shared_ptr<ProcedureNode> proc) {
//    std::vector<ParentLine> parent;
//    extractParentsHelper(proc, parent);
//}
//
//
//void  RelationshipExtraction::extractParentsHelper(const std::shared_ptr<ProcedureNode> proc, const std::vector<ParentLine> parent) {
//    extractParentsStmts(proc->stmtList, parent);
//}
//
//void RelationshipExtraction::extractParentsStmts(const std::vector<Stmt> stmts, const std::vector<ParentLine> parent) {
//    if (parent.size() > 0) {
//        const Line  lastP = parent.back();
//        for (const auto& stmt : stmts) {
//            const Line lnNum = pkbFacade->getLineFromNode(stmt);
//            // add direct relationship
//            pkbFacade->storeParent(lastP, lnNum);
//        }
//    }
//    for (const auto& stmt : stmts) {
//        std::visit(
//            [this, parent](const auto& s) {
//                using T = std::decay_t<decltype(s)>;
//                if constexpr (std::is_same_v<T, std::shared_ptr<ProcedureNode>> ||
//                    std::is_same_v<T, std::shared_ptr<IfNode>> ||
//                    std::is_same_v<T, std::shared_ptr<WhileNode>>) {
//                    extractParentsHelper(s, parent);
//                }
//            },
//            stmt);
//    }
//}
//void RelationshipExtraction::extractParentsHelper( const std::shared_ptr<IfNode> ifNode, std::vector<ParentLine> parent) {
//    const Line lnNum = pkbFacade->getLineFromNode(ifNode);
//    parent.push_back(lnNum);
//    extractParentsStmts(ifNode->thenStmtList, parent);
//    extractParentsStmts(ifNode->elseStmtList, parent);
//}
//
//void RelationshipExtraction::extractParentsHelper(const std::shared_ptr<WhileNode> whileNode, std::vector<ParentLine> parent) {
//    const Line lnNum = pkbFacade->getLineFromNode(whileNode);
//    parent.push_back(lnNum);
//    extractParentsStmts(whileNode->stmtList, parent);
//}
