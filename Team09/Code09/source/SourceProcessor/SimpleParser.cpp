#include <string>

#include "Parser.h"
#include "SimpleTokenizer.h"
#include "astBuilder/SimpleAstBuilder.h"
#include "designExtractions/DesignExtractor.h"
#include "exceptions/SimpleInvalidSyntaxException.h"

using namespace std;

SimpleParser::SimpleParser(istream* program) {
    SimpleTokenizer tokenizer(program);
    tokens = tokenizer.tokenize();
}

PKBStorage SimpleParser::parse() {
    SimpleAstBuilder builder(tokens);
    builder.build();
    PKBStorage pkb;
    DesignExtractor extractor(&pkb);
    // extractor.extractDesignAbstractions(builder.getProgramNode());
    return pkb;
}
