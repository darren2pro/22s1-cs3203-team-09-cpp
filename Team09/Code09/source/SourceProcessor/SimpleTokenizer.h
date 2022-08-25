#pragma once

#include "SimpleToken.h"

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
    string program;
public:
    SimpleTokenizer(const string program);

    ~SimpleTokenizer();

    /**
     * @brief Tokenizes the program into a vector of tokens
     * @return The vector of tokens contained in the program
     */
    vector<SimpleToken> tokenize() const;

    /**
     * @brief Tokenizes a word
     * @details A word may not be a token if the sticky terminals are not space-separated from the symbols. E.g. a+b is a word, not a token.
     * Hence I need to tokenize the word into a vector of tokens using this function.
     * @param word
     * @return A vector of tokens obtained from this word
     */
    vector<SimpleToken> tokenizeWord(string word) const;
};
