#include <string>
#include <vector>
#include <regex>
#include "QueryParser.h"
#include "../Relation.h"
#include "../Pattern.h"
#include "../Declaration.h"
#include "QPSValidatorException.h"

namespace parserre {
	std::string synonym = "[a-zA-Z]([a-zA-Z0-9])*";
	std::string integer = "(0|[1-9]([0-9])*)";
	std::string stmtRef = synonym + "|_|" + integer;
	std::string entRef = synonym + "|_|\"" + synonym + "\"";
	std::string expressionSpec = synonym + "|" + integer;


	std::regex design_enteties_re("stmt|read|print|while|if|assign|variable|constant|procedure");
	std::regex relation_re("Follows|Follows*|Parent|Parent*|Uses|Modifies");
	std::regex synonym_re(synonym);									// synonym: IDENT	--> IDENT: LETTER (LETTER|DIGIT)*
	std::regex stmtRef_re(stmtRef);									// stmtRef: synonym | '_' | INTEGER
	std::regex entRef_re(entRef);									// endRef: synonym | '_' | '"' IDENT '"'
	std::regex expressionSpec_re(expressionSpec);					// var_name | INTEGER
}

QueryParser::QueryParser(std::vector<std::string> tokens) {
	query_tokens = tokens;
	index = 0;
	current_token = getNextToken();
}

QueryParser::~QueryParser() {

}

std::string QueryParser::getNextToken() {
	if (index >= query_tokens.size()) {
		return "";
	}

	std::string token = query_tokens[index];
	index++;
	return token;
}

void QueryParser::match(std::string token) {
	if (current_token == token) {
		current_token = getNextToken();
	}
	else {
		throw SyntaxError("Expected " + token + " but found " + current_token);
	}
}

void QueryParser::match(std::regex re) {
	if (std::regex_match(current_token, re)) {
		current_token = getNextToken();
	}
	else {
		throw SyntaxError("Unexpected token");
	}
}

Declaration::DesignEntity QueryParser::getDesignEntity(std::string token) {
	if (token == "stmt") {
		return Declaration::DesignEntity::Statement;
	}
	else if (token == "read") {
		return Declaration::DesignEntity::Read;
	}
	else if (token == "print") {
		return Declaration::DesignEntity::Print;
	}
	else if (token == "while") {
		return Declaration::DesignEntity::While;
	}
	else if (token == "if") {
		return Declaration::DesignEntity::If;
	}
	else if (token == "assign") {
		return Declaration::DesignEntity::Assignment;
	}	
	else if (token == "variable") {
		return Declaration::DesignEntity::Variable;
	}	
	else if (token == "constant") {
		return Declaration::DesignEntity::Constant;
	}	
	else if (token == "procedure") {
		return Declaration::DesignEntity::Procedure;
	}
	else {
		return Declaration::DesignEntity::NONE;
	}
}

std::vector<Declaration> QueryParser::declaration() {
	std::vector<Declaration> declarations = std::vector<Declaration>();

	while (std::regex_match(current_token, parserre::design_enteties_re)) {
		Declaration::DesignEntity type = getDesignEntity(current_token);
		match(parserre::design_enteties_re);

		std::string name = current_token;
		match(parserre::synonym_re);
		declarations.push_back(Declaration::Declaration(type, name));

		while (current_token != ";") {
			match(",");
			std::string name = current_token;
			match(parserre::synonym_re);
			declarations.push_back(Declaration::Declaration(type, name));
		}
		match(";");
	}

	return declarations;
}

std::string QueryParser::select() {
	match("Select");
	std::string target = current_token;
	match(parserre::synonym_re);

	return target;
}

Pattern QueryParser::patternClause() {
	match("pattern");
	std::string syn_assign = current_token;
	match(parserre::synonym_re);
	match("(");

	std::string left_arg = current_token;
	if (current_token == "_") {		// left argument of pattern can only be either a wildcard or entRef
		match("_");
	}
	else {
		match(parserre::entRef_re);
	}

	match(",");

	std::string right_arg = "";

	if (current_token == "_") {
		right_arg += current_token;
		match("_");
	}

	// check if right_arg matches '_exprssion_'
	if (current_token != ")") {
		right_arg += current_token;
		match(parserre::expressionSpec_re);
		match("_");
		right_arg += "_";
	}

	match(")");

	return Pattern(syn_assign, left_arg, right_arg);
}

Relation::Types QueryParser::getType(std::string token) {
	if (token == "Follows") {
		return Relation::Types::Follows;
	}
	else if (token == "Follows*") {
		return Relation::Types::FollowsT;
	}
	else if (token == "Parent") {
		return Relation::Types::Parent;
	}
	else if (token == "Parent*") {
		return Relation::Types::ParentT;
	}
	else if (token == "Uses") {
		return Relation::Types::Uses;
	}
	else if (token == "Modifies") {
		return Relation::Types::Modifies;
	}
	else {
		return Relation::Types::NONE;
	}
}

Relation QueryParser::suchThatClause() {
	match("such");
	match("that");
	Relation::Types type = getType(current_token);
	match(parserre::relation_re);
	match("(");
	std::string left_arg = current_token;
	match(parserre::stmtRef_re);
	match(",");
	std::string right_arg = current_token;
	if (type == Relation::Types::Uses || type == Relation::Types::UsesT
		|| type == Relation::Types::Modifies || type == Relation::Types::ModifiesT) {
		match(parserre::entRef_re);
	}
	else {
		match(parserre::stmtRef_re);
	}
	match(")");

	return Relation(type, left_arg, right_arg);
}

Query* QueryParser::parse() {
	Query* query = new Query();

	declarations = declaration();	// parse declarations
	target = select();				// parse Select statement

	// parse such that and pattern clause
	while (index < query_tokens.size()) {
		if (current_token == "such that") {
			suchThatCl = suchThatClause();
		}
		else if (current_token == "pattern") {
			patternCl = patternClause();
		}
		else {
			throw SyntaxError("Unexpected token");
		}
	}

	query->declarations = declarations;
	query->relations = suchThatCl;
	query->patterns = patternCl;
	query->target = target;

	return query;
}
