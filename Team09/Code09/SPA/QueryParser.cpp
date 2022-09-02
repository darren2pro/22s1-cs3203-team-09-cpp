#include <string>
#include <vector>
#include "QueryParser.h"
#include "QPSValidatorException.h"

std::regex design_enteties("a");
std::regex synonym("a");
std::regex relRef("a");
std::regex stmtRef("a");
std::regex entRef("a");
std::regex expressionSpec("a");

QueryParser::QueryParser(std::vector<std::string> tokens) {
	query_tokens = tokens;
	index = 0;
}

QueryParser::~QueryParser() {

}

std::string QueryParser::getNextToken() {
	std::string token = query_tokens[index];
	index++;
	return token;
}

void QueryParser::match(std::string token) {
	if (current_token == token) {
		current_token = getNextToken();
	} else {
		throw QueryParserException("Expected " + token + " but found " + current_token);
	}
}

void QueryParser::match(std::regex re) {
	if (std::regex_match(current_token, re)) {
		current_token = getNextToken();
	}
	else {
		throw QueryParserException("Unexpected token");
	}
}

std::string QueryParser::declaration() {
	std::string type = current_token;
	match(design_enteties);
	std::string name = current_token;
	match(synonym);

	return name;
}

std::string QueryParser::select() {
	match("Select");
	std::string result = current_token;
	match(synonym);

	return result;
}

std::vector<std::string> QueryParser::patternClause() {
	match("pattern");
	std::string syn_assign = current_token;
	match(synonym);
	match("(");

	std::string left_arg = current_token;
	if (current_token == "_") {
		match("_");
		left_arg += current_token;
	}

	match(entRef);
	match(",");

	std::string right_arg = current_token;

	if (current_token == "_") {
		match("_");
		right_arg += current_token;
	}

	match(expressionSpec);

	if (current_token == "_") {
		right_arg += current_token;
		match("_");
	}

	match(")");

	std::vector<std::string> p;
	p.push_back(syn_assign);
	p.push_back(syn_assign);
	p.push_back(syn_assign);
	return p;
}

std::string QueryParser::suchThatClause() {
	match("such that");
	std::string type = current_token;
	match(relRef);
	match("(");
	std::string left_arg = current_token;
	match(stmtRef);
	match(",");
	std::string right_arg = current_token;
	match(stmtRef);
	match(")");

	//return Relation(type, left_arg, right_arg);
	return "S";
}

Query QueryParser::parse() {
	std::string currentToken;
	std::vector<std::string> synonyms;
	std::vector<std::string> target;
	std::vector<std::string> suchThatCl;
	std::vector<std::string> patternCl;

	current_token = getNextToken();

	while (index < query_tokens.size()) {
		synonyms.push_back(declaration());	// returns a synonym to add to the query list
		
		if (current_token == ";") {		// end of declarations
			break;
		} else {
			match(",");
		}
	}

	match(";");

	if (index < query_tokens.size()) {
		target.push_back(select());	// add to result clause? 
	}

	// such that and pattern clause
	while (index < query_tokens.size()) {
		if (current_token == "such") {
			suchThatCl.push_back(suchThatClause());
		} else if (current_token == "pattern") {
			patternCl.push_back(suchThatClause());
		} else {
			throw QueryParserException("Unexpected token");
		}	
	}

	return Query(synonyms, target, suchThatCl, patternCl);
}