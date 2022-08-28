#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "Parser.h"
#include "SimpleTokenizer.h"
#include "validation/SimpleSourceValidator.h"

using namespace std;

SimpleParser::SimpleParser(const string program) {
    SimpleTokenizer tokenizer(program);
    tokens = tokenizer.tokenize();
}

int SimpleParser::parse() {
    // TODO: Validate the source program, then create the AST
    // validate tokens
    SimpleSourceValidator validator(tokens);
    if (validator.validate()) {
        // create AST
        SimpleASTBuilder builder(tokens);
        return builder.build();
    } else {
        return -1;
    }
}
