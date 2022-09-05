#pragma once

#include <memory>
#include "../../PKB.h"
#include "../astBuilder/TNode.h"

using namespace std;

class EntityExtraction {
private:
    PKB pkbFacade;

public:
    explicit EntityExtraction(PKB pkb);

    ~EntityExtraction();

    void extractEntities(const TNode::PROGRAM_NODE_PTR astRoot);
    void extractEntities(const TNode::PROCEDURE_NODE_PTR proc);
    void extractEntities(TNode::ASSIGNMENT_NODE_PTR assign);
    void extractEntities(TNode::VARIABLE_NODE_PTR var);
    void extractEntities(TNode::CONSTANT_NODE_PTR cons);

};
