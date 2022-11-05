#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

class Visitor;
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
    virtual bool operator==(const TNode& other) const = 0;
    virtual std::string toString() const = 0;
    virtual void accept(Visitor* v) = 0;
};

class ProgramNode : public TNode, public std::enable_shared_from_this<ProgramNode> {
public:
    std::vector<ProcedureNodePtr> procList;
    explicit ProgramNode(std::vector<ProcedureNodePtr> procList);
    bool operator==(const TNode& other) const override;
    std::string toString() const override;
    void accept(Visitor* v) override;
};

class ProcedureNode : public TNode, public std::enable_shared_from_this<ProcedureNode> {
public:
    std::string procName;
    StmtLst stmtList;
    explicit ProcedureNode(const std::string procName, StmtLst stmtList);
    bool operator==(const TNode& other) const override;
    std::string toString() const override;
    void accept(Visitor* v) override;
};

class ConstantNode : public TNode, public std::enable_shared_from_this<ConstantNode> {
public:
    std::string value;
    explicit ConstantNode(const std::string value);
    bool operator==(const TNode& other) const override;
    std::string toString() const override;
    void accept(Visitor* v) override;
};

class VariableNode : public TNode, public std::enable_shared_from_this<VariableNode> {
public:
    std::string varName;
    explicit VariableNode(const std::string varName);
    bool operator==(const TNode& other) const override;
    std::string toString() const override;
    void accept(Visitor* v) override;
};

class AssignmentNode : public TNode, public std::enable_shared_from_this<AssignmentNode> {
public:
    VariableNodePtr var;
    Expr expr;
    explicit AssignmentNode(VariableNodePtr var, Expr expr);
    bool operator==(const TNode& other) const override;
    std::string toString() const override;
    void accept(Visitor* v) override;
};

class CallNode : public TNode, public std::enable_shared_from_this<CallNode> {
public:
    ProcedureNodePtr proc;
    explicit CallNode(ProcedureNodePtr proc);
    bool operator==(const TNode& other) const override;
    std::string toString() const override;
    void accept(Visitor* v) override;
};

class PrintNode : public TNode, public std::enable_shared_from_this<PrintNode> {
public:
    VariableNodePtr var;
    explicit PrintNode(VariableNodePtr var);
    bool operator==(const TNode& other) const override;
    std::string toString() const override;
    void accept(Visitor* v) override;
};

class ReadNode : public TNode, public std::enable_shared_from_this<ReadNode> {
public:
    VariableNodePtr var;
    explicit ReadNode(VariableNodePtr var);
    bool operator==(const TNode& other) const override;
    std::string toString() const override;
    void accept(Visitor* v) override;
};

class IfNode : public TNode, public std::enable_shared_from_this<IfNode> {
public:
    CondExprNodePtr condExpr;
    StmtLst thenStmtList;
    StmtLst elseStmtList;
    explicit IfNode(CondExprNodePtr condExpr,
                    StmtLst thenStmtList, StmtLst elseStmtList);
    bool operator==(const TNode& other) const override;
    std::string toString() const override;
    void accept(Visitor* v) override;
};

class WhileNode : public TNode, public std::enable_shared_from_this<WhileNode> {
public:
    CondExprNodePtr condExpr;
    StmtLst stmtList;
    explicit WhileNode(CondExprNodePtr condExpr, StmtLst stmtList);
    bool operator==(const TNode& other) const override;
    std::string toString() const override;
    void accept(Visitor* v) override;
};

class BinOpNode : public TNode, public std::enable_shared_from_this<BinOpNode> {
public:
    std::string op;
    Expr leftExpr;
    Expr rightExpr;
    explicit BinOpNode(std::string op, Expr leftExpr, Expr rightExpr);
    bool operator==(const TNode& other) const override;
    std::string toString() const override;
    void accept(Visitor* v) override;
};

class CondExprNode : public TNode, public std::enable_shared_from_this<CondExprNode> {
public:
    //! Simple relational expr e.g. (1 >= 6 % 4)
    RelExprNodePtr relExpr = nullptr;

    //! For () && ()
    std::string op = "";
    CondExprNodePtr leftCond = nullptr;
    CondExprNodePtr rightCond = nullptr;

    // rel_expr
    explicit CondExprNode(RelExprNodePtr relExpr);

    // !(cond_expr)
    explicit CondExprNode(CondExprNodePtr leftCond);

    // (cond_expr) op (cond_expr)
    explicit CondExprNode(std::string op,
                          CondExprNodePtr leftCond, CondExprNodePtr rightCond);

    bool operator==(const TNode& other) const override;
    std::string toString() const override;
    void accept(Visitor* v) override;
};

class RelExprNode : public TNode, public std::enable_shared_from_this<RelExprNode> {
public:
    std::string op;
    Expr leftRel;
    Expr rightRel;
    explicit RelExprNode(std::string op, Expr leftRel, Expr rightRel);
    bool operator==(const TNode& other) const override;
    std::string toString() const override;
    void accept(Visitor* v) override;
};
