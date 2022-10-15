#pragma once
#include <vector>
#include <string>
#include "../../../ProgramKnowledgeBase/PKBStorage.h"
#include "../../Pattern.h"
#include "../../Declaration.h"
#include "../ResultsDatabase/ResultsDatabase.h"

typedef std::string LineNum;
typedef std::string Variable;

class PatternEvaluator {
public:
	Pattern pattern;
	Declaration patternArg;
	std::string patternSynonym;

	Reference leftArg;
	std::string leftSynonym;

	Expression rightArg;
	std::string rightString;

	std::vector<Declaration> declarations;
	PKBStorage* pkb;
	ResultsDatabase& rdb;

	std::string temporaryStrip(std::string arg);

	virtual std::unordered_set<LineNum> patternLeftUnderscoreRightRelaxedExpression(std::string expr) = 0;
	virtual std::unordered_set<LineNum> patternLeftUnderscoreRightStrictExpression(std::string expr) = 0;
	virtual std::unordered_set<LineNum> patternLeftUnderscoreRightUnderScore() = 0;

	virtual std::unordered_set<std::pair<LineNum, Variable>, PKB::pairHash> patternLeftSynonymRightRelaxedExpression(std::string expr) = 0; // For Demo in Week 5
	virtual std::unordered_set<std::pair<LineNum, Variable>, PKB::pairHash> patternLeftSynonymRightStrictExpression(std::string expr) = 0; // For Demo in Week 5
	virtual std::unordered_set<std::pair<LineNum, Variable>, PKB::pairHash> patternLeftSynonymRightUnderscore() = 0;

	virtual std::unordered_set<LineNum> patternLeftSimpleRightRelaxedExpression(Variable var, std::string expr) = 0; // For Demo in Week 5
	virtual std::unordered_set<LineNum> patternLeftSimpleRightStrictExpression(Variable var, std::string expr) = 0; // For Demo in Week 5
	virtual std::unordered_set<LineNum> patternLeftSimpleRightUnderscore(Variable var) = 0;


	PatternEvaluator(std::vector<Declaration> declarations, Pattern pattern, ResultsDatabase& rdb, PKBStorage* pkb) :
		declarations(declarations), pattern(pattern), rdb(rdb), pkb(pkb),
		patternArg(pattern.synonym), patternSynonym(pattern.synonym.name),
		leftArg(pattern.LEFT_ARG), rightArg(pattern.RIGHT_ARG), rightString(pattern.RIGHT_ARG.value) {};
	bool evaluate();
};
