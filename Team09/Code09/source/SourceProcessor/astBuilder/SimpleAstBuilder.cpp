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
    TNode::T_NODE_PTR expressionRootNode = buildExpressionTree();
    assignmentNode->addExpressionRootNode(expressionRootNode);
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

TNode::T_NODE_PTR SimpleAstBuilder::buildExpressionTree() {
    vector<SimpleToken> expression;
    SimpleToken currentToken = getCurrentToken();
    SimpleToken::TokenType tokenType = currentToken.getType();
    while (tokenType != SimpleToken::TokenType::SEMICOLON) {
        expression.push_back(currentToken);
        advanceTokenIndex();
        currentToken = getCurrentToken();
        tokenType = currentToken.getType();
    }
    if (expression.size() == 1) {
        SimpleToken expressionToken = expression.at(0);
        if (expressionToken.getType() == SimpleToken::TokenType::NUMBER) {
            return make_shared<ConstantNode>(expressionToken.getValue());
        } else if (expressionToken.getType() == SimpleToken::TokenType::WORD) {
            return make_shared<VariableNode>(expressionToken.getValue());
        } else {
            char* message = new char[100];
            sprintf(message, "Expected expression. Got %s", expressionToken.getValue().c_str());
            throw SimpleInvalidSyntaxException(message);
        }
    } else if (expression.size() == 3) {
        SimpleToken leftOperandToken = expression.at(0);
        SimpleToken operatorToken = expression.at(1);
        SimpleToken rightOperandToken = expression.at(2);
        if (leftOperandToken.getType() == SimpleToken::TokenType::NUMBER &&
                (operatorToken.getType() == SimpleToken::TokenType::PLUS || operatorToken.getType() == SimpleToken::TokenType::MINUS) &&
            rightOperandToken.getType() == SimpleToken::TokenType::NUMBER) {
            TNode::CONSTANT_NODE_PTR leftOperandNode = make_shared<ConstantNode>(leftOperandToken.getValue());
            TNode::CONSTANT_NODE_PTR rightOperandNode = make_shared<ConstantNode>(rightOperandToken.getValue());
            TNode::BIN_OP_NODE_PTR binaryOperatorNode = make_shared<PlusNode>();
            binaryOperatorNode->setLeftSubtree(leftOperandNode);
            binaryOperatorNode->setRightSubtree(rightOperandNode);
            return binaryOperatorNode;
        } else if (leftOperandToken.getType() == SimpleToken::TokenType::WORD &&
                   (operatorToken.getType() == SimpleToken::TokenType::PLUS || operatorToken.getType() == SimpleToken::TokenType::MINUS) &&
                   rightOperandToken.getType() == SimpleToken::TokenType::WORD) {
            TNode::VARIABLE_NODE_PTR leftOperandNode = make_shared<VariableNode>(leftOperandToken.getValue());
            TNode::VARIABLE_NODE_PTR rightOperandNode = make_shared<VariableNode>(rightOperandToken.getValue());
            TNode::BIN_OP_NODE_PTR binaryOperatorNode = make_shared<PlusNode>();
            binaryOperatorNode->setLeftSubtree(leftOperandNode);
            binaryOperatorNode->setRightSubtree(rightOperandNode);
            return binaryOperatorNode;
        } else if (leftOperandToken.getType() == SimpleToken::TokenType::WORD &&
                (operatorToken.getType() == SimpleToken::TokenType::PLUS || operatorToken.getType() == SimpleToken::TokenType::MINUS) &&
                   rightOperandToken.getType() == SimpleToken::TokenType::NUMBER) {
            TNode::VARIABLE_NODE_PTR leftOperandNode = make_shared<VariableNode>(leftOperandToken.getValue());
            TNode::CONSTANT_NODE_PTR rightOperandNode = make_shared<ConstantNode>(rightOperandToken.getValue());
            TNode::BIN_OP_NODE_PTR binaryOperatorNode = make_shared<PlusNode>();
            binaryOperatorNode->setLeftSubtree(leftOperandNode);
            binaryOperatorNode->setRightSubtree(rightOperandNode);
            return binaryOperatorNode;
        } else if (leftOperandToken.getType() == SimpleToken::TokenType::NUMBER &&
                (operatorToken.getType() == SimpleToken::TokenType::PLUS || operatorToken.getType() == SimpleToken::TokenType::MINUS) &&
                   rightOperandToken.getType() == SimpleToken::TokenType::WORD) {
            TNode::CONSTANT_NODE_PTR leftOperandNode = make_shared<ConstantNode>(leftOperandToken.getValue());
            TNode::VARIABLE_NODE_PTR rightOperandNode = make_shared<VariableNode>(rightOperandToken.getValue());
            TNode::BIN_OP_NODE_PTR binaryOperatorNode = make_shared<PlusNode>();
            binaryOperatorNode->setLeftSubtree(leftOperandNode);
            binaryOperatorNode->setRightSubtree(rightOperandNode);
            return binaryOperatorNode;
        } else {
            char* message = new char[100];
            sprintf(message, "Expected expression. Got %s", currentToken.getValue().c_str());
            throw SimpleInvalidSyntaxException(message);
        }
    } else {
        char* message = new char[100];
        sprintf(message, "Expected expression. Got %s", currentToken.getValue().c_str());
        throw SimpleInvalidSyntaxException(message);
    }
}
