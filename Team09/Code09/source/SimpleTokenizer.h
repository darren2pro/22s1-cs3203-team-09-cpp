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

    vector<SimpleToken> tokenize() const;
};
