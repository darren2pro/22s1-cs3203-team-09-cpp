#pragma once

#include "RelationEvaluator.h"

using namespace PKB;

class AffectsTRelationEvaluator : public RelationEvaluator {
public:
    using RelationEvaluator::RelationEvaluator;

    bool evaluate();

    void computeFully();
};