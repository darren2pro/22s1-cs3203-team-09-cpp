#pragma once

#include "../Parser.h"

/**
 * @brief The SimpleAstBuilder class which validates the input source program.
 */
class SimpleAstBuilder {
private:
    Parser::SOURCE_CODE_TOKENS tokens;

    int currentTokenIndex;

    int lineNo;

public:
    explicit SimpleAstBuilder(const Parser::SOURCE_CODE_TOKENS tokens);

    ~SimpleAstBuilder();

    int build();
};
