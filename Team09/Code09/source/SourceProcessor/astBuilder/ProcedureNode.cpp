#include "ProcedureNode.h"

using namespace std;

ProcedureNode::ProcedureNode(string name) {
    this->name = name;
}

string ProcedureNode::toString() {
    return std::string();
}

void ProcedureNode::addStatement(STATEMENT_NODE_PTR statement) {

}
