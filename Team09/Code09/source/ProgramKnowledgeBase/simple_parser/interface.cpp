#include "interface.h"
#include "lexer.h"
#include "parser.h"
#include "pratt.h"
#include "../../TNode/TNode.h"

#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>

using Simple::SimpleInterface;

//AST SimpleInterface::getAstFromFile(std::string filename) {
//  std::ifstream input(filename);
//
//  Lexer lexer = Lexer(&input);
//  auto tokens = lexer.lex();
//
//  Parser parser = Parser(tokens);
//  AST ast = parser.parse();
//  return ast;
//}

Expr SimpleInterface::parseExpression(std::string expression) {
  std::stringstream s(expression);
  Lexer lexer = Lexer(&s);
  auto tokens = lexer.lex();

  int current = 0;
  std::unordered_set<std::string> delimiters;

  ExprParser parser = ExprParser(tokens, &current, delimiters);
  Expr expr = parser.parse(0);

  return expr;
}