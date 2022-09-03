#pragma once

#include "../../PKB.h"
#include "../astBuilder/TNode.h"

class RelationshipExtraction {
private:
    PKB pkbFacade;

public:
    explicit RelationshipExtraction(PKB pkb);

    ~RelationshipExtraction();

    void extractRls(TNode::PROGRAM_NODE_PTR astRoot);
};
