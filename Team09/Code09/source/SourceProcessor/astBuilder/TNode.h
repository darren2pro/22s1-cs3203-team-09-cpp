#pragma once

#include <string>
#include <memory>
#include <vector>

using namespace std;

class ProgramNode;
class ProcedureNode;
class StmtLstNode;
class AssignmentNode;
class PlusNode;
class ConstantNode;
class VariableNode;
class StatementNode;

class TNode {
protected:
    typedef vector<shared_ptr<ProcedureNode>> PROCEDURE_PTR_LST;

    // TODO: Change from TNode to StatementNode
    typedef shared_ptr<TNode> STATEMENT_NODE_PTR;
    typedef vector<shared_ptr<TNode>> STATEMENT_PTR_LST;

public:
    virtual ~TNode() = default;

    virtual string toString() = 0;

    typedef shared_ptr<ProgramNode> PROGRAM_NODE_PTR;
    typedef shared_ptr<ProcedureNode> PROCEDURE_NODE_PTR;
    typedef shared_ptr<StmtLstNode> STMT_LST_NODE_PTR;
    typedef shared_ptr<AssignmentNode> ASSIGNMENT_NODE_PTR;
    typedef shared_ptr<VariableNode> VARIABLE_NODE_PTR;
    typedef shared_ptr<PlusNode> PLUS_NODE_PTR;
    typedef shared_ptr<ConstantNode> CONSTANT_NODE_PTR;
};
