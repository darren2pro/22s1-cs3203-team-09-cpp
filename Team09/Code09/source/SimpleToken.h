#pragma once

#include <string>

using namespace std;

class SimpleToken {
private:
/**
     * @brief The token type
     */
    string type;
    /**
     * @brief The token value
     */
    string value;

public:
    /**
     * @brief Constructor
     * @details Constructor
     * @param type The token type
     * @param value The token value
     */
    explicit SimpleToken(const string value);

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
    string getType() const;

    /**
     * @brief Gets the token value
     * @details Gets the token value
     * @return The token value
     */
    string getValue() const;
};
