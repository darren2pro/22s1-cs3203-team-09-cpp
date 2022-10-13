#include "DesignExtractor.h"
#include "EntityExtraction.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"

DesignExtractor::~DesignExtractor() {}

void DesignExtractor::extractDesignAbstractions(const std::shared_ptr<ProgramNode> astRoot) {
    EntityExtraction entityExtraction = EntityExtraction(pkbFacade);
    entityExtraction.createLineNumbers(astRoot);
    entityExtraction.createCFG(astRoot);
    entityExtraction.extractEntities(astRoot);
    entityExtraction.extractCallsRls(astRoot);
    entityExtraction.extractFollowsRls(astRoot);
    entityExtraction.extractParentsRls(astRoot);
    entityExtraction.extractModifyRls(astRoot);
    entityExtraction.extractUsesRls(astRoot);
    entityExtraction.extractAssignPattern(astRoot);
}
