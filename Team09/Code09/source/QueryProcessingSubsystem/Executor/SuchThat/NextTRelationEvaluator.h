#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "../../Relation.h"
#include "../../../ProgramKnowledgeBase/PKBStorage.h"
#include "../../../ProgramKnowledgeBase/PKBStorage.h"
#include "../ResultsDatabase/ResultsDatabase.h"
#include "RelationEvaluator.h"

using namespace PKB;

class NextTRelationEvaluator : public RelationEvaluator {
public:
    using RelationEvaluator::RelationEvaluator;

    bool evaluate();

    void computeFully();
};