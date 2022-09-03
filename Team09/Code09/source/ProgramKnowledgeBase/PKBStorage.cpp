#include "PKBStorage.h"

namespace PKB {
    PKBStorage::PKBStorage(){};
    PKBStorage::~PKBStorage(){};

    // store entities
    void PKBStorage::persistEntity(PKB::PKBStorage::assignNode) {
        PKBStorage::assignSet.insert(assignNode);
    }
    void PKBStorage::persistEntity(PKB::PKBStorage::procedureNode) {
        PKBStorage::procedureSet.insert(procedureNode);
    }

    // store modifies
    void PKBStorage::persistAssignModifyVariable(PKB::PKBStorage::assignNode, PKB::PKBStorage::variableNode) {
        PKBStorage::assignment_modifies_variable(assignNode, variableNode);
    }
}