#pragma once

#include "../Parser.h"

/**
 * @brief The SimpleAstBuilder class which validates the input source program.
 */
class SimpleAstBuilder {
private:
    Parser::SOURCE_CODE_TOKENS tokens;

public:
    explicit SimpleAstBuilder(const Parser::SOURCE_CODE_TOKENS tokens);

    ~SimpleAstBuilder();

    int build();
};
