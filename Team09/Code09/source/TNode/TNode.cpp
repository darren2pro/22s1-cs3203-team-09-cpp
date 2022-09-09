#include "TNode.h"

ProgramNode::ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procList) :
        procList(std::move(procList)) {};

ProcedureNode::ProcedureNode(const std::string procName, Stmt stmtList) :
        procName(procName), stmtList(std::move(stmtList)) {};

ConstantNode::ConstantNode(const std::string value) : value(value) {};

VariableNode::VariableNode(const std::string varName) : varName(varName) {};

AssignmentNode::AssignmentNode(const std::shared_ptr<VariableNode> var, Expr expr) :
        var(std::move(var)) {};

CallNode::CallNode(std::shared_ptr<ProcedureNode> proc) : proc(std::move(proc)) {};

PrintNode::PrintNode(std::shared_ptr<VariableNode> var) : var(std::move(var)) {};

ReadNode::ReadNode(std::shared_ptr<VariableNode> var) : var(std::move(var)) {};

IfNode::IfNode(std::shared_ptr<CondExprNode> condExpr, StmtLst thenStmtList,
               StmtLst elseStmtList) : condExpr(std::move(condExpr)),
                                       thenStmtList(std::move(thenStmtList)), elseStmtList(std::move(elseStmtList)) {};

WhileNode::WhileNode(std::shared_ptr<CondExprNode> condExpr, StmtLst stmtList) :
        condExpr(std::move(condExpr)), stmtList(std::move(stmtList)) {};

BinOpNode::BinOpNode(std::string op, Expr leftExpr, Expr rightExpr) :
        op(op), leftExpr(std::move(leftExpr)), rightExpr(std::move(rightExpr)) {};

CondExprNode::CondExprNode(std::shared_ptr<RelExprNode> relExpr) : relExpr(std::move(relExpr)) {};

CondExprNode::CondExprNode(std::shared_ptr<CondExprNode> leftCond) :
        op("!"), leftCond(std::move(leftCond)) {};

CondExprNode::CondExprNode(std::string op, std::shared_ptr<CondExprNode> leftCond,
                           std::shared_ptr<CondExprNode> rightCond) : op(op), leftCond(std::move(leftCond)),
                                                                      rightCond(std::move(rightCond)) {};

RelExprNode::RelExprNode(std::string op, Expr leftRel, Expr rightRel) :
        op(op), leftRel(std::move(leftRel)), rightRel(std::move(rightRel)) {};
