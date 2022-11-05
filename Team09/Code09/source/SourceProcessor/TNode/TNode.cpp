#include "TNode.h"
#include "../DesignExtractor/Visitor/Visitor.h"

ProgramNode::ProgramNode(std::vector<ProcedureNodePtr> procList) :
        procList(std::move(procList)) {}

bool ProgramNode::operator==(const TNode& other) const {
    const ProgramNode* castedOther = dynamic_cast<const ProgramNode*>(&other);
    bool canCast = castedOther != 0;
    if (!canCast) return false;
    bool equalProcedures = std::equal((this->procList).begin(), (this->procList).end(),
                                      (castedOther->procList).begin(), (castedOther->procList).end(),
                                      [](const ProcedureNodePtr _this, const ProcedureNodePtr _other) {
                                          return *_this == *_other;
                                      });
    return canCast && equalProcedures;
}

std::string ProgramNode::toString() const {
    std::string result = "ProgramNode(\n";
    for (auto proc: procList) {
        result += proc->toString() + ", ";
    }
    result += ")";
    return result;
};

void ProgramNode::accept(Visitor* v) {
    std::shared_ptr<ProgramNode> node{shared_from_this()};
    v->visitProgramNode(node);
}

ProcedureNode::ProcedureNode(const std::string procName, StmtLst stmtList) :
        procName(procName), stmtList(std::move(stmtList)) {}

bool ProcedureNode::operator==(const TNode& other) const {
    const ProcedureNode* castedOther = dynamic_cast<const ProcedureNode*>(&other);
    bool canCast = castedOther != 0;
    if (!canCast) return false;
    bool equalProcName = this->procName == castedOther->procName;
    bool equalStatements = std::equal((this->stmtList).begin(), (this->stmtList).end(),
                                      (castedOther->stmtList).begin(), (castedOther->stmtList).end(),
                                      [](const Stmt& _this, const Stmt& _other) {
                                          return std::visit([](const auto& _tp, const auto& _op) {
                                              return *_tp == *_op;
                                          }, _this, _other);
                                      });
    return canCast && equalProcName && equalStatements;
}

std::string ProcedureNode::toString() const {
    std::string result = "ProcedureNode(" + procName + ",\n";
    for (auto stmt: stmtList) {
        result += std::visit([](const auto& s) { return s->toString(); }, stmt) + ",\n";
    }
    result += ")";
    return result;
};

void ProcedureNode::accept(Visitor* v) {
    std::shared_ptr<ProcedureNode> node{ shared_from_this() };
    v->visitProcedureNode(node);
}

ConstantNode::ConstantNode(const std::string value) : value(value) {}

bool ConstantNode::operator==(const TNode& other) const {
    const ConstantNode* castedOther = dynamic_cast<const ConstantNode*>(&other);
    bool canCast = castedOther != 0;
    if (!canCast) return false;
    bool equalValue = std::stoi(this->value) == std::stoi(castedOther->value);
    return canCast && equalValue;
}

std::string ConstantNode::toString() const {
    return "ConstantNode(" + value + ")";
};

void ConstantNode::accept(Visitor* v) {
    std::shared_ptr<ConstantNode> node{ shared_from_this() };
    v->visitConstantNode(node);
}

VariableNode::VariableNode(const std::string varName) : varName(varName) {}

bool VariableNode::operator==(const TNode& other) const {
    const VariableNode* castedOther = dynamic_cast<const VariableNode*>(&other);
    bool canCast = castedOther != 0;
    if (!canCast) return false;
    bool equalVarName = this->varName == castedOther->varName;
    return canCast && equalVarName;
}

std::string VariableNode::toString() const {
    return "VariableNode(" + varName + ")";
};

void VariableNode::accept(Visitor* v) {
    std::shared_ptr<VariableNode> node{ shared_from_this() };
    v->visitVariableNode(node);
}

AssignmentNode::AssignmentNode(VariableNodePtr var, Expr expr) :
        var(std::move(var)), expr(std::move(expr)) {}

bool AssignmentNode::operator==(const TNode& other) const {
    const AssignmentNode* castedOther = dynamic_cast<const AssignmentNode*>(&other);
    bool canCast = castedOther != 0;
    if (!canCast) return false;
    bool equalVarNode = *(this->var) == *(castedOther->var);
    bool equalExpr = std::visit([](const auto& _tp, const auto& _op) {
        return *_tp == *_op;
    }, this->expr, castedOther->expr);
    return canCast && equalVarNode && equalExpr;
}

std::string AssignmentNode::toString() const {
    std::string result = "AssignmentNode(" + var->toString() + ", ";
    result += std::visit([](const auto& e) { return e->toString(); }, expr);
    result += ")";
    return result;
};

void AssignmentNode::accept(Visitor* v) {
    std::shared_ptr<AssignmentNode> node{ shared_from_this() };
    v->visitAssignmentNode(node);
}

CallNode::CallNode(ProcedureNodePtr proc) : proc(std::move(proc)) {}

