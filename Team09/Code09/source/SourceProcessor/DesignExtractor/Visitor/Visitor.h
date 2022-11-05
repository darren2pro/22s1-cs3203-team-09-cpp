#pragma once

#include "../../TNode/TNode.h"
#include "../../../ProgramKnowledgeBase/PKBStorage.h"
#include "../../../ProgramKnowledgeBase/PKBUtils.h"

using namespace PKB;

class Visitor {
public:
    PKB::PKBStorage* pkbStorage;

    static std::unordered_map<std::shared_ptr<TNode>, PKB::LineNum> nodeToLineMap;
    static std::unordered_map<PKB::LineNum, PKB::Procedure> lineToProcMap;
    static std::unordered_set<std::pair<PKB::LineNum, PKB::Procedure>, PKB::pairHash> lineCallsProcSet;

    PKB::LineNum getLineFromNode(Stmt node);
    PKB::Procedure getProcedureFromLine(const PKB::LineNum lineNum);
    std::unordered_set<std::pair<PKB::LineNum, PKB::Procedure>, PKB::pairHash> getLineCallsProc();

    Visitor(PKB::PKBStorage* pkb);
    ~Visitor();
    void reset();

    virtual void visitProgramNode(std::shared_ptr<ProgramNode>);
    virtual void visitProcedureNode(std::shared_ptr<ProcedureNode>);
    virtual void visitConstantNode(std::shared_ptr<ConstantNode>);
    virtual void visitVariableNode(std::shared_ptr<VariableNode>);
    virtual void visitAssignmentNode(std::shared_ptr<AssignmentNode>);
    virtual void visitPrintNode(std::shared_ptr<PrintNode>);
    virtual void visitCallNode(std::shared_ptr<CallNode>);
    virtual void visitReadNode(std::shared_ptr<ReadNode>);
    virtual void visitIfNode(std::shared_ptr<IfNode>);
    virtual void visitWhileNode(std::shared_ptr<WhileNode>);
    virtual void visitBinOpNode(std::shared_ptr<BinOpNode>);
    virtual void visitCondExprNode(std::shared_ptr<CondExprNode>);
    virtual void visitRelExprNode(std::shared_ptr<RelExprNode>);
    void visitStmtList(StmtLst, Visitor*);
    void visitExpr(Expr, Visitor*);
};