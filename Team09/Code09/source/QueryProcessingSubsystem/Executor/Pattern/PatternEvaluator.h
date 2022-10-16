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

	std::unordered_set<LineNum> patternLeftUnderscoreRightRelaxedExpression(std::string expr);
	std::unordered_set<LineNum> patternLeftUnderscoreRightStrictExpression(std::string expr);
	std::unordered_set<LineNum> patternLeftUnderscoreRightUnderScore();

	std::unordered_set<std::pair<LineNum, Variable>, PKB::pairHash> patternLeftSynonymRightRelaxedExpression(std::string expr); // For Demo in Week 5
	std::unordered_set<std::pair<LineNum, Variable>, PKB::pairHash> patternLeftSynonymRightStrictExpression(std::string expr); // For Demo in Week 5
	std::unordered_set<std::pair<LineNum, Variable>, PKB::pairHash> patternLeftSynonymRightUnderscore();

	std::unordered_set<LineNum> patternLeftSimpleRightRelaxedExpression(Variable var, std::string expr); // For Demo in Week 5
	std::unordered_set<LineNum> patternLeftSimpleRightStrictExpression(Variable var, std::string expr); // For Demo in Week 5
	std::unordered_set<LineNum> patternLeftSimpleRightUnderscore(Variable var);


	PatternEvaluator(std::vector<Declaration> declarations, Pattern pattern, ResultsDatabase& rdb, PKBStorage* pkb) :
		declarations(declarations), pattern(pattern), rdb(rdb), pkb(pkb),
		patternArg(pattern.synonym), patternSynonym(pattern.synonym.name),
		leftArg(pattern.LEFT_ARG), rightArg(pattern.RIGHT_ARG), rightString(pattern.RIGHT_ARG.value) {};
	bool evaluate();
};
