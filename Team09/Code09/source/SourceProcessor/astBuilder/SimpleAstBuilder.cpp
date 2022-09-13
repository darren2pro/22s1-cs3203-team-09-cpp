#include "SimpleAstBuilder.h"
#include "../exceptions/SimpleInvalidSyntaxException.h"
#include <unordered_set>

using namespace std;

SimpleAstBuilder::SimpleAstBuilder(const Parser::SOURCE_CODE_TOKENS _tokens) : tokens(_tokens), currentTokenIndex(0),
                                                                               arithmeticParser(tokens,
                                                                                                &currentTokenIndex,
                                                                                                unordered_set<string>(
                                                                                                        {">", ">=", "<",
                                                                                                         "<=", "==",
                                                                                                         "!=", ";",
                                                                                                         ")"})) {}

SimpleAstBuilder::~SimpleAstBuilder() {
}

AST SimpleAstBuilder::build() {
    vector<ProcedureNodePtr> procedureLst;
    while (true) {
        if (match(SimpleToken::TokenType::END_OF_FILE)) {
            break;
        }
        ProcedureNodePtr procedureNode = parseProcedure();
        procedureLst.push_back(procedureNode);
    }
    if (procedureLst.size() == 0) {
        throw SimpleInvalidSyntaxException("Expected at least 1 procedure.");
    }
    return make_shared<ProgramNode>(move(procedureLst));
}

