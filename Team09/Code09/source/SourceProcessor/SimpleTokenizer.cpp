#pragma once

#include <iostream>
#include <cctype>
#include <unordered_set>
#include <string>
#include <vector>
#include "SimpleTokenizer.h"
#include "SimpleToken.h"
#include "exceptions/SimpleInvalidSyntaxException.h"

using namespace std;

std::unordered_set<std::string> SINGLE_TOKEN_PUNCTUATIONS({"{", "}", "(", ")", "+",
                                                           "-", "*", "/", "%", ";"});
SimpleToken::TokenType mapSingleCharPunctuationToTokenType(char c) {
    switch (c) {
        case '{':
            return SimpleToken::TokenType::OPEN_BRACES;
            break;
        case '}':
            return SimpleToken::TokenType::CLOSE_BRACES;
            break;
        case '(':
            return SimpleToken::TokenType::OPEN_PARENTHESES;
            break;
        case ')':
            return SimpleToken::TokenType::CLOSE_PARENTHESES;
            break;
        case '+':
            return SimpleToken::TokenType::PLUS;
            break;
        case '-':
            return SimpleToken::TokenType::MINUS;
            break;
        case '*':
            return SimpleToken::TokenType::MULTIPLY;
            break;
        case '/':
            return SimpleToken::TokenType::DIVIDE;
            break;
        case '%':
            return SimpleToken::TokenType::MODULO;
            break;
        case ';':
            return SimpleToken::TokenType::SEMICOLON;
            break;
        default:
            throw SimpleInvalidSyntaxException("Invalid punctuation");
    }
}

char SimpleTokenizer::peekNextChar() {
    return program->peek();;
}

char SimpleTokenizer::advanceChar() {
    colNo++;
    return program->get();
}

void SimpleTokenizer::readFullSymbol() {
    while (isalnum(peekNextChar()) != 0) {
        currentString += advanceChar();
    }
}

void SimpleTokenizer::readFullNumber() {
    while (isdigit(peekNextChar()) != 0) {
        currentString += advanceChar();
    }
}

SimpleTokenizer::SimpleTokenizer(istream* program) {
    this->program = program;
}

SimpleTokenizer::~SimpleTokenizer() {
}

Parser::SOURCE_CODE_TOKENS SimpleTokenizer::tokenize() {
    char nextChar;
    while (!program->eof()) {
        nextChar = advanceChar();
        if (nextChar == EOF) {
            break;
        }
        currentString += nextChar;
        if (nextChar == '\n') {
            lineNo++;
            colNo = 1;
        } else if (isspace(nextChar) != 0) {
        } else if (isalpha(nextChar) != 0) {  // Symbols
            readFullSymbol();
            tokens.push_back(new SimpleToken(currentString, SimpleToken::TokenType::WORD));
        } else if (isdigit(nextChar) != 0) {  // Numbers
            readFullNumber();
            tokens.push_back(new SimpleToken(currentString, SimpleToken::TokenType::NUMBER));
        } else {  // Punctuations
            if (nextChar == '!') {
                if (peekNextChar() == '=') {
                    currentString += advanceChar();
                    tokens.push_back(new SimpleToken(currentString, SimpleToken::TokenType::NOT_EQUAL_TO));
                } else {
                    // It is something like (!  (1>  x ) )
                    tokens.push_back(new SimpleToken(currentString, SimpleToken::TokenType::NOT));
                }
            } else if (nextChar == '=') {
                if (peekNextChar() == '=') {
                    currentString += advanceChar();
                    tokens.push_back(new SimpleToken(currentString, SimpleToken::TokenType::EQUALS));
                } else {
                    tokens.push_back(new SimpleToken(currentString, SimpleToken::TokenType::ASSIGN));
                }
            } else if (nextChar == '&') {
                if (peekNextChar() == '&') {
                    currentString += advanceChar();
                    tokens.push_back(new SimpleToken(currentString, SimpleToken::TokenType::AND));
                } else {
                    throw SimpleInvalidSyntaxException("Invalid character after &");
                }
            } else if (nextChar == '|') {
                if (peekNextChar() == '|') {
                    currentString += advanceChar();
                    tokens.push_back(new SimpleToken(currentString, SimpleToken::TokenType::OR));
                } else {
                    throw SimpleInvalidSyntaxException("Invalid character after |");
                }
            } else if (nextChar == '<') {
                if (peekNextChar() == '=') {
                    currentString += advanceChar();
                    tokens.push_back(new SimpleToken(currentString, SimpleToken::TokenType::LESS_THAN_OR_EQUAL_TO));
                } else {
                    tokens.push_back(new SimpleToken(currentString, SimpleToken::TokenType::LESS_THAN));
                }
            } else if (nextChar == '>') {
                if (peekNextChar() == '=') {
                    currentString += advanceChar();
                    tokens.push_back(new SimpleToken(currentString, SimpleToken::TokenType::GREATER_THAN_OR_EQUAL_TO));
                } else {
                    tokens.push_back(new SimpleToken(currentString, SimpleToken::TokenType::GREATER_THAN));
                }
            } else if (SINGLE_TOKEN_PUNCTUATIONS.find(currentString) != SINGLE_TOKEN_PUNCTUATIONS.end()) {
                SimpleToken::TokenType tokenType = mapSingleCharPunctuationToTokenType(nextChar);
                tokens.push_back(new SimpleToken(currentString, tokenType));
            } else {
                throw SimpleInvalidSyntaxException("Invalid character");
            }
        }
        currentString = "";
    }
    tokens.push_back(new SimpleToken("EOF", SimpleToken::TokenType::END_OF_FILE));
    return tokens;
}
