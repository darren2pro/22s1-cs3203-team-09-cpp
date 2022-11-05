#pragma once

#include "../Parser/SimpleParser.h"
#include "../TNode/TNode.h"
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
    const SimpleParser::SOURCE_CODE_TOKENS &tokens;

    /**
     * A list of delimiters that signify the end of the arithemtic expression.
     */
    const unordered_set<string> delimiters;

    /**
     * Returns the current token and advances the current token index number (from the AstBuilder) using de-referencing.
     */
    SimpleToken* advance();

    /**
     * Returns the current token without advancing the current token index number.
     */
    SimpleToken* peek();

    /**
     * Returns the previous token without advancing the current token index number.
     */
    SimpleToken* previous();

    /**
     * Checks if the current token is the last token.
     */
    bool isAtEnd();

    /**
     * Checks if the current token is of the specified string value. Does not try to change the token index.
     */
    bool check(string s);

    /**
     * Checks if the current token is of the specified string value. If yes, it consumes the token and advances the current token index.
     * If no, it returns false and leaves the token index unchanged.
     */
    bool match(string s);

    /**
     * Asserts the current token at the current index to be of string s. Consumes the token if true, else throws an exception.
     * This method is essentially the same as match, but it incorporates an exception if the current token is not the expected one.
     */
    bool expect(string s);

    /**
     * @brief Null denotation function for the current token.
     * @details The null denotation function is the function that is called when the current token is the first token in the expression.
     * This method does not care about the tokens to the left. This method is used by values (constants and variables) and prefix operators such as !.
     * This method might call the parse function as well.
     */
    Expr nullDenotation(SimpleToken* token);

    /**
     * @brief This method calculates the left binding power for the given token.
     * @param pToken
     * @return The left binding power for the given token.
     */
    int leftBindingPower(SimpleToken* pToken);

    /**
     * @brief Left denotation method for the given token.
     * @details The left denotation method is used to process infix (such as +, *) and suffix operators.
     * This method might call the parse function as well.
     */
    Expr leftDenotation(SimpleToken* token, Expr left);

public:
    ArithmeticParser(const SimpleParser::SOURCE_CODE_TOKENS &tokens, int* currentTokenIndex,
                     const unordered_set<string> delimiters);

    /**
     * @brief Parses the arithmetic expression.
     * @param rightBindingPower The right binding power of the current token.
     * @return The root of the arithmetic expression tree.
     */
    Expr parse(int rightBindingPower);
};
