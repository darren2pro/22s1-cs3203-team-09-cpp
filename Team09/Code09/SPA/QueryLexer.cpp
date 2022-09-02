#include <cctype>
#include "QueryLexer.h"
#include <string>
#include <vector>
#include <regex>
#include "QPSValidatorException.h""

QueryLexer::QueryLexer(std::string query) {
	query_string = query;
}

QueryLexer::~QueryLexer() {

}

std::vector<std::string> QueryLexer::lex() {
    std::vector<std::string> tokens;
	
    std::regex whitespace("[ \t]+");
    std::regex punctuation("[(),;_]");
    
    std::string str = "";

	for (int i = 0; i < query_string.length(); i++) {
        const char currentChar = query_string[i];
        if (std::regex_match(std::string(1, currentChar), whitespace)) {
            if (str == "such") {        // the only time we want to add a space to tokens is for the 'such that' token
                str += currentChar;
            } else if (str.length() > 0) {
                tokens.push_back(str);
                str.clear();
            }
        } else if (std::regex_match(std::string(1, currentChar), punctuation)) {
            if (str.length() > 0) {
                if (str[0] == '"' && (currentChar == '(' || currentChar == ')')) {    // check if it's part of an expression
                    str += currentChar;
                } else {
                    tokens.push_back(str);
                    tokens.push_back(std::string(1, currentChar));
                    str.clear();
                }
            } else {
                tokens.push_back(std::string(1, currentChar));
            }
        } else if (currentChar == '"') {
            str += currentChar;
            if (str.length() > 0 && str[0] == '"') {
                tokens.push_back(str);
                str.clear();
            }
        } else if (isalnum(currentChar) || currentChar == '*') {
             str += currentChar;
        } else {
            throw QueryLexerException("Unexpected character (" + std::string(1, currentChar) + ") while tokenizing\n");
        }
	}

	return tokens;
}
