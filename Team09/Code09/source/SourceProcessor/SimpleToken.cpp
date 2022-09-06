#include "SimpleToken.h"
#include <regex>

using namespace std;

SimpleToken::SimpleToken(string value, TokenType type) {
    this->value = value;
    this->type = type;
}

SimpleToken::~SimpleToken() {}

SimpleToken::TokenType SimpleToken::getType() const {
    return this->type;
}

string SimpleToken::getValue() const {
    return this->value;
}

bool SimpleToken::operator==(const SimpleToken &other) const {
    return this->type == other.type && this->value.compare(other.value) == 0;
}
