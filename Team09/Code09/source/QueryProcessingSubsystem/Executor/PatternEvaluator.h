#pragma once
#include <vector>
#include <string>
#include "../../ProgramKnowledgeBase/PKBStorage.h"
#include "../Pattern.h"
#include "../Declaration.h"

class PatternEvaluator {
private:
	Declaration declaration;
	Pattern pattern;
	PKB::PKBStorage pkb;

	virtual std::unordered_set<std::string> patternleftUnderscoreRightRelaxedExpression(std::string RIGHT_ARG) = 0;
	virtual std::unordered_set<std::string> patternleftUnderscoreRightStrictExpression(std::string RIGHT_ARG) = 0;
	virtual std::unordered_set<std::string> patternleftUnderscoreRightUnderScore() = 0;

	virtual std::unordered_set<std::string> patternleftSynonymRightRelaxedExpression(std::string LEFT_ARG, std::string RIGHT_ARG) = 0; // For Demo in Week 5
	virtual std::unordered_set<std::string> patternleftSynonymRightStrictExpression(std::string LEFT_ARG, std::string RIGHT_ARG) = 0; // For Demo in Week 5
	virtual std::unordered_set<std::string> patternleftSynonymRightUnderscore(std::string LEFT_ARG) = 0;

	virtual std::unordered_set<std::string> patternleftSimpleRightRelaxedExpression(std::string LEFT_ARG, std::string RIGHT_ARG) = 0; // For Demo in Week 5
	virtual std::unordered_set<std::string> patternleftSimpleRightStrictExpression(std::string LEFT_ARG, std::string RIGHT_ARG) = 0; // For Demo in Week 5
	virtual std::unordered_set<std::string> patternleftSimpleRightUnderscore(std::string LEFT_ARG) = 0;


public:
	PatternEvaluator(Declaration declaration, Pattern pattern, PKB::PKBStorage pkb) :
		declaration(declaration), pattern(pattern), pkb(pkb) {};
	std::unordered_set<std::string> evaluate();
};
