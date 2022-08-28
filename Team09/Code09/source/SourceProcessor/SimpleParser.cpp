#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "Parser.h"
#include "SimpleTokenizer.h"

using namespace std;

SimpleParser::SimpleParser(const string program) {
    SimpleTokenizer tokenizer(program);
    tokens = tokenizer.tokenize();
}

int SimpleParser::parse() {
    // TODO: Create AST from the token vector
    return 0;
}
