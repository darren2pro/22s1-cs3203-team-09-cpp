#pragma once

#include <iostream>
#include <string>

using namespace std;

/**
 * @brief Parser abstract class
 * @details Parser abstract class which parses the input program into tokens
 */
class Parser {
public:
    /**
    * @brief Destructor
    * @details Destructor
    */
    virtual ~Parser() = 0;

    /**
     * @brief Parses the input program into tokens
     * @details Parses the input program into tokens. This is a pure virtual function which must be implemented in the derived class.
     * @param program The input program
     * @return 0 if successful, -1 otherwise
     */
    virtual int Parse(const string program) = 0;
};
