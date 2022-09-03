#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PKB.h"
#include <SourceProcessor/astBuilder/TNode.h>


class EntityExtraction {
private:
	PKB pkbFacade;

public:
	EntityExtraction(PKB pkb);
	~EntityExtraction();
	void extractEntities(const std::shared_ptr<ProgramNode> astRoot);
	void extractEntities(const std::shared_ptr<ProcedureNode> proc);
	void EntityExtraction::extractEntities(const std::shared_ptr<ProcedureNode> proc);
	void EntityExtraction::extractEntities(const std::shared_ptr<AssignmentNode> assign);
	void EntityExtraction::extractEntities(const std::shared_ptr<VariableNode> var);
	void extractEntities(const std::shared_ptr<PlusNode> plus);
};