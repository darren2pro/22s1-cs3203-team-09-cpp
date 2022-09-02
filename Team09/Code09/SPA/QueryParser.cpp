#include <string>
#include <vector>
#include "QueryParser.h"
#include "Relation.h"
#include "QPSValidatorException.h"

std::regex design_enteties("stmt|read|print|while|if|assign|variable|constant|procedure");
std::regex synonym("[a-zA-Z]([a-zA-Z0-9])*");
std::regex relation("Follows|Follows*|Parent|Parent*|Uses|Modifies");
std::regex stmtRef("*");
std::regex entRef("*");
std::regex expressionSpec("*");

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

Relation::Types QueryParser::getType(std::string token) {
	if (token == "Follows") {
		return Relation::Types::Follow;
	} else if (token == "Follows*") {
		return Relation::Types::FollowStar;
	} else if (token == "Parent") {
		return Relation::Types::Parent;
	} else if (token == "Parent*") {
		return Relation::Types::ParentStar;
	} else if (token == "Uses") {
		return Relation::Types::Uses;
	} else if (token == "Modifies") {
		return Relation::Types::Modifies;
	} else {
		return Relation::Types::NONE;
	}
}

Relation QueryParser::suchThatClause() {
	match("such that");
	Relation::Types type = getType(current_token);
	match(relation);
	match("(");
	std::string left_arg = current_token;
	match(stmtRef);
	match(",");
	std::string right_arg = current_token;
	match(stmtRef);
	match(")");

	return Relation(type, left_arg, right_arg);
}

Query QueryParser::parse() {
	std::string currentToken;
	std::vector<std::string> synonyms;
	std::string target;
	Relation suchThatCl;
	std::vector<std::string> patternCl;
	Query query = Query();

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
		target = select();	// add to result clause? 
	}

	// such that and pattern clause
	while (index < query_tokens.size()) {
		if (current_token == "such") {
			suchThatCl = suchThatClause();
		} else if (current_token == "pattern") {
			patternCl = patternClause();
		} else {
			throw QueryParserException("Unexpected token");
		}	
	}

	query.declarations = synonyms;
	query.relations = suchThatCl;
	query.patterns = patternCl;
	query.target = target;

	return query;
}