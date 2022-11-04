#pragma once
#include <vector>
#include <string>
#include "../../With.h"
#include "../../../ProgramKnowledgeBase/PKBStorage.h"
#include "../ResultsDatabase/ResultsDatabase.h"

using namespace PKB;

class WithEvaluator {
public:
	std::string leftSynonym;
	std::string rightSynonym;

	ResultsDatabase& rdb;
	PKBStorage* pkb;
	std::vector<Declaration> declarations;
	With with;
	AttrReference leftArg;
	AttrReference rightArg;


	// With has 3 possible arguments - integer, ident, attrRef -> 9 possible combination

	//// Same type -> ==
	//bool withBothInt();
	//bool withBothIdent();
	//bool withBothAttrRef();

	//// Different type
	//bool withIntAttrRef(int integer, AttrReference attrRef);
	//bool withIntIdent(int integer, std::string ident);
	//bool withAttrRefIdent(AttrReference attrRef, std::string ident);


	WithEvaluator(std::vector<Declaration> declarations, With with, ResultsDatabase& rdb, PKBStorage* pkb) :
		declarations(declarations), 
		with(with),
		rdb(rdb),
		pkb(pkb),
		leftArg(with.ref1),
		rightArg(with.ref2) {};

	bool evaluate();
};
