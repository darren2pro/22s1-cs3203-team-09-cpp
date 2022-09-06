#include <string>

#include "Parser.h"
#include "SimpleTokenizer.h"
#include "astBuilder/SimpleAstBuilder.h"
#include "designExtractions/DesignExtractor.h"

using namespace std;

SimpleParser::SimpleParser(const string program) {
    SimpleTokenizer tokenizer(program);
    tokens = tokenizer.tokenize();
}

PKB::PKBStorage SimpleParser::parse() {
    SimpleAstBuilder builder(tokens);
    builder.build();
    PKBStorage pkb;
    DesignExtractor extractor(&pkb);
    extractor.extractDesignAbstractions(builder.getProgramNode());
    return pkb;
}
