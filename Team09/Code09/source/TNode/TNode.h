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

// TODO: Replace the occurrences
using ProgramNodePtr = std::shared_ptr<ProgramNode>;
using ProcedureNodePtr = std::shared_ptr<ProcedureNode>;
using ConstantNodePtr = std::shared_ptr<ConstantNode>;
using VariableNodePtr = std::shared_ptr<VariableNode>;
using AssignmentNodePtr = std::shared_ptr<AssignmentNode>;
using CallNodePtr = std::shared_ptr<CallNode>;
using PrintNodePtr = std::shared_ptr<PrintNode>;
using ReadNodePtr = std::shared_ptr<ReadNode>;
using IfNodePtr = std::shared_ptr<IfNode>;
using WhileNodePtr = std::shared_ptr<WhileNode>;
using BinOpNodePtr = std::shared_ptr<BinOpNode>;
using CondExprNodePtr = std::shared_ptr<CondExprNode>;
using RelExprNodePtr = std::shared_ptr<RelExprNode>;

using Stmt = std::variant<std::shared_ptr<AssignmentNode>, std::shared_ptr<CallNode>,
                          std::shared_ptr<PrintNode>, std::shared_ptr<ReadNode>,
                          std::shared_ptr<IfNode>, std::shared_ptr<WhileNode>>;

using Expr = std::variant<std::shared_ptr<VariableNode>,
                          std::shared_ptr<ConstantNode>,
                          std::shared_ptr<BinOpNode>>;

using Factor = std::variant<std::shared_ptr<VariableNode>,
                            std::shared_ptr<ConstantNode>,
                            std::shared_ptr<BinOpNode>>;

using StmtLst = std::vector<Stmt>;

using AST = std::shared_ptr<ProgramNode>;

class TNode {
    public:
        virtual ~TNode() = default;
};

class ProgramNode : public TNode {
    public:
        std::vector<std::shared_ptr<ProcedureNode>> procList;
        explicit ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procList);
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
        std::shared_ptr<VariableNode> var;
        Expr expr;
        explicit AssignmentNode(const std::shared_ptr<VariableNode> var, Expr expr);
};

class CallNode : public TNode {
    public:
        std::shared_ptr<ProcedureNode> proc;
        explicit CallNode(std::shared_ptr<ProcedureNode> proc);
};

class PrintNode : public TNode {
    public:
        std::shared_ptr<VariableNode> var;
        explicit PrintNode(std::shared_ptr<VariableNode> var);
};

class ReadNode : public TNode {
    public:
        std::shared_ptr<VariableNode> var;
        explicit ReadNode(std::shared_ptr<VariableNode> var);
};

class IfNode : public TNode {
    public:
        std::shared_ptr<CondExprNode> condExpr;
        StmtLst thenStmtList;
        StmtLst elseStmtList;
        explicit IfNode(std::shared_ptr<CondExprNode> condExpr,
            StmtLst thenStmtList, StmtLst elseStmtList);
};

class WhileNode : public TNode {
    public:
        std::shared_ptr<CondExprNode> condExpr;
        StmtLst stmtList;
        explicit WhileNode(std::shared_ptr<CondExprNode> condExpr, StmtLst stmtList);
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
        std::shared_ptr<RelExprNode> relExpr = nullptr;
        std::shared_ptr<CondExprNode> leftCond = nullptr;
        std::shared_ptr<CondExprNode> rightCond = nullptr;
        
        // rel_expr
        explicit CondExprNode(std::shared_ptr<RelExprNode> relExpr);

        // !(cond_expr)
        explicit CondExprNode(std::shared_ptr<CondExprNode> leftCond);

        // (cond_expr) op (cond_expr)
        explicit CondExprNode(std::string op,
            std::shared_ptr<CondExprNode> leftCond, std::shared_ptr<CondExprNode> rightCond);
};

class RelExprNode : public TNode {
    public:
        std::string op;
        Expr leftRel;
        Expr rightRel;
        explicit RelExprNode(std::string op, Expr leftRel, Expr rightRel);
};