bool CallNode::operator==(const TNode& other) const {
    const CallNode* castedOther = dynamic_cast<const CallNode*>(&other);
    bool canCast = castedOther != 0;
    if (!canCast) return false;

    //! When comparing CallNodes, you only need to ensure that the procedure names are the same
    //! You do not need to compare the procedure nodes themselves (i.e. statement lists)
    bool equalProcName = (this->proc->procName) == (castedOther->proc->procName);
    return canCast && equalProcName;
}

std::string CallNode::toString() const {
    return "CallNode(procNode:" + proc->procName + ")";
};

void CallNode::accept(Visitor* v) {
    std::shared_ptr<CallNode> node{ shared_from_this() };
    v->visitCallNode(node);
}

PrintNode::PrintNode(VariableNodePtr var) : var(std::move(var)) {}

bool PrintNode::operator==(const TNode& other) const {
    const PrintNode* castedOther = dynamic_cast<const PrintNode*>(&other);
    bool canCast = castedOther != 0;
    if (!canCast) return false;
    bool equalVarNode = *(this->var) == *(castedOther->var);
    return canCast && equalVarNode;
}

std::string PrintNode::toString() const {
    return "PrintNode(" + var->toString() + ")";
};

void PrintNode::accept(Visitor* v) {
    std::shared_ptr<PrintNode> node{ shared_from_this() };
    v->visitPrintNode(node);
}

ReadNode::ReadNode(VariableNodePtr var) : var(std::move(var)) {}

bool ReadNode::operator==(const TNode& other) const {
    const ReadNode* castedOther = dynamic_cast<const ReadNode*>(&other);
    bool canCast = castedOther != 0;
    if (!canCast) return false;
    bool equalVarNode = *(this->var) == *(castedOther->var);
    return canCast && equalVarNode;
}

std::string ReadNode::toString() const {
    return "ReadNode(" + var->toString() + ")";
};

void ReadNode::accept(Visitor* v) {
    std::shared_ptr<ReadNode> node{ shared_from_this() };
    v->visitReadNode(node);
}

IfNode::IfNode(CondExprNodePtr condExpr, StmtLst thenStmtList,
               StmtLst elseStmtList) : condExpr(std::move(condExpr)),
                                       thenStmtList(std::move(thenStmtList)), elseStmtList(std::move(elseStmtList)) {}

bool IfNode::operator==(const TNode& other) const {
    const IfNode* castedOther = dynamic_cast<const IfNode*>(&other);
    bool canCast = castedOther != 0;
    if (!canCast) return false;
    bool equalCondExpr = *(this->condExpr) == *(castedOther->condExpr);
    bool equalThenStmtList = std::equal((this->thenStmtList).begin(), (this->thenStmtList).end(),
                                        (castedOther->thenStmtList).begin(), (castedOther->thenStmtList).end(),
                                        [](const Stmt& _this, const Stmt& _other) {
                                            return std::visit([](const auto& _tp, const auto& _op) {
                                                return *_tp == *_op;
                                            }, _this, _other);
                                        });
    bool equalElseStmtList = std::equal((this->elseStmtList).begin(), (this->elseStmtList).end(),
                                        (castedOther->elseStmtList).begin(), (castedOther->elseStmtList).end(),
                                        [](const Stmt& _this, const Stmt& _other) {
                                            return std::visit([](const auto& _tp, const auto& _op) {
                                                return *_tp == *_op;
                                            }, _this, _other);
                                        });
    return canCast && equalCondExpr && equalThenStmtList && equalElseStmtList;
}

std::string IfNode::toString() const {
    std::string result = "IfNode(" + condExpr->toString() + ",\n";
    result += "thenStmtList:[";
    for (const Stmt& stmt : thenStmtList) {
        result += std::visit([](const auto& e) { return e->toString(); }, stmt) + ",\n";
    }
    result += "],\n";
    result += "elseStmtList:[";
    for (const Stmt& stmt : elseStmtList) {
        result += std::visit([](const auto& e) { return e->toString(); }, stmt) + ",\n";
    }
    result += "])";
    return result;
};

void IfNode::accept(Visitor* v) {
    std::shared_ptr<IfNode> node{ shared_from_this() };
    v->visitIfNode(node);
}

WhileNode::WhileNode(CondExprNodePtr condExpr, StmtLst stmtList) :
        condExpr(std::move(condExpr)), stmtList(std::move(stmtList)) {}

bool WhileNode::operator==(const TNode& other) const {
    const WhileNode* castedOther = dynamic_cast<const WhileNode*>(&other);
    bool canCast = castedOther != 0;
    if (!canCast) return false;
    bool equalCondExpr = *(this->condExpr) == *(castedOther->condExpr);
    bool equalStmtList = std::equal((this->stmtList).begin(), (this->stmtList).end(),
                                    (castedOther->stmtList).begin(), (castedOther->stmtList).end(),
                                    [](const Stmt& _this, const Stmt& _other) {
                                        return std::visit([](const auto& _tp, const auto& _op) {
                                            return *_tp == *_op;
                                        }, _this, _other);
                                    });
    return canCast && equalCondExpr && equalStmtList;
}

std::string WhileNode::toString() const {
    std::string result = "WhileNode(" + condExpr->toString() + ",\n";
    for (auto stmt: stmtList) {
        result += std::visit([](const auto& s) { return s->toString(); }, stmt) + ",\n";
    }
    result += ")";
    return result;
};

