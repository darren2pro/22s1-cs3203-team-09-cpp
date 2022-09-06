#pragma once

#include <string>
#include <memory>
#include <vector>

using namespace std;

class ProgramNode;

class ProcedureNode;

class StmtLstNode;

class AssignmentNode;

class BinaryOperatorNode;

class PlusNode;

class ConstantNode;

class VariableNode;

class StatementNode;

class TNode {
public:
    typedef shared_ptr<ProgramNode> PROGRAM_NODE_PTR;
    typedef shared_ptr<ProcedureNode> PROCEDURE_NODE_PTR;
    typedef shared_ptr<StmtLstNode> STMT_LST_NODE_PTR;
    typedef shared_ptr<AssignmentNode> ASSIGNMENT_NODE_PTR;
    typedef shared_ptr<VariableNode> VARIABLE_NODE_PTR;
    typedef shared_ptr<BinaryOperatorNode> BIN_OP_NODE_PTR;
    typedef shared_ptr<PlusNode> PLUS_NODE_PTR;
    typedef shared_ptr<ConstantNode> CONSTANT_NODE_PTR;

    typedef vector<shared_ptr<ProcedureNode>> PROCEDURE_PTR_LST;

    // TODO: Change from AssignmentNode to StatementNode
    typedef shared_ptr<AssignmentNode> STATEMENT_NODE_PTR;
    typedef vector<shared_ptr<AssignmentNode>> STATEMENT_PTR_LST;
    typedef shared_ptr<TNode> T_NODE_PTR;

    virtual ~TNode() = default;

    virtual string toString() = 0;

    virtual bool isDesignEntity() = 0;

    /*
     * TODO: This is poor design. To be changed to visitor pattern in a future sprint.
     * https://stackoverflow.com/a/500495
     */
    virtual bool isAssignmentNode() = 0;

    virtual bool isBinaryOperatorNode() {
        return false;
    };

    virtual bool isVariableNode() {
        return false;
    };

    virtual string getVariableName() {
        return "";
    };
};
