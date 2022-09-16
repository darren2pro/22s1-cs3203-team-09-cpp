#pragma once

#include "../../ProgramKnowledgeBase/PKBStorage.h"

class RelationshipExtraction {
private:
    PKBStorage* pkbFacade;

public:
    explicit RelationshipExtraction(PKBStorage* pkb);

    ~RelationshipExtraction();

    // void extractRls(TNode::PROGRAM_NODE_PTR astRoot);
    // void extractRls(TNode::PROCEDURE_NODE_PTR proc);
    // void extractRls(TNode::ASSIGNMENT_NODE_PTR assign, TNode::PROCEDURE_NODE_PTR proc);
};
