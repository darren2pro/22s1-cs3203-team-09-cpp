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
protected:
    vector<SimpleToken> tokens;

public:
    /**
    * @brief Destructor
    */
    virtual ~Parser() {};
	
    /**
     * @brief Parses the input program into tokens
     * @details Parses the input program into tokens, and also creates an AST. This is a pure virtual function which must be implemented in the derived class.
     * @return 0 if successful, -1 otherwise
     */
    virtual int parse() = 0;

    const vector<SimpleToken> &getTokens() const {
        return tokens;
    }
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
