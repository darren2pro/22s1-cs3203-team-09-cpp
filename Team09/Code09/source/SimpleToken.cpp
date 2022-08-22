#include "SimpleToken.h"

using namespace std;

SimpleToken::~SimpleToken() {
}

SimpleToken::SimpleToken(const string value) {
    this->value = value;
}

string SimpleToken::getType() const {
    return string();
}

string SimpleToken::getValue() const {
    return string();
}
