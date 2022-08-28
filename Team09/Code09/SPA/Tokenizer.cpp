#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "SimpleTokenizer.h"
#include "SimpleToken.h"

SimpleTokenizer::SimpleTokenizer(const string program) {
    this->program = program;
}

SimpleTokenizer::~SimpleTokenizer() {
}

vector<SimpleToken> SimpleTokenizer::tokenize() const {
}
