#pragma once

#include<unordered_set>
#include "../SourceProcessor/astBuilder/TNode.h"

namespace PKB {

    class PKBStorage {
    public:
        PKBStorage();
        ~PKBStorage();

        // node pointers
        using assignNode = TNode::ASSIGNMENT_NODE_PTR;
        using procedureNode = TNode::PROCEDURE_NODE_PTR;
        using variableNode = TNode::VARIABLE_NODE_PTR;

        // design entities
        std::unordered_set<assignNode> assignSet;
        std::unordered_set<procedureNode> procedureSet;

        // relationships
        std::unordered_set<assignNode, variableNode> assignment_modifies_variable;

        // store
        void persistEntity(assignNode);
        void persistEntity(procedureNode);

        void persistAssignModifyVariable(assignNode, variableNode);
    };
}