#pragma once

#include "../Parser.h"
#include "../../TNode/TNode.h"
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <optional>

using namespace std;

/**
 * @brief The SimpleAstBuilder class which validates the input source program.
 */
class SimpleAstBuilder {
private:
    Parser::SOURCE_CODE_TOKENS tokens;

    ExprParser exprParser;

    int currentTokenIndex;

    /**
     * @brief Checks if the current token is of the specified type. If yes, it consumes the token and advances the current token index.
     * @param type
     * @return true if the current token is of the specified type, false otherwise
     */
    bool match(SimpleToken::TokenType type);

    /**
     * @brief Checks if the current token is of the specified string value. If yes, it consumes the token and advances the current token index.
     * @param value
     * @return true if the current token is of the specified string value, false otherwise
     */
    bool match(string value);

    /**
     * @brief Checks if the current token is of the specified type.
     * @param type
     * @return true if the current token is of the specified type, false otherwise
     */
    bool check(SimpleToken::TokenType type);

    /**
     * @brief Checks if the current token is of the specified string value.
     * @param value
     * @return true if the current token is of the specified string value, false otherwise
     */
    bool check(string value);

    /**
     * @brief Consumes the current token and advances the current token index.
     */
     SimpleToken* advance();

     /**
      * @brief Checks whether the ast builder is at the end of the tokens already
      * @return true if the ast builder is at the end of the tokens already, false otherwise
      */
     bool isAtEnd();

     /**
      * @brief Returns the current token
      * @return the current token
      */
     SimpleToken* peek();

     /**
      * @brief Returns the previous token
      * @return the previous token
      */
     SimpleToken* previous();

     /**
      * Parses a number expression
      * @return the shared pointer for this constant number
      */
     shared_ptr<ConstantNode> parseConstant();

     /**
      * Parses a variable expression
      * @return the shared pointer for this variable
      */
    shared_ptr<VariableNode> parseVariable();

    shared_ptr<ProcedureNode> parseProcedure();

    StmtLst parseStmtLst();

    optional<Stmt> parseStatement();

    shared_ptr<CallNode> parseCall();

    shared_ptr<ReadNode> parseRead();

    shared_ptr<PrintNode> parsePrint();

    shared_ptr<WhileNode> parseWhile();

    shared_ptr<IfNode> parseIf();

    shared_ptr<AssignmentNode> parseAssign();

    //! Parse factor is used for relational expressions
    Factor parseFactor();

    Expr parseExpr();

    shared_ptr<AssignmentNode> parseAssign();

public:
    explicit SimpleAstBuilder(const Parser::SOURCE_CODE_TOKENS tokens);

    ~SimpleAstBuilder();

    AST build();

    /**
     * @brief Deals with the building of the AST for a single procedure block.
     * @details This method will create the necessary nodes for the procedure block and then call other methods
     * as per needed for the statements in the procedure block.
     */
};
