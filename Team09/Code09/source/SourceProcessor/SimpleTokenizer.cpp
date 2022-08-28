#pragma once

#include <iostream>
#include <string>
#include "SimpleTokenizer.h"
#include "SimpleToken.h"
#include "Parser.h"

using namespace std;

SimpleTokenizer::SimpleTokenizer(const string program) {
    this->program = program;
}

SimpleTokenizer::~SimpleTokenizer() {
}

Parser::SOURCE_CODE_TOKENS SimpleTokenizer::tokenizeWord(string word) const {
    char stickyTerminals[12] = {'+', '-', '*', '/', '=', '(', ')', '{', '}', ';', ',', '\0'};
    Parser::SOURCE_CODE_TOKENS tokens;
    string tokenValue = "";
    for (int i = 0; i < word.length(); i++) {
        const char currentChar = word[i];
        if (strchr(stickyTerminals, currentChar) != NULL) {
            if (tokenValue.length() > 0) {
                tokens.push_back(SimpleToken(tokenValue));
                tokenValue.clear();
            }
            tokens.push_back(SimpleToken(string(1, currentChar)));
        } else {
            tokenValue += currentChar;
        }
    }
    if (tokenValue.length() > 0) {
        tokens.push_back(SimpleToken(tokenValue));
    }
    return tokens;
}

Parser::SOURCE_CODE_TOKENS SimpleTokenizer::tokenize() const {
    char delimiters[5] = {' ', '\n', '\t', '\r', '\0'};
    Parser::SOURCE_CODE_TOKENS tokens;
    string word;
    for (int i = 0; i < this->program.length(); i++) {
        if (strchr(delimiters, this->program[i]) != NULL) {
            if (word.length() > 0) {
                Parser::SOURCE_CODE_TOKENS tokenizedWords = SimpleTokenizer::tokenizeWord(word);
                tokens.insert(tokens.end(), tokenizedWords.begin(), tokenizedWords.end());
                word.clear();
            }
        } else {
            word.push_back(this->program[i]);
        }
    }
    if (word.length() > 0) {
        Parser::SOURCE_CODE_TOKENS tokenizedWords = SimpleTokenizer::tokenizeWord(word);
        tokens.insert(tokens.end(), tokenizedWords.begin(), tokenizedWords.end());
    }
    return tokens;
}
