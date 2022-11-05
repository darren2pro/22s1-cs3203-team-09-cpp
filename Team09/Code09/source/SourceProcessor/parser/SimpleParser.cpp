#include <string>

#include "SimpleParser.h"
#include "SimpleTokenizer.h"
#include "../ASTBuilder/SimpleAstBuilder.h"
#include "../Exceptions/SimpleInvalidSyntaxException.h"

using namespace std;

SimpleParser::SimpleParser(istream* program) {
    SimpleTokenizer tokenizer(program);
    tokens = tokenizer.tokenize();
}

AST SimpleParser::parse() {
    SimpleAstBuilder builder(tokens);
    AST ast = builder.build();
    return ast;
}
SimpleParser::~SimpleParser() {
    for (SimpleToken* token : tokens) {
        delete token;
    }
}
