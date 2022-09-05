#include <unordered_set>
#include <unordered_map>
#include "PKBStorage.h"
#include "../SourceProcessor/astBuilder/ProcedureNode.h"
#include "../SourceProcessor/astBuilder/AssignmentNode.h"
#include "../SourceProcessor/astBuilder/VariableNode.h"

namespace PKB {
    PKBStorage::PKBStorage(){};
    PKBStorage::~PKBStorage(){};

    void PKBStorage::persistEntity(AssignNode assignNode) {}
    void PKBStorage::persistEntity(ProcedureNode procedureNode) {
        this->procedureSet.insert(procedureNode->name);
    }

    void PKBStorage::persistEntity(VariableNode variableNode) {
        this->variableSet.insert(variableNode->variableName);
    }
    
    void PKBStorage::persistAssignModifyVariable(VariableNode variableNode) {
        this->assignment_modifies_variable.insert(variableNode->variableName);
    }

    std::unordered_set<std::string> PKBStorage::getVariableSet() {
        return this->variableSet;
    }

    std::unordered_set<std::string> PKBStorage::getProcedureSet() {
        return this->procedureSet;
    }

    std::unordered_set<std::string> PKBStorage::getAllModify() {
        return this->assignment_modifies_variable;
    }
}