bool SimpleAstBuilder::match(SimpleToken::TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool SimpleAstBuilder::match(string value) {
    if (check(value)) {
        advance();
        return true;
    }
    return false;
}

bool SimpleAstBuilder::check(SimpleToken::TokenType type) {
    return peek()->getType() == type;
}

bool SimpleAstBuilder::check(string value) {
    return peek()->getValue().compare(value) == 0;
}

bool SimpleAstBuilder::expect(string s) {
    if (match(s)) {
        return true;
    }
    throw SimpleInvalidSyntaxException("Expected " + s + ", got " + peek()->getValue() + ".");
}

SimpleToken* SimpleAstBuilder::advance() {
    if (!isAtEnd()) currentTokenIndex++;
    return previous();
}

bool SimpleAstBuilder::isAtEnd() {
    return peek()->getType() == SimpleToken::TokenType::END_OF_FILE;
}

SimpleToken* SimpleAstBuilder::peek() {
    return tokens.at(currentTokenIndex);
}

SimpleToken* SimpleAstBuilder::previous() {
    return tokens.at(currentTokenIndex - 1);
}

ConstantNodePtr SimpleAstBuilder::parseConstant() {
    int currTokenIdx = currentTokenIndex;
    if (match(SimpleToken::TokenType::NUMBER)) {
        string number = previous()->getValue();
        ConstantNodePtr constNode = make_shared<ConstantNode>(number);
        return constNode;
    } else {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }
}

VariableNodePtr SimpleAstBuilder::parseVariable() {
    int currTokenIdx = currentTokenIndex;
    if (match(SimpleToken::TokenType::WORD)) {
        string identifier = previous()->getValue();
        VariableNodePtr varNode = make_shared<VariableNode>(identifier);
        return varNode;
    } else {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }
}

ProcedureNodePtr SimpleAstBuilder::parseProcedure() {
    if (!match("procedure")) {
        throw SimpleInvalidSyntaxException("Expected 'procedure' keyword, got " + peek()->getValue() + ".");
    }

    string procedureName;
    if (match(SimpleToken::TokenType::WORD)) {
        procedureName = static_cast<SimpleToken*>(previous())->getValue();
    } else {
        throw SimpleInvalidSyntaxException("Expected valid procedure name, got " + peek()->getValue() + ".");
    }

    expect("{");
    StmtLst stmtLst = parseStmtLst();
    expect("}");

    return make_shared<ProcedureNode>(procedureName, move(stmtLst));
}

StmtLst SimpleAstBuilder::parseStmtLst() {
    StmtLst stmtLst;
    while (true) {
        optional<Stmt> stmt = parseStatement();
        if (stmt) {
            stmtLst.push_back(move(*stmt));
        } else {
            break;
        }
    }

    if (stmtLst.size() == 0) {
        throw SimpleInvalidSyntaxException("Statement list cannot be empty.");
    }

    return stmtLst;
}

optional<Stmt> SimpleAstBuilder::parseStatement() {
    if (check("}")) {
        // End of statement list
        return nullopt;
    }

    int currTokenIdx = currentTokenIndex;

    CallNodePtr callStmt = parseCall();
    if (callStmt) {
        return callStmt;
    }

    ReadNodePtr readStmt = parseRead();
    if (readStmt) {
        return readStmt;
    }

    PrintNodePtr printStmt = parsePrint();
    if (printStmt) {
        return printStmt;
    }

    WhileNodePtr whileStmt = parseWhile();
    if (whileStmt) {
        return whileStmt;
    }

    IfNodePtr ifStmt = parseIf();
    if (ifStmt) {
        return ifStmt;
    }

    //! Assignment statement is tried last because it can take in any token as the variable being modified
    AssignmentNodePtr assignStmt = parseAssign();
    if (assignStmt) {
        return assignStmt;
    }

    currentTokenIndex = currTokenIdx;
    return nullopt;
}

CallNodePtr SimpleAstBuilder::parseCall() {
    int currTokenIdx = currentTokenIndex;

    if (!match("call")) {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    string procedureName;
    if (match(SimpleToken::TokenType::WORD)) {
        procedureName = previous()->getValue();
    } else {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    expect(";");
    StmtLst emptyStmtLst;
    ProcedureNodePtr procNode = make_shared<ProcedureNode>(procedureName, move(emptyStmtLst));
    return make_shared<CallNode>(move(procNode));
}

ReadNodePtr SimpleAstBuilder::parseRead() {
    int currTokenIdx = currentTokenIndex;
    if (!match("read")) {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    VariableNodePtr variable = parseVariable();
    if (!variable) {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    expect(";");
    return make_shared<ReadNode>(move(variable));
}

PrintNodePtr SimpleAstBuilder::parsePrint() {
    int currTokenIdx = currentTokenIndex;
    if (!match("print")) {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    VariableNodePtr variable = parseVariable();
    if (!variable) {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    expect(";");
    return make_shared<PrintNode>(move(variable));
}

WhileNodePtr SimpleAstBuilder::parseWhile() {
    int currTokenIdx = currentTokenIndex;
    if (!match("while")) {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    if (!match("(")) {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    CondExprNodePtr condExpr = parseCondExpr();
    if (!condExpr) {
        throw SimpleInvalidSyntaxException("Expected condition expression, got " + peek()->getValue() + ".");
    }

    expect(")");
    expect("{");
    StmtLst stmtLst = parseStmtLst();
    expect("}");

    return make_shared<WhileNode>(move(condExpr), move(stmtLst));
}

IfNodePtr SimpleAstBuilder::parseIf() {
    int currTokenIdx = currentTokenIndex;
    if (!match("if")) {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    if (!match("(")) {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    CondExprNodePtr condExpr = parseCondExpr();
    if (!condExpr) {
        throw SimpleInvalidSyntaxException("Expected condition expression, got " + peek()->getValue() + ".");
    }

    expect(")");
    expect("then");
    expect("{");
    StmtLst thenStmtLst = parseStmtLst();
    expect("}");

    expect("else");
    expect("{");
    StmtLst elseStmtLst = parseStmtLst();
    expect("}");

    return make_shared<IfNode>(move(condExpr), move(thenStmtLst), move(elseStmtLst));
}

CondExprNodePtr SimpleAstBuilder::parseCondExpr() {
    if (match("!")) { // !()
        expect("(");
        CondExprNodePtr condExpr = parseCondExpr();
        expect(")");
        return make_shared<CondExprNode>(move(condExpr));
    } else if (check("(")) { // () op ()
        expect("(");
        CondExprNodePtr conditionLhs = parseCondExpr();
        expect(")");

        string op;
        if (match("&&")) {
            op = "&&";
        } else if (match("||")) {
            op = "||";
        } else {
            throw SimpleInvalidSyntaxException("Expected && or ||, got " + peek()->getValue() + ".");
        }

        expect("(");
        CondExprNodePtr conditionRhs = parseCondExpr();
        expect(")");

        return make_shared<CondExprNode>(op, move(conditionLhs), move(conditionRhs));
    } else { // relExpr
        RelExprNodePtr relExpr = parseRelExpr();
        if (relExpr) return make_shared<CondExprNode>(move(relExpr));
    }

    // Neither !(), (), nor relExpr
    return nullptr;
}

RelExprNodePtr SimpleAstBuilder::parseRelExpr() {
    unordered_set<string> validRelOps = {"==", "!=", "<", ">", "<=", ">="};
    RelFactor lhs = parseRelFactor();
    string op = peek()->getValue();

    if (validRelOps.find(op) == validRelOps.end())
        throw SimpleInvalidSyntaxException("Expected relational operator, got " + op + ".");

    advance();
    RelFactor rhs = parseRelFactor();
    // TODO: Check if the move is needed here.
    return make_shared<RelExprNode>(op, move(lhs), move(rhs));
}

RelFactor SimpleAstBuilder::parseRelFactor() {
    Expr expr = parseExpr();
    return expr;
}

Factor SimpleAstBuilder::parseFactor() {
    VariableNodePtr variable = parseVariable();
    if (variable) {
        return variable;
    }

    ConstantNodePtr constant = parseConstant();
    if (constant) {
        return constant;
    }

    expect("(");
    Expr expr = parseExpr();
    expect(")");
    return expr;
}

Expr SimpleAstBuilder::parseExpr() {
    return arithmeticParser.parse(0);
}

AssignmentNodePtr SimpleAstBuilder::parseAssign() {
    VariableNodePtr variable = parseVariable();
    if (!variable) {
        throw SimpleInvalidSyntaxException("Expected variable for assign statement, got " + peek()->getValue() + ".");
    }

    if (!match("=")) {
        throw SimpleInvalidSyntaxException("Expected = for assign statement, got " + peek()->getValue() + ".");
    }

    Expr expr = parseExpr();
    expect(";");
    return make_shared<AssignmentNode>(move(variable), move(expr));
}
