#pragma once

#include "Visitor.h"

class ExtractCallsVisitor : public Visitor {
private:
    void extractCallsT();
public:
    using Visitor::Visitor;
    virtual void visitProgramNode(std::shared_ptr<ProgramNode> node) override;
    virtual void visitProcedureNode(std::shared_ptr<ProcedureNode> node) override;
    virtual void visitIfNode(std::shared_ptr<IfNode> node) override;
    virtual void visitWhileNode(std::shared_ptr<WhileNode> node) override;
    virtual void visitCallNode(std::shared_ptr<CallNode> node) override;
};