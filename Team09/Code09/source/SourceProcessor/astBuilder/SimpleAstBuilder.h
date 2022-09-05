#pragma once

#include "../Parser.h"
#include "ProgramNode.h"
#include "AssignmentNode.h"
#include <memory>
#include <string>

/**
 * @brief The SimpleAstBuilder class which validates the input source program.
 */
class SimpleAstBuilder {
private:
    Parser::SOURCE_CODE_TOKENS tokens;

    int currentTokenIndex;

    int currentLineNo;

    // shared pointer of program node
    TNode::PROGRAM_NODE_PTR programNode;

public:
    explicit SimpleAstBuilder(const Parser::SOURCE_CODE_TOKENS tokens);

    ~SimpleAstBuilder();

    int build();

    /**
     * @brief Deals with the building of the AST for a single procedure block.
     * @details This method will create the necessary nodes for the procedure block and then call other methods
     * as per needed for the statements in the procedure block.
     */
    void handleProcedure();

    void handleProcedureStatement(TNode::PROCEDURE_NODE_PTR procedureNode);

    void handleAssignmentStatement(TNode::PROCEDURE_NODE_PTR procedureNode);

    void handleAssignmentExpression(TNode::ASSIGNMENT_NODE_PTR assignmentNode);

    const TNode::PROGRAM_NODE_PTR &getProgramNode() const;
};