void WhileNode::accept(Visitor* v) {
    std::shared_ptr<WhileNode> node{ shared_from_this() };
    v->visitWhileNode(node);
}

BinOpNode::BinOpNode(std::string op, Expr leftExpr, Expr rightExpr) :
        op(op), leftExpr(std::move(leftExpr)), rightExpr(std::move(rightExpr)) {}

bool BinOpNode::operator==(const TNode& other) const {
    const BinOpNode* castedOther = dynamic_cast<const BinOpNode*>(&other);
    bool canCast = castedOther != 0;
    if (!canCast) return false;
    bool equalOp = this->op == castedOther->op;
    bool equalLeftExpr = std::visit([](const auto& _tp, const auto& _op) {
        return *_tp == *_op;
    }, this->leftExpr, castedOther->leftExpr);
    bool equalRightExpr = std::visit([](const auto& _tp, const auto& _op) {
        return *_tp == *_op;
    }, this->rightExpr, castedOther->rightExpr);
    return canCast && equalOp && equalLeftExpr && equalRightExpr;
}

std::string BinOpNode::toString() const {
    std::string result = "BinOpNode(" + op + ", ";
    result += std::visit([](const auto& e) { return e->toString(); }, leftExpr) + ", ";
    result += std::visit([](const auto& e) { return e->toString(); }, rightExpr) + ")";
    return result;
};

void BinOpNode::accept(Visitor* v) {
    std::shared_ptr<BinOpNode> node{ shared_from_this() };
    v->visitBinOpNode(node);
}

CondExprNode::CondExprNode(RelExprNodePtr relExpr) : relExpr(std::move(relExpr)) {};

CondExprNode::CondExprNode(CondExprNodePtr leftCond) :
        op("!"), leftCond(std::move(leftCond)) {};

CondExprNode::CondExprNode(std::string op, CondExprNodePtr leftCond,
                           CondExprNodePtr rightCond) : op(op), leftCond(std::move(leftCond)),
                                                        rightCond(std::move(rightCond)) {}

bool CondExprNode::operator==(const TNode& other) const {
    const CondExprNode* castedOther = dynamic_cast<const CondExprNode*>(&other);
    bool canCast = castedOther != 0;
    if (!canCast) return false;
    //! Either both their relExpr are nullptr or both are (not nullptr and equal)
    bool equalRelExpr = (this->relExpr == castedOther->relExpr) || *(this->relExpr) == *(castedOther->relExpr);
    bool equalOp = this->op == castedOther->op;
    //! Either both their leftCond are nullptr or both are (not nullptr and equal)
    bool equalLeftCond = (this->leftCond == castedOther->leftCond) || *(this->leftCond) == *(castedOther->leftCond);
    //! Either both their rightCond are nullptr or both are (not nullptr and equal)
    bool equalRightCond = (this->rightCond == castedOther->rightCond) || *(this->rightCond) == *(castedOther->rightCond);
    return canCast && equalRelExpr && equalOp && equalLeftCond && equalRightCond;
}

std::string CondExprNode::toString() const {
    std::string result = "CondExprNode(" + op + ", ";
    if (relExpr) {
        //! Just a normal relational expression
        result += relExpr->toString();
    } else if (rightCond == nullptr) {
        //! This conditional expression node is in the form of !(condExpr)
        result += leftCond->toString();
    } else {
        //! This conditional expression node is in the form of (condExpr1 op condExpr2)
        result += leftCond->toString() + ", " + rightCond->toString();
    }
    result += ")";
    return result;
};

void CondExprNode::accept(Visitor* v) {
    std::shared_ptr<CondExprNode> node{ shared_from_this() };
    v->visitCondExprNode(node);
}

RelExprNode::RelExprNode(std::string op, Expr leftRel, Expr rightRel) :
        op(op), leftRel(std::move(leftRel)), rightRel(std::move(rightRel)) {}

bool RelExprNode::operator==(const TNode& other) const {
    const RelExprNode* castedOther = dynamic_cast<const RelExprNode*>(&other);
    bool canCast = castedOther != 0;
    if (!canCast) return false;
    bool equalOp = this->op == castedOther->op;
    bool equalLeftRel = std::visit([](const auto& _tp, const auto& _op) {
        return *_tp == *_op;
    }, this->leftRel, castedOther->leftRel);
    bool equalRightRel = std::visit([](const auto& _tp, const auto& _op) {
        return *_tp == *_op;
    }, this->rightRel, castedOther->rightRel);
    return canCast && equalOp && equalLeftRel && equalRightRel;
}

std::string RelExprNode::toString() const {
    std::string result = "RelExprNode(" + op + ", ";
    result += std::visit([](const auto& e) { return e->toString(); }, leftRel) + ", ";
    result += std::visit([](const auto& e) { return e->toString(); }, rightRel) + ")";
    return result;
};

void RelExprNode::accept(Visitor* v) {
    std::shared_ptr<RelExprNode> node{ shared_from_this() };
    v->visitRelExprNode(node);
}
