#include <string>

#include "Parser.h"
#include "SimpleTokenizer.h"
#include "astBuilder/SimpleAstBuilder.h"

using namespace std;

SimpleParser::SimpleParser(const string program) {
    SimpleTokenizer tokenizer(program);
    tokens = tokenizer.tokenize();
}


// Consider returning an AST here
int SimpleParser::parse() {
    // TODO: Validate the source program and create the AST at the same time
    SimpleAstBuilder builder(tokens);
    builder.build();
    return -1;
}
