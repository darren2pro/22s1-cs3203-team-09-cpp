#include "ProgramNode.h"

ProgramNode::ProgramNode() {
}

string ProgramNode::toString() {
    return string();
}

void ProgramNode::addProcedure(PROCEDURE_NODE_PTR procedure) {
    procedureList.push_back(procedure);
}

bool ProgramNode::isDesignEntity() {
    return false;
}

bool ProgramNode::isAssignmentNode() {
    return false;
}

TNode::PROCEDURE_NODE_PTR ProgramNode::getProcedureByIndex(int index) {
    return procedureList[index];
}

size_t ProgramNode::getProcedureCount() {
    return procedureList.size();
}
