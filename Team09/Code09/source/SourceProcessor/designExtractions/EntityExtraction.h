#pragma once

#include <memory>
#include "TNode/TNode.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"

using namespace PKB;

class EntityExtraction {
private:
    PKBStorage* pkbFacade;

public:
    explicit EntityExtraction(PKBStorage* pkb);

    ~EntityExtraction();

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
};