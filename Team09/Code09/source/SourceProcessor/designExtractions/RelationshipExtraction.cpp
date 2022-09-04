#include "RelationshipExtraction.h"
#include "PKB.h"
#include "../astBuilder/TNode.h"
#include "../astBuilder/ProgramNode.h"
#include "../astBuilder/ProcedureNode.h"
#include "../astBuilder/AssignmentNode.h"

RelationshipExtraction::RelationshipExtraction(PKB pkb) {
    pkbFacade = pkb;
}

RelationshipExtraction::~RelationshipExtraction() {}

void RelationshipExtraction::extractRls(TNode::PROGRAM_NODE_PTR astRoot) {
    for (size_t i = 0; i < astRoot->getProcedureCount(); i++) {
        TNode::PROCEDURE_NODE_PTR proc = astRoot->getProcedureByIndex(i);
        extractRls(proc);
    }
}
void RelationshipExtraction::extractRls(TNode::PROCEDURE_NODE_PTR proc) {
        for (size_t i = 0; i < proc->getStatementCount(); i++) {
            TNode::ASSIGNMENT_NODE_PTR stmt = proc->getStatementByIndex(i);
            extractRls(stmt);
        }
 }
void RelationshipExtraction::extractRls(TNode::ASSIGNMENT_NODE_PTR assign) {
        pkbFacade.persistModifies(assign, assign->variableNode);
}