#include "SimpleToken.h"

using namespace std;

SimpleToken::SimpleToken(string value) {
    this->value = value;
}

SimpleToken::~SimpleToken() { }

string SimpleToken::getType() const {
    return this->type;
}

string SimpleToken::getValue() const {
    return this->value;
}
