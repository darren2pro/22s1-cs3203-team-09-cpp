#include "ExtractCallsVisitor.h"
#include "../../../QueryProcessingSubsystem/Validator/SemanticException.h"

void ExtractCallsVisitor::extractCallsT() {
    for (const auto& calls : pkbStorage->getRelationSet(Relation::Calls)) {
        PKB::CallerProc caller = calls.first;
        std::vector<PKB::CalleeProc> list;
        list.push_back(caller);
        while (!list.empty()) {
            PKB::CalleeProc currCallee = list.back();
            list.pop_back();
            for (const auto& callee : pkbStorage->getRelationSecondFromFirst(Relation::Calls, currCallee)) {
                if (caller == callee) throw SemanticError("Cyclic call detected.");
                pkbStorage->storeRelations(Relation::CallsT, caller, callee);
                if (pkbStorage->relationContainsFirst(Relation::Calls, callee)) {
                    list.push_back(callee);
                }
            }
        }
    }
}

void ExtractCallsVisitor::visitProgramNode(std::shared_ptr<ProgramNode> node) {
    for (const auto& proc : node->procList) {
        proc->accept(this);
    }
    extractCallsT();
}
void ExtractCallsVisitor::visitProcedureNode(std::shared_ptr<ProcedureNode> node) {
    visitStmtList(node->stmtList, this);
}
void ExtractCallsVisitor::visitIfNode(std::shared_ptr<IfNode> node) {
    visitStmtList(node->thenStmtList, this);
    visitStmtList(node->elseStmtList, this);
}
void ExtractCallsVisitor::visitWhileNode(std::shared_ptr<WhileNode> node) {
    visitStmtList(node->stmtList, this);
}
void ExtractCallsVisitor::visitCallNode(std::shared_ptr<CallNode> node) {
    const PKB::LineNum lnNum = getLineFromNode(node);
    const PKB::Procedure caller = getProcedureFromLine(lnNum);
    const PKB::Procedure callee = node->proc->procName;
    if (caller == callee) throw SemanticError("Calling itself");
    pkbStorage->storeRelations(Relation::Calls, caller, callee);
}