#pragma once

#include <string>

using namespace std;

class SimpleToken {
public:
    /**
     * The enums for all the token types in SIMPLE <br><br>
     * NUMBER is restricted to an integer in SIMPLE <br><br>
     * SYMBOL represent variables, and terminals/keywords in SIMPLE such as while, if, procedure, etc. <br><br>
     * PUNCTUATION represents all the punctuation symbols (relational and arithmetic operators, and semi-colons)
     */
    enum class TokenType {
        NUMBER,
        OPEN_PARENTHESES,
        CLOSE_PARENTHESES,
        OPEN_BRACES,
        CLOSE_BRACES,
        SEMICOLON,
        EQUALS,
        LESS_THAN,
        GREATER_THAN,
        LESS_THAN_OR_EQUAL_TO,
        GREATER_THAN_OR_EQUAL_TO,
        NOT_EQUAL_TO,
        PLUS,
        MINUS,
        MULTIPLY,
        DIVIDE,
        ASSIGN,
        WORD, //! variable or procedure names, or invalid word if wrongly placed. It is could also be read,
                //! while, call, because we don't know whether they are keywords or variable names yet.
        PROCEDURE,
        CALL,
        WHILE,
        IF,
        THEN,
        ELSE,
        PRINT,
        READ,
        AND,
        OR,
        NOT,
        MODULO,
        END_OF_FILE
    };

    /**
     * @brief Constructor
     * @details Constructor
     * @param value The token value
     */
    explicit SimpleToken(string value, TokenType type);

    /**
     * @brief Destructor
     * @details Destructor
     */
    ~SimpleToken();

    /**
     * @brief Gets the token type
     * @details Gets the token type
     * @return The token type
     */
    TokenType getType() const;

    /**
     * @brief Gets the token value
     * @details Gets the token value
     * @return The token value
     */
    string getValue() const;

    bool operator==(const SimpleToken &other) const;

private:
    /**
     * @brief The token type
     */
    TokenType type;
    /**
     * @brief The token value
     */
    string value;
};
