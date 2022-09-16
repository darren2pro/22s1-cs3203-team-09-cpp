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
	std::string expressionSpec = "_|\"(" + synonym + "|" + integer + ")\"";

	std::regex integer_re(integer);
	std::regex design_enteties_re("stmt|read|print|while|if|assign|variable|constant|procedure|call");
	std::regex relation_re("Follows|Follows[\*]|Parent|Parent[\*]|Uses|Modifies");
	std::regex synonym_re(synonym);									// synonym: IDENT	--> IDENT: LETTER (LETTER|DIGIT)*
	std::regex stmtRef_re(stmtRef);									// stmtRef: synonym | '_' | INTEGER
	std::regex entRef_re(entRef);									// endRef: synonym | '_' | '"' IDENT '"'
	std::regex expressionSpec_re(expressionSpec);					// var_name | const_value  * update for next milestones
}

QueryParser::QueryParser(std::vector<std::string> tokens) {
	query_tokens = tokens;
	index = 0;
	current_token = getNextToken();
	declarations = std::vector<Declaration>();
	target = Declaration();
	suchThatCl = Relation();
	patternCl = Pattern();
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
	else if (token == "call") {
		return Declaration::DesignEntity::Call;
	}
	else {
		return Declaration::DesignEntity::NONE;
	}
}

std::vector<Declaration> QueryParser::declaration() {
	std::vector<Declaration> declarations = std::vector<Declaration>();
	std::vector<std::string> names = std::vector<std::string>();

	while (std::regex_match(current_token, parserre::design_enteties_re)) {
		Declaration::DesignEntity type = getDesignEntity(current_token);
		match(parserre::design_enteties_re);

		std::string name = current_token;
		match(parserre::synonym_re);
		declarations.push_back(Declaration::Declaration(type, name));

		if (std::find(names.begin(), names.end(), name) != names.end()) {		// checks for duplicates
			throw SemanticError("Duplicate synonym");
		}
		names.push_back(name);

		while (current_token != ";") {
			match(",");
			std::string name = current_token;
			match(parserre::synonym_re);
			declarations.push_back(Declaration::Declaration(type, name));

			if (std::find(names.begin(), names.end(), name) != names.end()) {		// checks for duplicates
				throw SemanticError("Duplicate synonym");
			}
			names.push_back(name);
		}
		match(";");
	}

	return declarations;
}

Declaration QueryParser::findDeclaration(std::string name) {
	for (Declaration d: declarations) {
		if (d.name == name) {
			return Declaration::Declaration(d.TYPE, d.name);
		}
	}
	throw SemanticError("Synonym not declared");
}

void QueryParser::validate_stmtRef(Relation::Types rel, std::string arg) {
	if (arg == "_" || std::regex_match(arg, parserre::integer_re)) {
		return;
	}

	Declaration d = findDeclaration(arg);

	std::vector<Declaration::DesignEntity> valid_types = std::vector<Declaration::DesignEntity>({Declaration::DesignEntity::Statement, 
															Declaration::DesignEntity::Assignment,Declaration::DesignEntity::If, 
															Declaration::DesignEntity::While, Declaration::DesignEntity::Call});

	if (rel != Relation::Types::Modifies) valid_types.push_back(Declaration::DesignEntity::Print);
	if (rel != Relation::Types::Uses) valid_types.push_back(Declaration::DesignEntity::Read);

	for (Declaration::DesignEntity de : valid_types) {
		if (d.TYPE == de)
			return;
	}
	throw SemanticError("Invalid synonym type used as an argument");
}

void QueryParser::validate_entRef(std::string arg) {
	if (arg == "_" || arg[0] == '"') {
		return;
	}

	Declaration d = findDeclaration(arg);

	std::vector<Declaration::DesignEntity> valid_types = std::vector<Declaration::DesignEntity>({Declaration::DesignEntity::Variable});
	
	for (Declaration::DesignEntity de : valid_types) {
		if (d.TYPE == de)
			return;
	}
	throw SemanticError("Invalid synonym type used as an argument");
}

Declaration QueryParser::select() {
	match("Select");
	std::string target = current_token;
	match(parserre::synonym_re);

	// checks if target is a declared synonym in the declaration list
	Declaration d = findDeclaration(target);

	return d;
}

Pattern QueryParser::patternClause() {
	match("pattern");


	// check syn-assign
	std::string syn_assign = current_token;
	match(parserre::synonym_re);
	Declaration d = findDeclaration(syn_assign);
	if (d.TYPE != Declaration::DesignEntity::Assignment) {
		throw SemanticError("syn-assign is not an Assignment synonym");
	}


	match("(");


	// check the left argument
	std::string left_arg = current_token;
	if (current_token == "_") {		// left argument of pattern can only be either a wildcard or entRef
		match("_");
	}
	else {
		match(parserre::entRef_re);
		validate_entRef(left_arg);
	}


	match(",");


	// check the right argument
	std::string right_arg = "";

	if (current_token == "_") {
		right_arg += current_token;
		match("_");
	}

	if (current_token != ")") {		// check if right_arg matches '_exprssion_'
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


	// check the relation
	Relation::Types type = getType(current_token);
	match(parserre::relation_re);


	match("(");


	// check the left argument
	std::string left_arg = current_token;
	match(parserre::stmtRef_re);
	validate_stmtRef(type, left_arg);


	match(",");


	// check the right argument
	std::string right_arg = current_token;
	if (type == Relation::Types::Uses || type == Relation::Types::UsesT
		|| type == Relation::Types::Modifies || type == Relation::Types::ModifiesT) {		
		
		if (left_arg == "_") {
			throw SemanticError("First args for uses/modifies can't be _");
		}

		match(parserre::entRef_re);
		validate_entRef(right_arg);
	}
	else {
		match(parserre::stmtRef_re);
		validate_stmtRef(type, right_arg);
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
		if (current_token == "such") {
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
