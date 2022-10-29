#pragma once
#include <vector>
#include <string>
#include "../../../ProgramKnowledgeBase/PKBStorage.h"
#include "../../With.h"
#include "../../Declaration.h"
#include "../ResultsDatabase/ResultsDatabase.h"

typedef std::string LineNum;
typedef std::string Variable;

class WithEvaluator {
public:
	With with;
	std::vector<Declaration> declarations;
	PKBStorage* pkb;
	ResultsDatabase& rdb;

	AttrReference leftArg;
	AttrReference rightArg;
	std::string leftSynonym;
	std::string rightSynonym;

	// With has 3 possible arguments - integer, ident, attrRef -> 9 possible combination

	// Same type -> ==
	//bool withBothInt();
	//bool withBothIdent();
	//bool withBothAttrRef();

	//// Different type
	//bool withIntAttrRef(int integer, AttrReference attrRef);
	//bool withIntIdent(int integer, std::string ident);
	//bool withAttrRefIdent(AttrReference attrRef, std::string ident);


	WithEvaluator(std::vector<Declaration> declarations, With with, ResultsDatabase& rdb, PKBStorage* pkb) :
		declarations(declarations), with(with), rdb(rdb), pkb(pkb), leftArg(with.ref1), rightArg(with.ref2) {};

	bool evaluate();
};
