#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

class TNode;
class ProgramNode;
class ProcedureNode;
class ConstantNode;
class VariableNode;
class AssignmentNode;
class CallNode;
class PrintNode;
class ReadNode;
class IfNode;
class WhileNode;
class BinOpNode;
class CondExprNode;
class RelExprNode;

typedef std::shared_ptr<ProgramNode> AST;
typedef std::shared_ptr<ProcedureNode> ProcedureNodePtr;
typedef std::shared_ptr<ConstantNode> ConstantNodePtr;
typedef std::shared_ptr<VariableNode> VariableNodePtr;
typedef std::shared_ptr<AssignmentNode> AssignmentNodePtr;
typedef std::shared_ptr<CallNode> CallNodePtr;
typedef std::shared_ptr<PrintNode> PrintNodePtr;
typedef std::shared_ptr<ReadNode> ReadNodePtr;
typedef std::shared_ptr<IfNode> IfNodePtr;
typedef std::shared_ptr<WhileNode> WhileNodePtr;
typedef std::shared_ptr<BinOpNode> BinOpNodePtr;
typedef std::shared_ptr<CondExprNode> CondExprNodePtr;
typedef std::shared_ptr<RelExprNode> RelExprNodePtr;

typedef std::variant<AssignmentNodePtr, CallNodePtr,
        PrintNodePtr, ReadNodePtr,
        IfNodePtr, WhileNodePtr> Stmt;

typedef std::vector<Stmt> StmtLst;

typedef std::variant<VariableNodePtr,
        ConstantNodePtr,
        BinOpNodePtr> Expr;

typedef std::variant<VariableNodePtr,
        ConstantNodePtr,
        BinOpNodePtr> Factor;

typedef Factor RelFactor;

class TNode {
public:
    virtual ~TNode() = default;
};

class ProgramNode : public TNode {
public:
    std::vector<ProcedureNodePtr> procList;

    explicit ProgramNode(std::vector<ProcedureNodePtr> procList);
};

class ProcedureNode : public TNode {
public:
    std::string procName;
    StmtLst stmtList;

    explicit ProcedureNode(const std::string procName, Stmt stmtList);
};

class ConstantNode : public TNode {
public:
    std::string value;

    explicit ConstantNode(const std::string value);
};

class VariableNode : public TNode {
public:
    std::string varName;

    explicit VariableNode(const std::string varName);
};

class AssignmentNode : public TNode {
public:
    VariableNodePtr var;
    Expr expr;

    explicit AssignmentNode(const VariableNodePtr var, Expr expr);
};

class CallNode : public TNode {
public:
    ProcedureNodePtr proc;

    explicit CallNode(ProcedureNodePtr proc);
};

class PrintNode : public TNode {
public:
    VariableNodePtr var;

    explicit PrintNode(VariableNodePtr var);
};

class ReadNode : public TNode {
public:
    VariableNodePtr var;

    explicit ReadNode(VariableNodePtr var);
};

class IfNode : public TNode {
public:
    CondExprNodePtr condExpr;
    StmtLst thenStmtList;
    StmtLst elseStmtList;

    explicit IfNode(CondExprNodePtr condExpr,
                    StmtLst thenStmtList, StmtLst elseStmtList);
};

class WhileNode : public TNode {
public:
    CondExprNodePtr condExpr;
    StmtLst stmtList;

    explicit WhileNode(CondExprNodePtr condExpr, StmtLst stmtList);
};

class BinOpNode : public TNode {
public:
    std::string op;
    Expr leftExpr;
    Expr rightExpr;

    explicit BinOpNode(std::string op, Expr leftExpr, Expr rightExpr);
};

class CondExprNode : public TNode {
public:
    std::string op = "";
    RelExprNodePtr relExpr = nullptr;
    CondExprNodePtr leftCond = nullptr;
    CondExprNodePtr rightCond = nullptr;

    // rel_expr
    explicit CondExprNode(RelExprNodePtr relExpr);

    // !(cond_expr)
    explicit CondExprNode(CondExprNodePtr leftCond);

    // (cond_expr) op (cond_expr)
    explicit CondExprNode(std::string op,
                          CondExprNodePtr leftCond, CondExprNodePtr rightCond);
};

class RelExprNode : public TNode {
public:
    std::string op;
    Expr leftRel;
    Expr rightRel;

    explicit RelExprNode(std::string op, Expr leftRel, Expr rightRel);
};
