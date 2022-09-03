#include "EntityExtraction.h"
#include "../astBuilder/ProgramNode.h"

EntityExtraction::EntityExtraction(PKB pkb) {
    pkbFacade = pkb;
}

EntityExtraction::~EntityExtraction() {}

void EntityExtraction::extractEntities(const TNode::PROGRAM_NODE_PTR astRoot) {
    for (size_t i = 0; i < astRoot->getProcedureCount(); i++) {
        TNode::PROCEDURE_NODE_PTR proc = astRoot->getProcedureByIndex(i);
        // extract entities from procedure
        // extractEntities(proc);
    }
}

/*
void EntityExtraction::extractEntities(const std::shared_ptr<ProcedureNode> proc) {
	statementsIterator(proc->StmtList);
}
void  EntityExtraction::statementsIterator(const std::vector<StmtLstNode> stmt_lst) {
	for (const auto& stmt : stmt_lst) {
		//iterate through statements
	}
}
void EntityExtraction::extractEntities(const std::shared_ptr<AssignmentNode> assign) {
	extractEntities(assign -> variableNode);
	extractEntities(assign -> plusNode);
}

void EntityExtraction::extractEntities(
	const std::shared_ptr<VariableNode> var) {
}

void EntityExtraction::extractEntities(const std::shared_ptr<PlusNode> plus) {
}
 */
