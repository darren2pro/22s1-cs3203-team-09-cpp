#include "TNode.h"

ProgramNode::ProgramNode(std::vector<ProcedureNodePtr> procList) :
        procList(std::move(procList)) {};

ProcedureNode::ProcedureNode(const std::string procName, StmtLst stmtList) :
    procName(procName), stmtList(std::move(stmtList)) {};

ConstantNode::ConstantNode(const std::string value) : value(value) {};

VariableNode::VariableNode(const std::string varName) : varName(varName) {};

AssignmentNode::AssignmentNode(VariableNodePtr var, Expr expr) :
    var(std::move(var)) {};

CallNode::CallNode(ProcedureNodePtr proc) : proc(std::move(proc)) {};

PrintNode::PrintNode(VariableNodePtr var) : var(std::move(var)) {};

ReadNode::ReadNode(VariableNodePtr var) : var(std::move(var)) {};

IfNode::IfNode(CondExprNodePtr condExpr, StmtLst thenStmtList,
               StmtLst elseStmtList) : condExpr(std::move(condExpr)),
                                       thenStmtList(std::move(thenStmtList)), elseStmtList(std::move(elseStmtList)) {};

WhileNode::WhileNode(CondExprNodePtr condExpr, StmtLst stmtList) :
        condExpr(std::move(condExpr)), stmtList(std::move(stmtList)) {};

BinOpNode::BinOpNode(std::string op, Expr leftExpr, Expr rightExpr) :
        op(op), leftExpr(std::move(leftExpr)), rightExpr(std::move(rightExpr)) {};

CondExprNode::CondExprNode(RelExprNodePtr relExpr) : relExpr(std::move(relExpr)) {};

CondExprNode::CondExprNode(CondExprNodePtr leftCond) :
        op("!"), leftCond(std::move(leftCond)) {};

CondExprNode::CondExprNode(std::string op, CondExprNodePtr leftCond,
                           CondExprNodePtr rightCond) : op(op), leftCond(std::move(leftCond)),
                                                                      rightCond(std::move(rightCond)) {};

RelExprNode::RelExprNode(std::string op, Expr leftRel, Expr rightRel) :
        op(op), leftRel(std::move(leftRel)), rightRel(std::move(rightRel)) {};
