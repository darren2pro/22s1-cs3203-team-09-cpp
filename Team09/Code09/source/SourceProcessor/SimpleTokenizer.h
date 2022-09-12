#pragma once

#include "SimpleToken.h"
#include "Parser.h"

using namespace std;

/**
 * @brief SimpleTokenizer class
 * @details SimpleTokenizer class which tokenizes the input program into tokens.
 */
class SimpleTokenizer {
private:
    /**
     * @brief The input program
     */
    istream* program;

    vector<SimpleToken*> tokens;

    size_t lineNo = 1;

    size_t colNo = 0;

    string currentString;

    char peekNextChar();

    char advanceChar();

    void readFullSymbol();

    void readFullNumber();

public:
    explicit SimpleTokenizer(istream* program);

    ~SimpleTokenizer();

    /**
     * @brief Tokenizes the program into a vector of tokens
     * @return The vector of tokens contained in the program
     */
    Parser::SOURCE_CODE_TOKENS tokenize();
};
