#include "SimpleInterface.h"
#include "Parser.h"
#include "SimpleTokenizer.h"
#include "astBuilder/ArithmeticParser.h"

using namespace std;

AST SimpleInterface::getAstFromFile(string fileName) {
    ifstream file(fileName);
    SimpleParser parser(&file);
    AST ast = parser.parse();
    return ast;
}

AST SimpleInterface::getAstFromProgram(string program) {
    istringstream iss(program);
    SimpleParser parser(&iss);
    AST ast = parser.parse();
    return ast;
}

Expr SimpleInterface::parseExpression(string expression) {
    stringstream ss(expression);
    SimpleTokenizer tokenizer(&ss);
    Parser::SOURCE_CODE_TOKENS tokens = tokenizer.tokenize();
    int currentIndex = 0;
    //! No need for any relational operators delimiters because we only parse arithmetic
    unordered_set<string> delimiters;
    ArithmeticParser arithmeticParser(tokens, &currentIndex, delimiters);
    Expr expr = arithmeticParser.parse(0);
    return expr;
}
