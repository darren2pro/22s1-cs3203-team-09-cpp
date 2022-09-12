#pragma once

#include "../Parser.h"
#include <unordered_set>
#include <vector>

using namespace std;

/**
 * Pratt parser for SIMPLE arithmetic expressions
 */
class ArithmeticParser {
private:
    /**
     * Current index in tokens. It is a pointer so it can be embedded in the AST builder.
     */
    int* currentTokenIndex;

    /**
     * The vector of lexical tokens. It is a reference so it can be embedded in the AST builder.
     */
    const Parser::SOURCE_CODE_TOKENS& tokens;

    /**
     * A list of delimiters that signify the end of the arithemtic expression.
     */
    const unordered_set<string> delimiters;


public:
    ArithmeticParser(const Parser::SOURCE_CODE_TOKENS& tokens, int* currentTokenIndex, const unordered_set<string> delimiters);
};
