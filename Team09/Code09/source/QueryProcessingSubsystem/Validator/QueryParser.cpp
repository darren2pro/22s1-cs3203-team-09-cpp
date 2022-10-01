#include <string>
#include <vector>
#include <regex>
#include "QueryParser.h"
#include "../Relation.h"
#include "../Pattern.h"
#include "../Declaration.h"
#include "../Expression.h"
#include "../Reference.h"
#include "SyntaxException.h"
#include "SemanticException.h"

namespace parser {
	std::string synonym = "[a-zA-Z]([a-zA-Z0-9])*";
	std::string integer = "(0|[1-9]([0-9])*)";
	std::string stmtRef = synonym + "|_|" + integer;
	std::string entRef = synonym + "|_|\"" + synonym + "\"";
	std::string expressionSpec = "_|\"(.*)\"";						// The expression will be validated using the AST parser
	//std::string expressionSpec = "_|\"(" + synonym + "|" + integer + ")\"";

	std::regex integer_re(integer);
	std::regex design_enteties_re("stmt|read|print|while|if|assign|variable|constant|procedure|call");
	std::regex relation_re("Follows|Follows[\*]|Parent|Parent[\*]|Uses|Modifies|Calls|Calls[\*]");
	std::regex synonym_re(synonym);									// synonym: IDENT	--> IDENT: LETTER (LETTER|DIGIT)*
	std::regex stmtRef_re(stmtRef);									// stmtRef: synonym | '_' | INTEGER
	std::regex entRef_re(entRef);									// endRef: synonym | '_' | '"' IDENT '"'
	std::regex expressionSpec_re(expressionSpec);					// _ | '"' (*) '"' 

