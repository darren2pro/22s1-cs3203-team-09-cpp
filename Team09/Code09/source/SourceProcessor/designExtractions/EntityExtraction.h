#pragma once

// #include "PKB.h"
#include <memory>
#include "../../PKB.h"
#include "../astBuilder/TNode.h"

class EntityExtraction {
private:
    PKB pkbFacade;

public:
    explicit EntityExtraction(PKB pkb);

    ~EntityExtraction();

    void extractEntities(const TNode::PROGRAM_NODE_PTR astRoot);

    /*
    void extractEntities(const std::shared_ptr<ProcedureNode> proc);

    void EntityExtraction::extractEntities(const std::shared_ptr<ProcedureNode> proc);

    void EntityExtraction::extractEntities(const std::shared_ptr<AssignmentNode> assign);

    void EntityExtraction::extractEntities(const std::shared_ptr<VariableNode> var);

    void extractEntities(const std::shared_ptr<PlusNode> plus);
     */
};