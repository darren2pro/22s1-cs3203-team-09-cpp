#include "SimpleAstBuilder.h"
#include "../exceptions/SimpleInvalidSyntaxException.h"

using namespace std;

SimpleAstBuilder::SimpleAstBuilder(const Parser::SOURCE_CODE_TOKENS tokens) {
    this->tokens = tokens;
    this->currentTokenIndex = 0;
    this->arithmeticParser = make_shared<ArithmeticParser>(tokens,
                                                     currentTokenIndex,
                                                     unordered_set<string>({">", ">=", "<", "<=", "==", "!=", ";", ")"})
    );
}

SimpleAstBuilder::~SimpleAstBuilder() {
}

AST SimpleAstBuilder::build() {
    vector<shared_ptr<ProcedureNode>> procedureLst;
    while (true) {
        if (match(SimpleToken::TokenType::END_OF_FILE)) {
            break;
        }
        shared_ptr<ProcedureNode> procedureNode = parseProcedure();
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

void SimpleAstBuilder::expect(string s) {
    if (!match(s)) {
        throw SimpleInvalidSyntaxException("Expected " + s + ", got " + peek()->getValue() + ".");
    }
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

shared_ptr<ConstantNode> SimpleAstBuilder::parseConstant() {
    int currTokenIdx = currentTokenIndex;
    if (match(SimpleToken::TokenType::NUMBER)) {
        string number = previous()->getValue();
        shared_ptr<ConstantNode> constNode = make_shared<ConstantNode>(number);
        return constNode;
    } else {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }
}

shared_ptr<VariableNode> SimpleAstBuilder::parseVariable() {
    int currTokenIdx = currentTokenIndex;
    if (match(SimpleToken::TokenType::WORD)) {
        string identifier = previous()->getValue();
        shared_ptr<VariableNode> varNode = make_shared<VariableNode>(identifier);
        return varNode;
    } else {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }
}

shared_ptr<ProcedureNode> SimpleAstBuilder::parseProcedure() {
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

    shared_ptr<CallNode> callStmt = parseCall();
    if (callStmt) {
        return callStmt;
    }

    shared_ptr<ReadNode> readStmt = parseRead();
    if (readStmt) {
        return readStmt;
    }

    shared_ptr<PrintNode> printStmt = parsePrint();
    if (printStmt) {
        return printStmt;
    }

    shared_ptr<WhileNode> whileStmt = parseWhile();
    if (whileStmt) {
        return whileStmt;
    }

    shared_ptr<IfNode> ifStmt = parseIf();
    if (ifStmt) {
        return ifStmt;
    }

    //! Assignment statement is tried last because it can take in any token as the variable being modified
    shared_ptr<AssignmentNode> assignStmt = parseAssign();
    if (assignStmt) {
        return assignStmt;
    }

    currentTokenIndex = currTokenIdx;
    return nullopt;
}

shared_ptr<CallNode> SimpleAstBuilder::parseCall() {
    int currTokenIdx = currentTokenIndex;

    if (!match("call")) {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    string procedureName;
    if (match(SimpleToken::TokenType::WORD)) {
        procedureName = static_cast<SimpleToken*>(previous())->getValue();
    } else {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    expect(";");
    return make_shared<CallNode>(move(procedureName));
}

shared_ptr<ReadNode> SimpleAstBuilder::parseRead() {
    int currTokenIdx = currentTokenIndex;
    if (!match("read")) {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    shared_ptr<VariableNode> variable = parseVariable();
    if (!variable) {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    expect(";");
    return make_shared<ReadNode>(move(variable));
}

shared_ptr<PrintNode> SimpleAstBuilder::parsePrint() {
    int currTokenIdx = currentTokenIndex;
    if (!match("print")) {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    shared_ptr<VariableNode> variable = parseVariable();
    if (!variable) {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    expect(";");
    return make_shared<PrintNode>(move(variable));
}

shared_ptr<WhileNode> SimpleAstBuilder::parseWhile() {
    int currTokenIdx = currentTokenIndex;
    if (!match("while")) {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    if (!match("(")) {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    shared_ptr<CondExprNode> condExpr = parseCondExpr();
    if (!condExpr) {
        throw SimpleInvalidSyntaxException("Expected condition expression, got " + peek()->getValue() + ".");
    }

    expect(")");
    expect("{");
    StmtLst stmtLst = parseStmtLst();
    expect("}");

    return make_shared<WhileNode>(move(condExpr), move(stmtLst));
}

shared_ptr<IfNode> SimpleAstBuilder::parseIf() {
    int currTokenIdx = currentTokenIndex;
    if (!match("if")) {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    if (!match("(")) {
        currentTokenIndex = currTokenIdx;
        return nullptr;
    }

    shared_ptr<CondExprNode> condExpr = parseCondExpr();
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

Factor SimpleAstBuilder::parseFactor() {
    shared_ptr<VariableNode> variable = parseVariable();
    if (variable) {
        return variable;
    }

    shared_ptr<ConstantNode> constant = parseConstant();
    if (constant) {
        return constant;
    }

    expect("(");
    Expr expr = parseExpr();
    expect(")");
    return expr;
}

Expr SimpleAstBuilder::parseExpr() {
    return arithmeticParser->parse(0);
}

shared_ptr<AssignmentNode> SimpleAstBuilder::parseAssign() {
    shared_ptr<VariableNode> variable = parseVariable();
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
