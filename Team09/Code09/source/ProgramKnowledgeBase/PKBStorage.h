#pragma once

#include <unordered_set>
#include "../TNode/TNode.h"

namespace PKB {

    class PKBStorage {
    public:
        PKBStorage();
        ~PKBStorage();

        // design entities
        std::unordered_set<std::string> assignSet;
        std::unordered_set<std::string> procedureSet;
        std::unordered_set<std::string> variableSet;

        // relationships
        std::unordered_set<std::string> assignment_modifies_variable;

        // store
        void persistEntity(AssignmentNode);
        void persistEntity(ProcedureNode);
        void persistEntity(VariableNode);
        void persistAssignModifyVariable(VariableNode);

        std::unordered_set<std::string> getVariableSet();
        std::unordered_set<std::string> getProcedureSet();
        std::unordered_set<std::string> getAllModify();
    };
}
