/*!
 * \file   interface.h
 * \brief  Interface for the SIMPLE language.
 *
 * The bridging interface for SIMPLE subcomponents with the SPA manager.
 *
 * \date   2019-02-15
 */

#pragma once
#include <memory>

#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "../../TNode/TNode.h"

namespace Simple {
struct SimpleInterface {
 public:
  //static AST getAstFromFile(std::string filename);
  static Expr parseExpression(std::string expression);
};
}  // namespace Simple
