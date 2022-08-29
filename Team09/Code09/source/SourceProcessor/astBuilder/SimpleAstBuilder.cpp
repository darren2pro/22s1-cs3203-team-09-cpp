#include "SimpleAstBuilder.h"
#include "../exceptions/SimpleInvalidSyntaxException.h"
#include "ProgramNode.h"

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
        const SimpleToken currentToken = tokens[currentTokenIndex];
        if (currentToken.getType() != SimpleToken::TokenType::PROCEDURE) {
            char* message = new char[100];
            sprintf(message, "Expected procedure token. Got %s", currentToken.getValue().c_str());
            throw SimpleInvalidSyntaxException(message);
            return -1;
        }
        currentTokenIndex++;
        handleProcedure();
    }
    return 0;
}

void SimpleAstBuilder::handleProcedure() {
    const SimpleToken procedureToken = tokens[currentTokenIndex];
    if (procedureToken.getType() != SimpleToken::TokenType::WORD) {
        char* message = new char[100];
        sprintf(message, "Expected procedure name. Got %s", procedureToken.getValue().c_str());
        throw SimpleInvalidSyntaxException(message);
    }
    currentTokenIndex++;
    const SimpleToken leftBracesToken = tokens[currentTokenIndex];
    if (leftBracesToken.getType() != SimpleToken::TokenType::OPEN_BRACES) {
        char* message = new char[100];
        sprintf(message, "Expected open braces. Got %s", leftBracesToken.getValue().c_str());
        throw SimpleInvalidSyntaxException(message);
    }
    currentTokenIndex++;
    while (tokens[currentTokenIndex] != SimpleToken::TokenType::CLOSE_BRACES) {
        handleProcedureStatement();
    }
//    while (currentTokenIndex < tokens.size()) {
//        const SimpleToken currentToken = tokens[currentTokenIndex];
//        if (currentToken.getType() == SimpleToken::TokenType::RIGHT_CURLY) {
//            currentTokenIndex++;
//            break;
//        }
//        handleStatement();
//    }
}

void SimpleAstBuilder::handleProcedureStatement() {
const SimpleToken currentToken = tokens[currentTokenIndex];
    if (currentToken.getType() == SimpleToken::TokenType::WORD) {
        currentTokenIndex++;
        const SimpleToken equalToken = tokens[currentTokenIndex];
        if (equalToken.getType() != SimpleToken::TokenType::EQUAL) {
            char* message = new char[100];
            sprintf(message, "Expected equal token. Got %s", equalToken.getValue().c_str());
            throw SimpleInvalidSyntaxException(message);
        }
        currentTokenIndex++;
        const SimpleToken valueToken = tokens[currentTokenIndex];
        if (valueToken.getType() != SimpleToken::TokenType::WORD) {
            char* message = new char[100];
            sprintf(message, "Expected value token. Got %s", valueToken.getValue().c_str());
            throw SimpleInvalidSyntaxException(message);
        }
        currentTokenIndex++;
    } else if (currentToken.getType() == SimpleToken::TokenType::OPEN_BRACES) {
        currentTokenIndex++;
        while (tokens[currentTokenIndex] != SimpleToken::TokenType::CLOSE_BRACES) {
            handleStatement();
        }
        currentTokenIndex++;
    } else {
        char* message = new char[100];
        sprintf(message, "Expected procedure statement. Got %s", currentToken.getValue().c_str());
        throw SimpleInvalidSyntaxException(message);
    }
}
