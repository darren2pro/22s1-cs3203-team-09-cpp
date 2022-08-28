#include "SimpleAstBuilder.h"

using namespace std;

SimpleAstBuilder::SimpleAstBuilder(const Parser::SOURCE_CODE_TOKENS tokens) {
    this->tokens = tokens;
}

SimpleAstBuilder::~SimpleAstBuilder() {
}

int SimpleAstBuilder::build() {
    int currentTokenIndex = 0;
    while (currentTokenIndex < tokens.size()) {
        const SimpleToken currentToken = tokens[currentTokenIndex];
//        if (currentToken.getType()) {
//
//        }
//        currentTokenIndex++;
    }

//    for (int i = 0; i < tokens.size(); i++) {
//        // get the current token
//        SimpleToken token = tokens[i];
//    }

    return 0;
}
