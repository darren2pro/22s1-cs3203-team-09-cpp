#pragma once

#include "Visitor.h"

class SetLineNumberVisitor : public Visitor {
private:
    int lineNum = 1;
    PKB::Procedure proc;

    PKB::LineNum getCurrLineNumber();
    void incrementCurrLineNumber();

    PKB::LineNum storeLine(Stmt node);
    void storeLineToProcedure(const PKB::LineNum lineNum, const PKB::Procedure proc);
public:
    using Visitor::Visitor;
    virtual void visitProgramNode(std::shared_ptr<ProgramNode> node) override;
    virtual void visitProcedureNode(std::shared_ptr<ProcedureNode> node) override;
    virtual void visitAssignmentNode(std::shared_ptr<AssignmentNode> node) override;
    virtual void visitReadNode(std::shared_ptr<ReadNode> node) override;
    virtual void visitPrintNode(std::shared_ptr<PrintNode> node) override;
    virtual void visitCallNode(std::shared_ptr<CallNode> node) override;
    virtual void visitIfNode(std::shared_ptr<IfNode> node) override;
    virtual void visitWhileNode(std::shared_ptr<WhileNode> node) override;
};