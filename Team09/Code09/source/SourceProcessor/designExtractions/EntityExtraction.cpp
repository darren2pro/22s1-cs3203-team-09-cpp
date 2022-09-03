#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PKB.h"
#include "EntityExtraction.h"
#include "../astBuilder/TNode.h"
#include "SourceProcessor/astBuilder/ProgramNode.h"
#include "SourceProcessor/astBuilder/ProcedureNode.h"
#include "SourceProcessor/astBuilder/AssignmentNode.h"


EntityExtraction::EntityExtraction(PKB pkb) {
	pkbFacade = pkb;
}

EntityExtraction::~EntityExtraction() {}

void EntityExtraction::extractEntities(const std::shared_ptr<ProgramNode> astRoot){
	for (const auto& proc : astRoot->procedureList) {
		extractEntities(proc);
	}
}
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