#include "EntityExtraction.h"
#include "../astBuilder/ProgramNode.h"
#include <stack>
#include "../astBuilder/StmtLstNode.h"
#include "../astBuilder/ProcedureNode.h"
#include "../astBuilder/AssignmentNode.h"
#include "PKB.h"
#include "../astBuilder/PlusNode.h"

EntityExtraction::EntityExtraction(PKB pkb) {
	pkbFacade = pkb;
}

EntityExtraction::~EntityExtraction() {}

void EntityExtraction::extractEntities(const TNode::PROGRAM_NODE_PTR astRoot) {
	for (size_t i = 0; i < astRoot->getProcedureCount(); i++) {
		TNode::PROCEDURE_NODE_PTR proc = astRoot->getProcedureByIndex(i);
		extractEntities(proc);
	}
}
void EntityExtraction::extractEntities(const TNode::PROCEDURE_NODE_PTR proc) {
	for (size_t i = 0; i < proc->getStatementCount(); i++) {
		TNode::ASSIGNMENT_NODE_PTR stmt = proc->getStatementByIndex(i);
		extractEntities(stmt);
	}
	pkbFacade.persistEntity(proc);
}

void EntityExtraction::extractEntities(TNode::ASSIGNMENT_NODE_PTR assign) {
	TNode::VARIABLE_NODE_PTR var = assign->getAssignedVariableNode();
	TNode::PLUS_NODE_PTR plus = assign->plusNode;
	pkbFacade.persistEntity(assign);
	pkbFacade.persistEntity(var);
	extractEntities(plus->leftSubtree);
	extractEntities(plus->rightSubtree);
}
void EntityExtraction::extractEntities(TNode::VARIABLE_NODE_PTR var) {
	pkbFacade.persistEntity(var);
}
void EntityExtraction::extractEntities(TNode::CONSTANT_NODE_PTR cons) {
	pkbFacade.persistEntity(cons);
}