#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cctype>

#include "SimpleToken.h"
#include "../../TNode/TNode.h"

using namespace std;

/**
 * @brief Parser for the SIMPLE language. Parses the input program into tokens, and then generates the AST.
 */
class SimpleParser {
private:
    istream* program;

public:
    typedef vector<SimpleToken*> SOURCE_CODE_TOKENS;

    explicit SimpleParser(istream* program);

    //! Destructor for SimpleParser
    ~SimpleParser();

    const SOURCE_CODE_TOKENS& getTokens() const {
        return tokens;
    }

    /**
    * @brief Parses the source code tokens into an abstract syntax tree
    * @details Parses the input source to build that it is syntactically correct, and create an AST.
    * This is a pure virtual function which must be implemented in the derived class.
    * @return 0 if successful, -1 otherwise
    */
    AST parse();

protected:
    SOURCE_CODE_TOKENS tokens;
};
