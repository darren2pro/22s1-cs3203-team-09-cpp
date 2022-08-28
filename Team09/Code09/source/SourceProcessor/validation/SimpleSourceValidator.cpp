#include "SimpleSourceValidator.h"
#include "../Parser.h"

using namespace std;

SimpleSourceValidator::SimpleSourceValidator(const Parser::SOURCE_CODE_TOKENS tokens) {
    this->tokens = tokens;
}

SimpleSourceValidator::~SimpleSourceValidator() {
}

int SimpleSourceValidator::validate() {
    // TODO: Implement this
    return 0;
}
