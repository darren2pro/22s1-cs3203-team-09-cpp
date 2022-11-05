#pragma once

#include "Visitor.h"

class ExtractPatternVisitor : public Visitor {
private:
    Stmt stmt;
public:
    using Visitor::Visitor;
    virtual void visitProgramNode(std::shared_ptr<ProgramNode> node) override;
    virtual void visitProcedureNode(std::shared_ptr<ProcedureNode> node) override;
    virtual void visitIfNode(std::shared_ptr<IfNode> node) override;
    virtual void visitWhileNode(std::shared_ptr<WhileNode> node) override;
    virtual void visitAssignmentNode(std::shared_ptr<AssignmentNode> node) override;
    virtual void visitVariableNode(std::shared_ptr<VariableNode> node) override;
    virtual void visitBinOpNode(std::shared_ptr<BinOpNode> node) override;
    virtual void visitCondExprNode(std::shared_ptr<CondExprNode> node) override;
    virtual void visitRelExprNode(std::shared_ptr<RelExprNode> node) override;
};