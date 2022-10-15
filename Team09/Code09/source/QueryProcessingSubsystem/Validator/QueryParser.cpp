#include <string>
#include <vector>
#include <regex>
#include <variant>
#include "QueryParser.h"
#include "../Relation.h"
#include "../Pattern.h"
#include "../Declaration.h"
#include "../Expression.h"
#include "../Reference.h"
#include "SyntaxException.h"
#include "SemanticException.h"
#include "../With.h"
#include "../AttrReference.h"

namespace parser {
	std::string synonym = "[a-zA-Z]([a-zA-Z0-9])*";
	std::string integer = "(0|[1-9]([0-9])*)";
	std::string stmtRef = synonym + "|_|" + integer;
	std::string entRef = synonym + "|_|\"" + synonym + "\"";
	std::string expressionSpec = "_|\"(.*)\"";						// The expression will be validated using the AST parser
	std::string attrRef = synonym + "|\"" + synonym + "\"|" + integer;

	std::regex integer_re(integer);
	std::regex design_enteties_re("stmt|read|print|while|if|assign|variable|constant|procedure|call");
	std::regex relation_re("Follows|Follows[\*]|Parent|Parent[\*]|Uses|Modifies|Calls|Calls[\*]|Next|Next[\*]|Affects|Affects[\*]");
	std::regex synonym_re(synonym);									// synonym: IDENT	--> IDENT: LETTER (LETTER|DIGIT)*
	std::regex stmtRef_re(stmtRef);									// stmtRef: synonym | '_' | INTEGER
	std::regex entRef_re(entRef);									// endRef: synonym | '_' | '"' IDENT '"'
	std::regex expressionSpec_re(expressionSpec);					// _ | '"' (*) '"' 
	std::regex attr_re("procName|varName|value|stmt#");
	std::regex attrRef_re(attrRef);									// synonym | " IDENT " | INTEGER

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
	// valid stmtRef synonym types for Affects
	std::vector<Declaration::DesignEntity> stmtRef_Affects_de = std::vector<Declaration::DesignEntity>({ Declaration::DesignEntity::Assignment });
	// valid (first arg) entRef synonym types for UsesP/ModifiesP/Calls
	std::vector<Declaration::DesignEntity> entRef_Proc_de = std::vector<Declaration::DesignEntity>({ Declaration::DesignEntity::Procedure });
	// valid (second arg) entRef synonym types for UsesP/ModifiesP/Calls & pattern
	std::vector<Declaration::DesignEntity> entRef_Var_de = std::vector<Declaration::DesignEntity>({ Declaration::DesignEntity::Variable });
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

std::vector<Declaration> QueryParser::parseDeclaration() {
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

Reference QueryParser::getReference(std::string arg) {
	try {
		return Reference(arg);
	}
	catch (SyntaxError&) {
		return Reference(findDeclaration(arg));
	}
}

bool QueryParser::is_valid_stmtRef(Reference ref, std::vector<Declaration::DesignEntity> valid_types) {
	if (ref.isUnderscore() || ref.isStmtNum()) { return true; }

	if (ref.isSynonym()) {
		if (std::find(valid_types.begin(), valid_types.end(), ref.declaration.Type) != valid_types.end()) {
			return true;
		}
	}

	return false;
}

bool QueryParser::is_valid_entRef(Reference ref, std::vector<Declaration::DesignEntity> valid_types) {
	if (ref.isUnderscore() || ref.isString()) { return true; }

	if (ref.isSynonym()) {
		if (std::find(valid_types.begin(), valid_types.end(), ref.declaration.Type) != valid_types.end()) {
			return true;
		}
	}

	return false;
}

Reference QueryParser::parseEntRef(std::vector<Declaration::DesignEntity> de) {
	Reference ref = getReference(match(parser::entRef_re));
	if (!is_valid_entRef(ref, de)) {
		throw SemanticError("Invalid synonym type used as an argument");
	}
	return ref;
}

Reference QueryParser::parseStmtRef(std::vector<Declaration::DesignEntity> de) {
	Reference ref = getReference(match(parser::stmtRef_re));
	if (!is_valid_stmtRef(ref, de)) {
		throw SemanticError("Invalid synonym type used as an argument");
	}
	return ref;
}

std::variant<Declaration, AttrReference> QueryParser::parseSelect() {
	match("Select");
	std::string strelem = match(parser::synonym_re);

	// checks if result is a declared synonym in the declaration list
	Declaration d = findDeclaration(strelem);
	
	// check if its an attribute reference
	if (current_token == ".") {
		match(".");
		AttrReference::Attribute attr = AttrReference::getAttribute(match(parser::attr_re));
		return AttrReference(d, attr);
	}
	else {
		return d;
	}

}

AttrReference QueryParser::parseAttrRef() {
	std::string arg = match(parser::attrRef_re);
	
	if (std::regex_match(arg, parser::synonym_re)) {	// synonym.attribute
		Declaration d = findDeclaration(arg);
		match(".");
		AttrReference::Attribute attr = AttrReference::getAttribute(match(parser::attr_re));
		return AttrReference(d, attr);
	}
	else {		// " IDENT " | INTEGER
		return AttrReference(arg);
	}
}

With QueryParser::withClause() {

	if (current_token == "with") {		// to check for the case of ' with ref1= ref2 and with ref3 = ref4 ' 
		try {
			findDeclaration(current_token);
		}
		catch (SemanticError&) {
			throw SyntaxError("invalid syntax");
		}
	}

	AttrReference ref1 = parseAttrRef();
	
	match("=");

	AttrReference ref2 = parseAttrRef();
	
	// Check if ref1 and ref2 are the same type (both name or both integer)
	if (ref1.valueType != ref2.valueType) {
		throw SemanticError("attribute reference types are not the same");
	}

	return With(ref1, ref2);
}

void QueryParser::parseWith() {
	match("with");
	withCl.push_back(withClause());

	while (current_token == "and") {
		match("and");
		withCl.push_back(withClause());
	}
}

Pattern QueryParser::patternClause() {
	// check synonym
	Declaration d = findDeclaration(match(parser::synonym_re));

	match("(");


	// check the left argument
	std::string arg = match(parser::entRef_re);
	Reference left_arg = getReference(arg);
	if (!is_valid_entRef(left_arg, parser::entRef_Var_de)) {
		throw SemanticError("Invalid synonym type used as an argument");
	}


	match(",");

	if (d.Type == Declaration::DesignEntity::Assignment) {
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

		return Pattern(Pattern::Types::Assign, d, left_arg, right_arg);

	}
	else if (d.Type == Declaration::DesignEntity::While) {
		match("_");
		match(")");
		return Pattern(Pattern::Types::While, d, left_arg);
	}
	else if (d.Type == Declaration::DesignEntity::If) {
		match("_");
		match(",");
		match("_");
		match(")");
		return Pattern(Pattern::Types::If, d, left_arg);
	}
	else {
		throw SemanticError("Invalid synonym type for pattern");
	}

}

void QueryParser::parsePattern() {
	match("pattern");
	patternCl.push_back(patternClause());

	while (current_token == "and") {
		match("and");
		patternCl.push_back(patternClause());
	}
}

Relation::Types QueryParser::getUsesModifiesType(Relation::Types type) {
	try {		// if it's a synonym then check by synonym type
		Declaration d = findDeclaration(current_token);
		if (d.Type == Declaration::DesignEntity::Procedure) {
			if (type == Relation::Types::Uses) { return Relation::Types::UsesP; }
			if (type == Relation::Types::Modifies) { return Relation::Types::ModifiesP; }
		}
		else {
			if (type == Relation::Types::Uses) { return Relation::Types::UsesS; }
			if (type == Relation::Types::Modifies) { return Relation::Types::ModifiesS; }
		}
	}
	catch (SemanticError&) {		// if not it's either stmtRed or entRef
		if (std::regex_match(current_token, parser::stmtRef_re)) {
			if (type == Relation::Types::Uses) { return Relation::Types::UsesS; }
			if (type == Relation::Types::Modifies) { return Relation::Types::ModifiesS; }
		}
		else {
			if (type == Relation::Types::Uses) { return Relation::Types::UsesP; }
			if (type == Relation::Types::Modifies) { return Relation::Types::ModifiesP; }
		}
	}
}

Relation QueryParser::suchThatClause() {
	// check the relation
	Relation::Types type = Relation::getType(match(parser::relation_re));	

	match("(");

	// check and validate arguments
	Reference left_arg, right_arg;
	if (type == Relation::Types::Uses || type == Relation::Types::Modifies) {
		// determine if it's UsesS/ModifiesS or UsesP/ModifiesP
		type = getUsesModifiesType(type);

		if (type == Relation::Types::UsesS) {
			left_arg = parseStmtRef(parser::stmtRef_Uses_de);
		}
		else if (type == Relation::Types::ModifiesS) {
			left_arg = parseStmtRef(parser::stmtRef_Modifies_de);
		}
		else {	// UsesP/ModifiesP
			left_arg = parseEntRef(parser::entRef_Proc_de);
		}

		if (left_arg.isUnderscore()) { throw SemanticError("First args for uses/modifies can't be _"); }

		match(",");

		right_arg = parseEntRef(parser::entRef_Var_de);
	}
	else if (type == Relation::Types::Calls || type == Relation::Types::CallsT) {
		left_arg = parseEntRef(parser::entRef_Proc_de);

		match(",");

		right_arg = parseEntRef(parser::entRef_Proc_de);
	}
	else {		// Follows/Follows*/Parent/Parent*/Next/Next*/Affects/Affects*
		std::vector<Declaration::DesignEntity> de = parser::stmtRef_de;
		if (type == Relation::Types::Affects || type == Relation::Types::AffectsT) {
			de = parser::stmtRef_Affects_de;
		}

		left_arg = parseStmtRef(de);

		match(",");

		right_arg = parseStmtRef(de);
	}

	match(")");

	return Relation(type, left_arg, right_arg);
}

void QueryParser::parseSuchThat() {
	match("such");
	match("that");

	suchThatCl.push_back(suchThatClause());

	while (current_token == "and") {
		match("and");
		suchThatCl.push_back(suchThatClause());
	}
}

Query* QueryParser::parse() {
	Query* query = new Query();

	declarations = parseDeclaration();	// parse declarations
	target = parseSelect();				// parse Select statement

	// parse such that, pattern, and with clause
	while (index < query_tokens.size()) {
		if (current_token == "such") {
			parseSuchThat();
		}
		else if (current_token == "pattern") {
			parsePattern();
		}
		else if (current_token == "with") {
			parseWith();
		} else {
			throw SyntaxError("Unexpected token");
		}
	}

	query->declarations = declarations;
	query->relations = suchThatCl;
	query->patterns = patternCl;
	query->withs = withCl;
	query->target = target;

	return query;
}
