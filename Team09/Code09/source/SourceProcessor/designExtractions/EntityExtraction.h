#pragma once

#include <memory>
#include "../../TNode/TNode.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"

class EntityExtraction {
private:
    std::shared_ptr<PKB::PKBStorage> pkbStorage;

public:
    explicit EntityExtraction(std::shared_ptr<PKB::PKBStorage> pkbFacade);

    ~EntityExtraction();

    //creating line numbers
    void createLineNumbers(const std::shared_ptr<ProgramNode> astRoot);
    void createLineNumbers(const std::shared_ptr<ProcedureNode> proc);
    void createLineNumbers(const Stmt stmts, const PKB::Procedure procName);
    void traverseLineNumbers(const std::vector<Stmt> stmts, const PKB::Procedure procName);

    //creating CFG
    void createCFG(const std::shared_ptr<ProgramNode> astRoot);
    void createCFG(const std::shared_ptr<ProcedureNode> proc);
    void traverseCFG(const std::vector<Stmt> stmts, std::shared_ptr<std::unordered_map<const PKB::LineNum, std::unordered_set<const PKB::LineNum>>> cache);
    std::unordered_set<const PKB::LineNum> extractTerminatingLines(const std::shared_ptr<IfNode> ifNode, std::shared_ptr<std::unordered_map<const PKB::LineNum, std::unordered_set<const PKB::LineNum>>> cache);
    std::unordered_set<const PKB::LineNum> extractTerminatingLines(const std::shared_ptr<WhileNode> whileNode, std::shared_ptr<std::unordered_map<const PKB::LineNum, std::unordered_set<const PKB::LineNum>>> cache);
    std::unordered_set<const PKB::LineNum> extractTerminatingLines(const std::shared_ptr<ReadNode> readNode, std::shared_ptr<std::unordered_map<const PKB::LineNum, std::unordered_set<const PKB::LineNum>>> cache);
    std::unordered_set<const PKB::LineNum> extractTerminatingLines(const std::shared_ptr<PrintNode> printNode, std::shared_ptr<std::unordered_map<const PKB::LineNum, std::unordered_set<const PKB::LineNum>>> cache);
    std::unordered_set<const PKB::LineNum> extractTerminatingLines(const std::shared_ptr<AssignmentNode> assignNode, std::shared_ptr<std::unordered_map<const PKB::LineNum, std::unordered_set<const PKB::LineNum>>> cache);
    std::unordered_set<const PKB::LineNum> extractTerminatingLines(const std::shared_ptr<CallNode> callNode, std::shared_ptr<std::unordered_map<const PKB::LineNum, std::unordered_set<const PKB::LineNum>>> cache);
    std::unordered_set<const PKB::LineNum> extractTerminatingLines(const Stmt stmt, std::shared_ptr<std::unordered_map<const PKB::LineNum, std::unordered_set<const PKB::LineNum>>> cache);
    void createCFG(const std::shared_ptr<IfNode> ifNode, std::shared_ptr<std::unordered_map<const PKB::LineNum, std::unordered_set<const PKB::LineNum>>> cache);
    void createCFG(const std::shared_ptr<WhileNode> whileNode, std::shared_ptr<std::unordered_map<const PKB::LineNum, std::unordered_set<const PKB::LineNum>>> cache);
    void createCFG(const std::shared_ptr<ReadNode>, std::shared_ptr<std::unordered_map<const PKB::LineNum, std::unordered_set<const PKB::LineNum>>>);
    void createCFG(const std::shared_ptr<PrintNode>, std::shared_ptr<std::unordered_map<const PKB::LineNum, std::unordered_set<const PKB::LineNum>>>);
    void createCFG(const std::shared_ptr<AssignmentNode>, std::shared_ptr<std::unordered_map<const PKB::LineNum, std::unordered_set<const PKB::LineNum>>>);
    void createCFG(const std::shared_ptr<CallNode>, std::shared_ptr<std::unordered_map<const PKB::LineNum, std::unordered_set<const PKB::LineNum>>>);
  
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
    void extractEntities(const std::shared_ptr<CallNode> callNode);
    void extractEntities(const Expr exp);

    //extracting Modifies relations
    void extractModifyRls(const std::shared_ptr<ProgramNode> astRoot);
    void extractIndirectModifyRls();
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
    void extractIndirectUsesRls();
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
    void extractParentsHelper(const std::shared_ptr<ProcedureNode> proc, const std::vector<PKB::ParentLine> parent);
    void extractParentsStmts(const std::vector<Stmt> stmts, const std::vector<PKB::ParentLine> parent);
    void extractParentsHelper(const std::shared_ptr<IfNode> ifNode, std::vector<PKB::ParentLine> parent);
    void extractParentsHelper(const std::shared_ptr<WhileNode> whileNode, std::vector<PKB::ParentLine> parent);

    //extracting Calls relations
    void extractCallsRls(const std::shared_ptr<ProgramNode> astRoot);
    void extractCallsRls(const std::shared_ptr<ProcedureNode> proc);
    void extractCallsStmts(const std::vector<Stmt> stmts);
    void extractCallsRls(const std::shared_ptr<IfNode> ifNode);
    void extractCallsRls(const std::shared_ptr<WhileNode> whileNode);
    void extractCallsRls(const std::shared_ptr<ReadNode>);
    void extractCallsRls(const std::shared_ptr<PrintNode>);
    void extractCallsRls(const std::shared_ptr<AssignmentNode>);
    void extractCallsRls(const std::shared_ptr<CallNode> node);
    void extractCallsTRls();

    //extracting Assign pattern relations
    void extractAssignPattern(const std::shared_ptr<ProgramNode> astRoot);
    void extractAssignPattern(const std::shared_ptr<ProcedureNode> proc);
    void extractAssignStmts(const std::vector<Stmt> stmts);
    void extractAssignPattern(const std::shared_ptr<AssignmentNode> assign);
    void extractAssignPattern(const std::shared_ptr<IfNode> ifNode);
    void extractAssignPattern(const std::shared_ptr<WhileNode> whileNode);
    void extractAssignPattern(const std::shared_ptr<ReadNode>);
    void extractAssignPattern(const std::shared_ptr<CallNode>);
    void extractAssignPattern(const std::shared_ptr<PrintNode>);
    void extractProcStartAndEnd(const std::shared_ptr<ProgramNode> astRoot);
};
