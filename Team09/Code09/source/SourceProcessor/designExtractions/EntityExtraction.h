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

    void extractEntities(TNode::PROGRAM_NODE_PTR astRoot);

    /*
    void extractEntities(const shared_ptr<ProcedureNode> proc);

    void extractEntities(const shared_ptr<AssignmentNode> assign);

    void extractEntities(const shared_ptr<VariableNode> var);

    void extractEntities(const shared_ptr<PlusNode> plus);
     */
};
