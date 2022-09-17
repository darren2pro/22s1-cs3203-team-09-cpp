#include <string>

#include "Parser.h"
#include "SimpleTokenizer.h"
#include "astBuilder/SimpleAstBuilder.h"
#include "designExtractions/DesignExtractor.h"
#include "exceptions/SimpleInvalidSyntaxException.h"
#include "../ProgramKnowledgeBase/PKBManager.h"

using namespace std;

SimpleParser::SimpleParser(istream* program) {
    SimpleTokenizer tokenizer(program);
    tokens = tokenizer.tokenize();
}

PKBManager SimpleParser::parse() {
    SimpleAstBuilder builder(tokens);
    PKBManager pkb;
    DesignExtractor extractor(pkb.getPKBStorage());
    extractor.extractDesignAbstractions(builder.build());
    return pkb;
}
