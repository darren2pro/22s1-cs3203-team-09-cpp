#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "../source/PKB.h"
#include "../source/SourceProcessor/astBuilder/TNode.h"
#include "DesignExtractor.h"
#include "EntityExtraction.h"
#include "RelationshipExtraction.h"

DesignExtractor::DesignExtractor(PKB pkb) {
	pkbFacade = pkb;
}

DesignExtractor::~DesignExtractor() {}

void DesignExtractor::extractDesignAbstractions(PROGRAM_NODE_PTR astRoot) {
	EntityExtraction entityExtraction = EntityExtraction(pkbFacade);
	entityExtraction.extractEntities(astRoot);
	RelationshipExtraction rlsExtraction = RelationshipExtraction(pkbFacade);
	rlsExtraction.extractRls(astRoot);

}
