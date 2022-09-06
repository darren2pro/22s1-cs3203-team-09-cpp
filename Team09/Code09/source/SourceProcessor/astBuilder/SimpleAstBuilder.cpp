#include "SimpleAstBuilder.h"
#include "../exceptions/SimpleInvalidSyntaxException.h"
#include "ProgramNode.h"
#include "ProcedureNode.h"
#include "AssignmentNode.h"
#include "ConstantNode.h"
#include "PlusNode.h"
#include "VariableNode.h"
#include "BinaryOperatorNode.h"
#include "MinusNode.h"

using namespace std;

SimpleAstBuilder::SimpleAstBuilder(const Parser::SOURCE_CODE_TOKENS tokens) {
    this->tokens = tokens;
    this->currentTokenIndex = 0;
    this->currentLineNo = 1;
    this->programNode = make_shared<ProgramNode>();
}

SimpleAstBuilder::~SimpleAstBuilder() {
}

int SimpleAstBuilder::build() {
    while (currentTokenIndex < tokens.size()) {
        const SimpleToken currentToken = getCurrentToken();
        if (currentToken.getType() != SimpleToken::TokenType::PROCEDURE) {
            char* message = new char[100];
            sprintf(message, "Expected procedure token. Got %s", currentToken.getValue().c_str());
            throw SimpleInvalidSyntaxException(message);
            return -1;
        }
        advanceTokenIndex();
        handleProcedure();
    }
    return 0;
}

void SimpleAstBuilder::handleProcedure() {
    const SimpleToken procedureToken = getCurrentToken();
    if (procedureToken.getType() != SimpleToken::TokenType::WORD) {
        char* message = new char[100];
        sprintf(message, "Expected procedure name. Got %s", procedureToken.getValue().c_str());
        throw SimpleInvalidSyntaxException(message);
    }
    advanceTokenIndex();
    const SimpleToken leftBracesToken = getCurrentToken();
    if (leftBracesToken.getType() != SimpleToken::TokenType::OPEN_BRACES) {
        char* message = new char[100];
        sprintf(message, "Expected open braces. Got %s", leftBracesToken.getValue().c_str());
        throw SimpleInvalidSyntaxException(message);
    }
    advanceTokenIndex();
    TNode::PROCEDURE_NODE_PTR procedureNode = make_shared<ProcedureNode>(procedureToken.getValue());
    programNode->addProcedure(procedureNode);
    while (tokens[currentTokenIndex].getType() != SimpleToken::TokenType::CLOSE_BRACES) {
        handleProcedureStatement(procedureNode);
        advanceTokenIndex();
    }
    advanceTokenIndex();
}

void SimpleAstBuilder::handleProcedureStatement(TNode::PROCEDURE_NODE_PTR procedureNode) {
    const SimpleToken currentToken = getCurrentToken();
    const SimpleToken::TokenType tokenType = currentToken.getType();
    switch (tokenType) {
        case SimpleToken::TokenType::WORD:
            // If it is a word then it is a variable assignment
            handleAssignmentStatement(procedureNode);
            break;
        default:
            char* message = new char[100];
            sprintf(message, "Expected procedure statement. Got %s", currentToken.getValue().c_str());
            throw SimpleInvalidSyntaxException(message);
    }
}

void SimpleAstBuilder::handleAssignmentStatement(TNode::PROCEDURE_NODE_PTR procedureNode) {
    const SimpleToken variableToken = getCurrentToken();
    advanceTokenIndex();
    const SimpleToken equalToken = getCurrentToken();
    if (equalToken.getType() != SimpleToken::TokenType::ASSIGN) {
        char* message = new char[100];
        sprintf(message, "Expected equals token. Got %s", equalToken.getValue().c_str());
        throw SimpleInvalidSyntaxException(message);
    }
    TNode::ASSIGNMENT_NODE_PTR assignmentNode = make_shared<AssignmentNode>();
    TNode::VARIABLE_NODE_PTR variableNode = make_shared<VariableNode>(variableToken.getValue());
    assignmentNode->addAssignedVariable(variableNode);
    procedureNode->addStatement(assignmentNode);
    handleAssignmentExpression(assignmentNode);
}

void SimpleAstBuilder::handleAssignmentExpression(TNode::ASSIGNMENT_NODE_PTR assignmentNode) {
    advanceTokenIndex();
    SimpleToken currentToken = tokens[currentTokenIndex];
    SimpleToken::TokenType tokenType = currentToken.getType();
    const TNode::PLUS_NODE_PTR assignmentExpressionRootNode = make_shared<PlusNode>();
    assignmentNode->addPlus(assignmentExpressionRootNode);
    // TODO: Need to improve this to build the expression subtree correctly. I think we have to build from bottom up
    while (tokenType != SimpleToken::TokenType::SEMICOLON) {
        switch (tokenType) {
            case SimpleToken::TokenType::WORD: {
                TNode::VARIABLE_NODE_PTR variableNode = make_shared<VariableNode>(currentToken.getValue());
                assignmentExpressionRootNode->setLeftSubtree(variableNode);
            }
                break;
            case SimpleToken::TokenType::NUMBER: {
                TNode::CONSTANT_NODE_PTR constantNode = make_shared<ConstantNode>(currentToken.getValue());
                assignmentExpressionRootNode->setRightSubtree(constantNode);
            }
                break;
            case SimpleToken::TokenType::PLUS:
                // To be edited
                break;
            default:
                char* message = new char[100];
                sprintf(message, "Expected assignment expression. Got %s", currentToken.getValue().c_str());
                throw SimpleInvalidSyntaxException(message);
        }
        advanceTokenIndex();
        currentToken = tokens.at(currentTokenIndex);
        tokenType = currentToken.getType();
    }
}

const TNode::PROGRAM_NODE_PTR &SimpleAstBuilder::getProgramNode() const {
    return programNode;
}

void SimpleAstBuilder::advanceTokenIndex() {
    currentTokenIndex++;
}

SimpleToken SimpleAstBuilder::getCurrentToken() {
    return tokens.at(currentTokenIndex);
}