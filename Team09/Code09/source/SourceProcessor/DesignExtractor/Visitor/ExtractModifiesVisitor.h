#pragma once

#include "Visitor.h"

class ExtractModifiesVisitor : public Visitor {
private:
    Stmt stmt;
    void extractIndirectModifies();
public:
    using Visitor::Visitor;
    virtual void visitProgramNode(std::shared_ptr<ProgramNode> node) override;
    virtual void visitProcedureNode(std::shared_ptr<ProcedureNode> node) override;
    virtual void visitIfNode(std::shared_ptr<IfNode> node) override;
    virtual void visitWhileNode(std::shared_ptr<WhileNode> node) override;
    virtual void visitAssignmentNode(std::shared_ptr<AssignmentNode> node) override;
    virtual void visitReadNode(std::shared_ptr<ReadNode> node) override;
    virtual void visitVariableNode(std::shared_ptr<VariableNode> node) override;
};