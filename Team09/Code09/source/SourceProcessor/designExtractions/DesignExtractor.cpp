#include "DesignExtractor.h"
#include "EntityExtraction.h"
#include "RelationshipExtraction.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"

/*
DesignExtractor::DesignExtractor(PKBStorage pkb) {
}
*/

DesignExtractor::~DesignExtractor() {}

void DesignExtractor::extractDesignAbstractions(TNode::PROGRAM_NODE_PTR astRoot) {
    EntityExtraction entityExtraction = EntityExtraction(pkbFacade);
    entityExtraction.extractEntities(astRoot);
    RelationshipExtraction rlsExtraction = RelationshipExtraction(pkbFacade);
    rlsExtraction.extractRls(astRoot);
}
