#pragma once

#include "Visitor.h"

class ExtractNextVisitor : public Visitor {
private:
    std::shared_ptr<std::unordered_map<PKB::LineNum, std::unordered_set<PKB::LineNum>>> cache;
    void traverseStmtList(StmtLst, Visitor*);
    const std::unordered_set<PKB::LineNum> getTerminatingLines(Stmt stmt, std::shared_ptr<std::unordered_map<PKB::LineNum, std::unordered_set<PKB::LineNum>>> cache);
    const std::unordered_set<PKB::LineNum> getIfTerminatingLines(std::shared_ptr<IfNode> ifNode, std::shared_ptr<std::unordered_map<PKB::LineNum, std::unordered_set<PKB::LineNum>>> cache);
    const std::unordered_set<PKB::LineNum> getOthersTerminatingLines(Stmt node, std::shared_ptr<std::unordered_map<PKB::LineNum, std::unordered_set<PKB::LineNum>>> cache);
public:
    using Visitor::Visitor;
    virtual void visitProgramNode(std::shared_ptr<ProgramNode> node) override;
    virtual void visitProcedureNode(std::shared_ptr<ProcedureNode> node) override;
    virtual void visitIfNode(std::shared_ptr<IfNode> node) override;
    virtual void visitWhileNode(std::shared_ptr<WhileNode> node) override;
};