#pragma once

/**
 * @brief The SimpleSourceValidator class which validates the input source program.
 */
class SimpleSourceValidator {
private:
    Parser::SOURCE_CODE_TOKENS tokens;

public:
    explicit SimpleSourceValidator(const Parser::SOURCE_CODE_TOKENS tokens);

    ~SimpleSourceValidator();

    int validate();
};
