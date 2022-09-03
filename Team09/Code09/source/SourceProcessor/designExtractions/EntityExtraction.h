#pragma once

#include <memory>
#include "../../PKB.h"
#include "../astBuilder/TNode.h"

class EntityExtraction {
private:
    PKB pkbFacade;

public:
    explicit EntityExtraction(PKB pkb);

    ~EntityExtraction();

    void extractEntities(TNode::PROGRAM_NODE_PTR astRoot);

    /*
    void extractEntities(const std::shared_ptr<ProcedureNode> proc);

    void extractEntities(const std::shared_ptr<ProcedureNode> proc);

    void extractEntities(const std::shared_ptr<AssignmentNode> assign);

    void extractEntities(const std::shared_ptr<VariableNode> var);

    void extractEntities(const std::shared_ptr<PlusNode> plus);
     */
};
