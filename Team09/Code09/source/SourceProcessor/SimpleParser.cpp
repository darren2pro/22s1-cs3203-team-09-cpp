#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "Parser.h"
#include "SimpleTokenizer.h"
#include "astBuilder/SimpleAstBuilder.h"

using namespace std;

SimpleParser::SimpleParser(const string program) {
    SimpleTokenizer tokenizer(program);
    tokens = tokenizer.tokenize();
}

int SimpleParser::parse() {
    // TODO: Validate the source program and create the AST at the same time
    SimpleAstBuilder builder(tokens);
    builder.build();
    return -1;
}
