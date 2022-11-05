#include "ArithmeticParser.h"
#include "../Exceptions/SimpleInvalidSyntaxException.h"
#include <string>

SimpleToken* ArithmeticParser::advance() {
    if (!isAtEnd()) {
        (*currentTokenIndex)++;
    }
    return previous();
}

SimpleToken* ArithmeticParser::peek() {
    return tokens.at(*currentTokenIndex);
}

SimpleToken* ArithmeticParser::previous() {
    return tokens.at(*currentTokenIndex - 1);
}

bool ArithmeticParser::isAtEnd() {
    return peek()->getType() == SimpleToken::TokenType::END_OF_FILE;
}

bool ArithmeticParser::check(string s) {
    if (isAtEnd()) return false;
    return peek()->getValue().compare(s) == 0;
}

bool ArithmeticParser::match(string s) {
    if (check(s)) {
        advance();
        return true;
    } else {
        return false;
    }
}

bool ArithmeticParser::expect(string s) {
    if (match(s)) {
        return true;
    } else {
        throw SimpleInvalidSyntaxException("Expected '" + s + "', got '" + peek()->getValue() + "'.");
    }
}

Expr ArithmeticParser::nullDenotation(SimpleToken* token) {
    if (token->getType() == SimpleToken::TokenType::NUMBER) {
        return make_shared<ConstantNode>(token->getValue());
    } else if (token->getType() == SimpleToken::TokenType::WORD) {
        return make_shared<VariableNode>(token->getValue());
    } else if (token->getValue() == "(") {
        Expr expr = parse(0);
        expect(")");
        return expr;
    } else {
        throw SimpleInvalidSyntaxException("Expected an expression, got '" + token->getValue() + "'.");
    }
}

int ArithmeticParser::leftBindingPower(SimpleToken* pToken) {
    if (delimiters.find(pToken->getValue()) != delimiters.end() ||
        pToken->getType() == SimpleToken::TokenType::END_OF_FILE ||
        pToken->getValue() == ")") {
        return 0;
    } else if (pToken->getValue() == "+" || pToken->getValue() == "-") {
        return 10;
    } else if (pToken->getValue() == "*" || pToken->getValue() == "/" || pToken->getValue() == "%") {
        return 20;
    } else {
        throw SimpleInvalidSyntaxException("Unexpected token '" + pToken->getValue() + "'.");
    }
}

Expr ArithmeticParser::leftDenotation(SimpleToken* token, Expr left) {
    if (token->getValue() == "+") {
        Expr right = parse(10);
        return make_shared<BinOpNode>("+", left, right);
    } else if (token->getValue() == "-") {
        Expr right = parse(10);
        return make_shared<BinOpNode>("-", left, right);
    } else if (token->getValue() == "*") {
        Expr right = parse(20);
        return make_shared<BinOpNode>("*", left, right);
    } else if (token->getValue() == "/") {
        Expr right = parse(20);
        return make_shared<BinOpNode>("/", left, right);
    } else if (token->getValue() == "%") {
        Expr right = parse(20);
        return make_shared<BinOpNode>("%", left, right);
    } else {
        throw SimpleInvalidSyntaxException("Left denotation called on an invalid token: " + token->getValue() + "'.");
    }
}

ArithmeticParser::ArithmeticParser(const vector<SimpleToken*> &tokens, int* currentTokenIndex,
                                   const unordered_set<string> delimiters) : currentTokenIndex(currentTokenIndex),
                                                                             tokens(tokens), delimiters(delimiters) {}

Expr ArithmeticParser::parse(int rightBindingPower) {
    SimpleToken* t = advance();
    Expr left = nullDenotation(t);

    while (rightBindingPower < leftBindingPower(peek())) {
        t = advance();
        left = leftDenotation(t, left);
    }

    return left;
}
