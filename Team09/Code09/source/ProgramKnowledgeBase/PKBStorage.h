#pragma once

#include <unordered_set>
#include "../SourceProcessor/astBuilder/TNode.h"

namespace PKB {

    class PKBStorage {
    public:
        PKBStorage();
        ~PKBStorage();

        // node pointers
        using AssignNode = TNode::ASSIGNMENT_NODE_PTR;
        using ProcedureNode = TNode::PROCEDURE_NODE_PTR;
        using VariableNode = TNode::VARIABLE_NODE_PTR;

        // design entities
        std::unordered_set<std::string> assignSet;
        std::unordered_set<std::string> procedureSet;
        std::unordered_set<std::string> variableSet;

        // relationships
        std::unordered_set<std::string> assignment_modifies_variable;

        // store
        void persistEntity(AssignNode);
        void persistEntity(ProcedureNode);
        void persistEntity(VariableNode);
        void persistAssignModifyVariable(VariableNode);

        std::unordered_set<std::string> getVariableSet();
        std::unordered_set<std::string> getAllModify();
    };
}
