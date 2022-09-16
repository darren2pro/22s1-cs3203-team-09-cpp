#pragma once

#include <memory>
#include "TNode/TNode.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"

using namespace PKB;

class EntityExtraction {
private:
    std::shared_ptr<PKBStorage> pkbFacade;

public:
    explicit EntityExtraction(std::shared_ptr<PKBStorage> pkbFacade);

    ~EntityExtraction();

    //creating line numbers
    void createLineNumbers(const std::shared_ptr<ProgramNode> astRoot);
    void createLineNumbers(const std::shared_ptr<ProcedureNode> proc);
    void createLineNumbers(const Stmt stmts);
    void traverseLineNumbers(const std::vector<Stmt> stmts);

    //entity design extractions from ast
    void extractEntities(const std::shared_ptr<ProgramNode> astRoot);
    void extractEntities(const std::shared_ptr<ProcedureNode> proc);
    void extractStatements(const std::vector<Stmt> stmts);
    void extractEntities(const std::shared_ptr<AssignmentNode> assign);
    void extractEntities(const std::shared_ptr<CondExprNode> cond);
    void extractEntities(const std::shared_ptr<RelExprNode> rel);
    void extractEntities(const std::shared_ptr<BinOpNode> bin);
    void extractEntities(const std::shared_ptr<VariableNode> var);
    void extractEntities(const std::shared_ptr<ConstantNode> cons);
    void extractEntities(const std::shared_ptr<IfNode> ifNode);
    void extractEntities(const std::shared_ptr<WhileNode> whileNode);
    void extractEntities(const std::shared_ptr<ReadNode> readNode);
    void extractEntities(const std::shared_ptr<PrintNode> printNode);
    void extractEntities(const std::shared_ptr<CallNode> node);
    void extractEntities(const Expr exp);

    //extracting Modifies relations
    void extractModifyRls(const std::shared_ptr<ProgramNode> astRoot);
    void extractModifyRls(const std::shared_ptr<ProcedureNode> proc);
    void extractModifyStmts(const std::vector<Stmt> stmts);
    void extractModifyRls(const std::shared_ptr<AssignmentNode> assign);
    void extractModifyRls(const std::shared_ptr<PrintNode> print);
    void extractModifyRls(const std::shared_ptr<IfNode> ifNode);
    void extractModifyRls(const std::shared_ptr<WhileNode> whileNode);
    void extractModifyRls(const std::shared_ptr<ReadNode> readNode);
    void extractModifyRls(const std::shared_ptr<CallNode>);
    void extractModifyHelper(const std::shared_ptr<VariableNode> var, const Stmt stmt);

    //extracting Uses relations
    void extractUsesRls(const std::shared_ptr<ProgramNode> astRoot);
    void extractUsesRls(const std::shared_ptr<ProcedureNode> proc);
    void extractUsesStmts(const std::vector<Stmt> stmts);
    void extractUsesRls(const std::shared_ptr<AssignmentNode> assign);
    void extractUsesRls(const std::shared_ptr<PrintNode> print);
    void extractUsesRls(const std::shared_ptr<IfNode> ifNode);
    void extractUsesRls(const std::shared_ptr<WhileNode> whileNode);
    void extractUsesRls(const std::shared_ptr<ReadNode> readNode);
    void extractUsesRls(const std::shared_ptr<CallNode>);

    void extractUsesHelper(const std::shared_ptr<BinOpNode> bin, const Stmt stmt);
    void extractUsesHelper(const std::shared_ptr<CondExprNode> cond, const Stmt stmt);
    void extractUsesHelper(const std::shared_ptr<RelExprNode> rel, const Stmt stmt);
    void extractUsesHelper(const Expr node, const Stmt stmt);
    void extractUsesHelper(const std::shared_ptr<VariableNode> var, const Stmt stmt);
    void extractUsesHelper(const std::shared_ptr<ConstantNode>cons, const Stmt stmt) {};

    //extracting Follows relations

    void extractFollowsRls(const std::shared_ptr<ProgramNode> astRoot);
    void extractFollowsRls(const std::shared_ptr<ProcedureNode> proc);
    void extractFollowsStmts(const std::vector<Stmt> stmts);
    void extractFollowsRls(const std::shared_ptr<IfNode> ifNode);
    void extractFollowsRls(const std::shared_ptr<WhileNode> whileNode);

    //extracting Parent relations

    void extractParentsRls(const std::shared_ptr<ProgramNode> astRoot);
    void extractParentsRls(const std::shared_ptr<ProcedureNode> proc);
    void extractParentsRls(const std::shared_ptr<IfNode> ifNode);
    void extractParentsRls(const std::shared_ptr<WhileNode> whileNode);
    void extractParentsHelper(const std::shared_ptr<ProcedureNode> proc, const std::vector<ParentLine> parent);
    void extractParentsStmts(const std::vector<Stmt> stmts, const std::vector<ParentLine> parent);
    void extractParentsHelper(const std::shared_ptr<IfNode> ifNode, std::vector<ParentLine> parent);
    void extractParentsHelper(const std::shared_ptr<WhileNode> whileNode, std::vector<ParentLine> parent);

};