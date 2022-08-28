#pragma once

#include <iostream>
#include <string>
#include "SimpleToken.h"
#include <vector>

using namespace std;

/**
 * @brief Parser abstract class
 * @details Parser abstract class which parses the input program into tokens, and then generates the AST.
 */
class Parser {
public:
    typedef vector<SimpleToken> SOURCE_CODE_TOKENS;

    /**
     * @brief Destructor
     */
    virtual ~Parser() {};

    /**
     * @brief Parses the source code tokens into an abstract syntax tree
     * @details Parses the input source to validate that it is syntactically correct, and create an AST.
     * This is a pure virtual function which must be implemented in the derived class.
     * @return 0 if successful, -1 otherwise
     */
    virtual int parse() = 0;

    const SOURCE_CODE_TOKENS &getTokens() const {
        return tokens;
    }

protected:
    SOURCE_CODE_TOKENS tokens;
};

// Below are the definitions of the subclasses
/**
 * @brief Parser for the SIMPLE language
 */
class SimpleParser : public Parser {
public:
    explicit SimpleParser(const string program);

    int parse() override;
};
