#include <string>

#include "SimpleParser.h"
#include "SimpleTokenizer.h"
#include "../astBuilder/SimpleAstBuilder.h"
#include "../designExtractions/DesignExtractor.h"
#include "../exceptions/SimpleInvalidSyntaxException.h"

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
