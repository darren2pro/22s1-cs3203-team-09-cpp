//#pragma once
//
//#include "TNode/TNode.h"
//#include "../../ProgramKnowledgeBase/PKBStorage.h"
//
//using namespace PKB;
//
//class RelationshipExtraction {
//private:
//    PKBStorage* pkbFacade;
//
//public:
//    explicit RelationshipExtraction(PKBStorage* pkb);
//
//    ~RelationshipExtraction();
//

    /*void extractModifyRls(const std::shared_ptr<ProgramNode> astRoot);
    void extractModifyRls(const std::shared_ptr<ProcedureNode> node);
    void extractModifyStmts(const std::vector<Stmt> stmts);
    void extractModifyRls(const std::shared_ptr<AssignmentNode> assign);
    void extractModifyRls(const std::shared_ptr<PrintNode> print);
    void extractModifyRls(const std::shared_ptr<IfNode> ifNode);
    void extractModifyRls(const std::shared_ptr<WhileNode> whileNode);
    void extractModifyRls(const std::shared_ptr<ReadNode> node);
    void extractModifyRls(const std::shared_ptr<CallNode>);
    void extractModifyHelper(const std::shared_ptr<VariableNode> var, const Stmt stmt);

    void extractUsesRls(const std::shared_ptr<ProgramNode> astRoot);
    void extractUsesRls(const std::shared_ptr<ProcedureNode> node);
    void extractUsesStmts(const std::vector<Stmt> stmts);
    void extractUsesRls(const std::shared_ptr<AssignmentNode> assign);
    void extractUsesRls(const std::shared_ptr<PrintNode> print);
    void extractUsesRls(const std::shared_ptr<IfNode> ifNode);
    void extractUsesRls(const std::shared_ptr<WhileNode> whileNode);
    void extractUsesRls(const std::shared_ptr<ReadNode> node);
    void extractUsesRls(const std::shared_ptr<CallNode>);

    void extractUsesHelper(const std::shared_ptr<BinOpNode> bin, const Stmt stmt);
    void extractUsesHelper(const std::shared_ptr<CondExprNode> cond, const Stmt stmt);
    void extractUsesHelper(const std::shared_ptr<RelExprNode> rel, const Stmt stmt);
    void extractUsesHelper(const Expr node, const Stmt stmt);
    void extractUsesHelper(const std::shared_ptr<VariableNode> node, const Stmt stmt);
    void extractUsesHelper(const std::shared_ptr<ConstantNode>cons, const Stmt stmt) {};

    void extractFollowsRls(const std::shared_ptr<ProgramNode> astRoot);
    void extractFollowsRls(const std::shared_ptr<ProcedureNode> proc);
    void extractFollowsStmts(const std::vector<Stmt> stmts);
    void extractFollowsRls(const std::shared_ptr<IfNode> ifNode);
    void extractFollowsRls(const std::shared_ptr<WhileNode> whileNode);

    void extractRls(TNode::PROGRAM_NODE_PTR astRoot);
    void extractRls(TNode::PROCEDURE_NODE_PTR proc);
    void extractRls(TNode::ASSIGNMENT_NODE_PTR assign, TNode::PROCEDURE_NODE_PTR proc);
};
