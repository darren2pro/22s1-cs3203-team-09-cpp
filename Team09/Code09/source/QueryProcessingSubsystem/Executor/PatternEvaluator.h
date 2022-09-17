#pragma once
#include <vector>
#include <string>
#include "../../ProgramKnowledgeBase/PKBStorage.h"
#include "../Pattern.h"
#include "../Declaration.h"
#include "ResultsDatabase/ResultsDatabase.h"

typedef std::string LineNum;
typedef std::string Variable;
typedef std::string Expression;

class PatternEvaluator {
public:
	Declaration declaration;
	Pattern pattern;
	std::string SYNONYM = pattern.synonym;
	std::string LEFT_ARG = pattern.LEFT_ARG;
	std::string RIGHT_ARG = pattern.RIGHT_ARG;
	PKB::PKBStorage pkb;
	ResultsDatabase rdb;

	virtual std::unordered_set<LineNum> patternLeftUnderscoreRightRelaxedExpression(Expression expr) = 0;
	virtual std::unordered_set<LineNum> patternLeftUnderscoreRightStrictExpression(Expression expr) = 0;
	virtual std::unordered_set<LineNum> patternLeftUnderscoreRightUnderScore() = 0;

	virtual std::unordered_set<std::pair<LineNum, Variable>> patternLeftSynonymRightRelaxedExpression(Expression expr) = 0; // For Demo in Week 5
	virtual std::unordered_set<std::pair<LineNum, Variable>> patternLeftSynonymRightStrictExpression(Expression expr) = 0; // For Demo in Week 5
	virtual std::unordered_set<std::pair<LineNum, Variable>> patternLeftSynonymRightUnderscore() = 0;

	virtual std::unordered_set<LineNum> patternLeftSimpleRightRelaxedExpression(Variable var, Expression expr) = 0; // For Demo in Week 5
	virtual std::unordered_set<LineNum> patternLeftSimpleRightStrictExpression(Variable var, Expression expr) = 0; // For Demo in Week 5
	virtual std::unordered_set<LineNum> patternLeftSimpleRightUnderscore(Variable var) = 0;


	PatternEvaluator(std::vector<std::string> declarations, Pattern pattern, ResultsDatabase rdb, PKB::PKBStorage pkb) :
		declaration(declaration), pattern(pattern), rdb(rdb), pkb(pkb) {};
	bool evaluate();
};