	// valid stmtRef synonym types for Follows/Parent
	std::vector<Declaration::DesignEntity> stmtRef_de = std::vector<Declaration::DesignEntity>({ Declaration::DesignEntity::Statement,	
															Declaration::DesignEntity::Assignment,Declaration::DesignEntity::If,
															Declaration::DesignEntity::While, Declaration::DesignEntity::Call,
															Declaration::DesignEntity::Read , Declaration::DesignEntity::Print });
	// valid stmtRef synonym types for ModifiesS
	std::vector<Declaration::DesignEntity> stmtRef_Modifies_de = std::vector<Declaration::DesignEntity>({ Declaration::DesignEntity::Statement,
															Declaration::DesignEntity::Assignment,Declaration::DesignEntity::If,
															Declaration::DesignEntity::While, Declaration::DesignEntity::Call, 
															Declaration::DesignEntity::Read });
	// valid stmtRef synonym types for UsesS
	std::vector<Declaration::DesignEntity> stmtRef_Uses_de = std::vector<Declaration::DesignEntity>({ Declaration::DesignEntity::Statement,
															Declaration::DesignEntity::Assignment,Declaration::DesignEntity::If,
															Declaration::DesignEntity::While, Declaration::DesignEntity::Call,
															Declaration::DesignEntity::Print });
	// valid (first arg) entRef synonym types for UsesP/ModifiesP/Calls
	std::vector<Declaration::DesignEntity> entRef_Proc_de = std::vector<Declaration::DesignEntity>({ Declaration::DesignEntity::Procedure });
	// valid (second arg) entRef synonym types for UsesP/ModifiesP/Calls & pattern
	std::vector<Declaration::DesignEntity> entRef_Var_de = std::vector<Declaration::DesignEntity>({ Declaration::DesignEntity::Variable });
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

std::string QueryParser::match(std::string token) {
	if (current_token == token) {
		std::string temp = current_token;
		current_token = getNextToken();
		return temp;
	}
	else {
		throw SyntaxError("Expected " + token + " but found " + current_token);
	}
}

std::string QueryParser::match(std::regex re) {
	if (std::regex_match(current_token, re)) {
		std::string temp = current_token;
		current_token = getNextToken();
		return temp;
	}
	else {
		throw SyntaxError("Unexpected token");
	}
}

std::vector<Declaration> QueryParser::declaration() {
	std::vector<Declaration> declarations = std::vector<Declaration>();
	std::vector<std::string> names = std::vector<std::string>();

	while (std::regex_match(current_token, parser::design_enteties_re)) {
		Declaration::DesignEntity type = Declaration::getDesignEntity(match(parser::design_enteties_re));

		 std::string name = match(parser::synonym_re);
		declarations.push_back(Declaration::Declaration(type, name));

		if (std::find(names.begin(), names.end(), name) != names.end()) {		// checks for duplicates
			throw SemanticError("Duplicate synonym");
		}
		names.push_back(name);

		while (current_token != ";") {	// multiple declarations of the same type
			match(",");
			 std::string name = match(parser::synonym_re);
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
			return d;
		}
	}
	throw SemanticError("Synonym not declared");
}

bool QueryParser::is_valid_stmtRef(Reference ref, std::vector<Declaration::DesignEntity> valid_types) {
	if (ref.isUnderscore() || ref.isStmtNum()) { return true; }

	if (ref.isSynonym()) {
		for (Declaration::DesignEntity de : valid_types) {
			if (ref.declaration.TYPE == de)
				return true;
		}
	}

	return false;
}

bool QueryParser::is_valid_entRef(Reference ref, std::vector<Declaration::DesignEntity> valid_types) {
	if (ref.isUnderscore() || ref.isString()) { return true; }

	if (ref.isSynonym()) {
		for (Declaration::DesignEntity de : valid_types) {
			if (ref.declaration.TYPE == de)
				return true;
		}
	}

	return false;
}

Declaration QueryParser::select() {
	match("Select");
	std::string target = match(parser::synonym_re);

	// checks if target is a declared synonym in the declaration list
	Declaration d = findDeclaration(target);

	return d;
}

Pattern QueryParser::patternClause() {
	match("pattern");


	// check syn-assign
	Declaration d = findDeclaration(match(parser::synonym_re));
	if (d.TYPE != Declaration::DesignEntity::Assignment) {
		throw SemanticError("syn-assign is not an Assignment synonym");
	}


	match("(");


	// check the left argument
	std::string arg = match(parser::entRef_re);
	Reference left_arg;

	try {
		left_arg = Reference(arg);
	}
	catch (SyntaxError&) {
		left_arg = Reference(findDeclaration(arg));
	}

	if (!is_valid_entRef(left_arg, parser::entRef_Var_de)) {
		throw SemanticError("Invalid synonym type used as an argument");
	}


	match(",");


	// check the right argument
	arg.clear();
	if (current_token == "_") {
		arg += match("_");
	}

	if (current_token != ")" || arg == "") {		// match 'exprssion'
		arg += match(parser::expressionSpec_re);
		
		if (current_token == "_") {					// match '_expression_'
			arg += match("_");
		}
	}

	Expression right_arg = Expression(arg);


	match(")");

	return Pattern(d, left_arg, right_arg);
}

Relation QueryParser::suchThatClause() {
	match("such");
	match("that");


	// check the relation
	Relation::Types type = Relation::getType(match(parser::relation_re));
	


	match("(");

	// check and validate arguments
	std::string arg;
	Reference left_arg, right_arg;
	if (type == Relation::Types::Calls || type == Relation::Types::CallsT) {
		arg = match(parser::entRef_re);

		try {
			left_arg = Reference(arg);
		}
		catch (SyntaxError&) {
			left_arg = Reference(findDeclaration(arg));
		}

		if (!is_valid_entRef(left_arg, parser::entRef_Proc_de)) {
			throw SemanticError("Invalid synonym type used as an argument");
		}


		match(",");


		arg = match(parser::entRef_re);

		try {
			right_arg = Reference(arg);
		}
		catch (SyntaxError&) {
			right_arg = Reference(findDeclaration(arg));
		}

		if (!is_valid_entRef(right_arg, parser::entRef_Proc_de)) {
			throw SemanticError("Invalid synonym type used as an argument");
		}
	}
	else if (type == Relation::Types::Uses || type == Relation::Types::Modifies) {
		try {
			arg = match(parser::stmtRef_re);
			if (type == Relation::Types::Uses) { type = Relation::Types::UsesS;}
			if (type == Relation::Types::Modifies) { type = Relation::Types::ModifiesS;}
		}
		catch (SyntaxError&) {		// if it's not UsesS/ModifiesS then it's UsesP/ModifiesP
			arg = match(parser::entRef_re);
			if (type == Relation::Types::Uses) { type = Relation::Types::UsesP; }
			if (type == Relation::Types::Modifies) { type = Relation::Types::ModifiesP; }
		}

		try {
			left_arg = Reference(arg);
		}
		catch (SyntaxError&) {
			left_arg = Reference(findDeclaration(arg));
		}

		if (left_arg.isUnderscore()) { throw SemanticError("First args for uses/modifies can't be _"); }

		if (type == Relation::Types::UsesS) {
			if (!is_valid_stmtRef(left_arg, parser::stmtRef_Uses_de)) {
				throw SemanticError("Invalid synonym type used as an argument");
			}
		}
		else if (type == Relation::Types::ModifiesS) {
			if (!is_valid_stmtRef(left_arg, parser::stmtRef_Modifies_de)) {
				throw SemanticError("Invalid synonym type used as an argument");
			}
		} else {	// UsesP/ModifiesP
			if (!is_valid_entRef(left_arg, parser::entRef_Proc_de)) {
				throw SemanticError("Invalid synonym type used as an argument");
			}
		}


		match(",");


		arg = match(parser::entRef_re);

		try {
			right_arg = Reference(arg);
		}
		catch (SyntaxError&) {
			right_arg = Reference(findDeclaration(arg));
		}

		if (type == Relation::Types::UsesS || type == Relation::Types::ModifiesS) {
			if (!is_valid_entRef(right_arg, parser::entRef_Var_de)) {
				throw SemanticError("Invalid synonym type used as an argument");
			}
		} else {	// UsesP/ModifiesP
			if (!is_valid_entRef(right_arg, parser::entRef_Proc_de)) {
				throw SemanticError("Invalid synonym type used as an argument");
			}
		}
	}
	else {		// Follows/Follows*/Parent/Parent*
		arg = match(parser::stmtRef_re);

		try {
			left_arg = Reference(arg);
		}
		catch (SyntaxError&) {
			left_arg = Reference(findDeclaration(arg));
		}

		if (!is_valid_stmtRef(left_arg, parser::stmtRef_de)) {
			throw SemanticError("Invalid synonym type used as an argument");
		}


		match(",");


		arg = match(parser::stmtRef_re);

		try {
			right_arg = Reference(arg);
		}
		catch (SyntaxError&) {
			right_arg = Reference(findDeclaration(arg));
		}

		if (!is_valid_stmtRef(right_arg, parser::stmtRef_de)) {
			throw SemanticError("Invalid synonym type used as an argument");
		}
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
