#pragma once

#include "RelationEvaluator.h"

using namespace PKB;

class AffectsRelationEvaluator : public RelationEvaluator {
private:
    bool lineUsesVar(std::string uses, std::string var);
    bool lineReachesline(std::string modifies, std::string uses);
    bool isNotModified(std::string modifies, std::string uses, std::string var);
public:
    using RelationEvaluator::RelationEvaluator;

    bool evaluate();

    void computeFully();
};