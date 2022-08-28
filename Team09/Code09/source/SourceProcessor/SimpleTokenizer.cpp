#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "SimpleTokenizer.h"
#include "SimpleToken.h"

using namespace std;

SimpleTokenizer::SimpleTokenizer(const string program) {
    this->program = program;
}

SimpleTokenizer::~SimpleTokenizer() {
}

vector<SimpleToken> SimpleTokenizer::tokenizeWord(string word) const {
    char stickyTerminals[] = {'+', '-', '*', '/', '=', '(', ')', '{', '}', ';', ',', '%', '<', '>', '\0'};
    vector<SimpleToken> tokens;
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

vector<SimpleToken> SimpleTokenizer::tokenize() const {
    char delimiters[5] = {' ', '\n', '\t', '\r', '\0'};
    vector<SimpleToken> tokens;
    string word;
    for (int i = 0; i < this->program.length(); i++) {
        if (strchr(delimiters, this->program[i]) != NULL) {
            if (word.length() > 0) {
                vector<SimpleToken> tokenizedWords = SimpleTokenizer::tokenizeWord(word);
                tokens.insert(tokens.end(), tokenizedWords.begin(), tokenizedWords.end());
                word.clear();
            }
        } else {
            word.push_back(this->program[i]);
        }
    }
    if (word.length() > 0) {
        vector<SimpleToken> tokenizedWords = SimpleTokenizer::tokenizeWord(word);
        tokens.insert(tokens.end(), tokenizedWords.begin(), tokenizedWords.end());
    }
    return tokens;
}
