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
    void extractRls(TNode::PROCEDURE_NODE_PTR proc);
    void extractRls(TNode::ASSIGNMENT_NODE_PTR assign);
};
