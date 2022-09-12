#include "ArithmeticParser.h"

ArithmeticParser::ArithmeticParser(const vector<SimpleToken*> &tokens, int* currentTokenIndex,
                                   const unordered_set<string> delimiters) : currentTokenIndex(currentTokenIndex),
                                                                             tokens(tokens), delimiters(delimiters) {}
