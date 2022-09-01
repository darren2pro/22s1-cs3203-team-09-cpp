#include "SimpleToken.h"
#include <regex>

using namespace std;

SimpleToken::SimpleToken(string value) {
    this->value = value;
    // switch statement for the value of token
    if (value == "read") {
        this->type = TokenType::READ;
    } else if (value == "print") {
        this->type = TokenType::PRINT;
    } else if (value == "call") {
        this->type = TokenType::CALL;
    } else if (value == "while") {
        this->type = TokenType::WHILE;
    } else if (value == "if") {
        this->type = TokenType::IF;
    } else if (value == "then") {
        this->type = TokenType::THEN;
    } else if (value == "else") {
        this->type = TokenType::ELSE;
    } else if (value == "procedure") {
        this->type = TokenType::PROCEDURE;
    } else if (value == ";") {
        this->type = TokenType::SEMICOLON;
    } else if (value == "=") {
        this->type = TokenType::ASSIGN;
    } else if (value == "<") {
        this->type = TokenType::LESS_THAN;
    } else if (value == ">") {
        this->type = TokenType::GREATER_THAN;
    } else if (value == "<=") {
        this->type = TokenType::LESS_THAN_OR_EQUAL_TO;
    } else if (value == ">=") {
        this->type = TokenType::GREATER_THAN_OR_EQUAL_TO;
    } else if (value == "==") {
        this->type = TokenType::EQUALS;
    } else if (value == "!=") {
        this->type = TokenType::NOT_EQUAL_TO;
    } else if (value == "+") {
        this->type = TokenType::PLUS;
    } else if (value == "-") {
        this->type = TokenType::MINUS;
    } else if (value == "*") {
        this->type = TokenType::MULTIPLY;
    } else if (value == "/") {
        this->type = TokenType::DIVIDE;
    } else if (value == "(") {
        this->type = TokenType::OPEN_PARENTHESES;
    } else if (value == ")") {
        this->type = TokenType::CLOSE_PARENTHESES;
    } else if (value == "{") {
        this->type = TokenType::OPEN_BRACES;
    } else if (value == "}") {
        this->type = TokenType::CLOSE_BRACES;
    } else if (regex_match(value, regex("[0-9]+"))) {
        this->type = TokenType::NUMBER;
    } else {
        // it is a random variable name
        this->type = TokenType::WORD;
    }
}

SimpleToken::~SimpleToken() {}

SimpleToken::TokenType SimpleToken::getType() const {
    return this->type;
}

string SimpleToken::getValue() const {
    return this->value;
}
