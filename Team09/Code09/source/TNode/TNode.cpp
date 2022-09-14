#include "TNode.h"

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
    return "CallNode(" + proc->procName + ")";
};

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
    std::string result = "IfNode(" + condExpr->toString() + ", ";
    for (auto stmt: thenStmtList) {
        result += std::visit([](const auto& s) { return s->toString(); }, stmt) + ", ";
    }
    result += ", ";
    for (auto stmt: elseStmtList) {
        result += std::visit([](const auto& s) { return s->toString(); }, stmt) + ", ";
    }
    result += ")";
    return result;
};

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
    std::string result = "WhileNode(" + condExpr->toString() + ", ";
    for (auto stmt: stmtList) {
        result += std::visit([](const auto& s) { return s->toString(); }, stmt) + ", ";
    }
    result += ")";
    return result;
};

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
    bool equalOp = this->op == castedOther->op;
    bool equalLeftCond = *(this->leftCond) == *(castedOther->leftCond);
    bool equalRightCond = *(this->rightCond) == *(castedOther->rightCond);
    bool equalRelExpr = *(this->relExpr) == *(castedOther->relExpr);
    return canCast && equalOp && equalLeftCond && equalRightCond && equalRelExpr;
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
