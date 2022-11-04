#pragma once

#include <sstream>
#include <string>
#include <fstream>
#include "../TNode/TNode.h"

using namespace std;

struct SimpleInterface {
    static AST getAstFromProgram(string program);
    /**
     * @brief Builds an arithmetic AST from a given string, using the Pratt algorithm
     * @details This method is generally only used by the PKB to build arithmetic ASTs from the user query arguments,
     * then compare subtrees with the source program AST
     */
    static Expr parseExpression(string expression);
};
