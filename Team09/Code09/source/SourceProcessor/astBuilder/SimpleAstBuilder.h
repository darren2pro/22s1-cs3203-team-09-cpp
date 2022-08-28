#pragma once

#include "../Parser.h"

/**
 * @brief The SimpleAstBuilder class which validates the input source program.
 */
class SimpleAstBuilder {
private:
    Parser::SOURCE_CODE_TOKENS tokens;

    int currentTokenIndex;

    int currentLineNo;

public:
    explicit SimpleAstBuilder(const Parser::SOURCE_CODE_TOKENS tokens);

    ~SimpleAstBuilder();

    int build();

    /**
     * @brief Deals with the building of the AST for a single procedure block.
     * @details This method will create the necessary nodes for the procedure block and then call other methods
     * as per needed for the statements in the procedure block.
     */
    void handleProcedure();

    void handleProcedureStatement();
};
