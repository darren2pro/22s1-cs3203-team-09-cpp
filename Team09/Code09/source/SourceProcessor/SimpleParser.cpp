#include <string>

#include "Parser.h"
#include "SimpleTokenizer.h"
#include "astBuilder/SimpleAstBuilder.h"
#include "designExtractions/DesignExtractor.h"
#include "../ProgramKnowledgeBase/PKBManager.h"

using namespace std;

SimpleParser::SimpleParser(istream* program) {
    SimpleTokenizer tokenizer(program);
    tokens = tokenizer.tokenize();
}

PKB::PKBManager SimpleParser::parse() {
    SimpleAstBuilder builder(tokens);
    builder.build();
    PKBManager pkb;
    DesignExtractor extractor(pkb.getPKBStorage());
    extractor.extractDesignAbstractions(builder.getProgramNode());
    return pkb;
}
