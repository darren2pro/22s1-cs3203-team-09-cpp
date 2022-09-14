#include <string>
#include <vector>
#include "PatternEvaluator.h"
#include "../Utils.h"

std::unordered_set<std::string> PatternEvaluator::evaluate() {

	// left underscore
	if (Utils().isUnderscore(pattern.LEFT_ARG) && Utils().isUnderscore(pattern.RIGHT_ARG)) {
		std::unordered_set<std::string> results = patternleftUnderscoreRightUnderScore();
		return results;
	}
	else if (Utils().isUnderscore(pattern.LEFT_ARG) && Utils().isStrictExpression(pattern.TYPE)) {
		std::unordered_set<std::string> results = patternleftUnderscoreRightStrictExpression(pattern.RIGHT_ARG);
		return results;
	}
	else if (Utils().isUnderscore(pattern.LEFT_ARG) && Utils().isRelaxedExpression(pattern.TYPE)) {
		std::unordered_set<std::string> results = patternleftUnderscoreRightRelaxedExpression(pattern.RIGHT_ARG);
		return results;
	}

	// left synonym
	else if(Utils().isSynonym(pattern.LEFT_ARG) && Utils().isUnderscore(pattern.RIGHT_ARG)) {
		std::unordered_set<std::string> results = patternleftSynonymRightUnderscore(pattern.LEFT_ARG);
		return results;
	}
	else if (Utils().isSynonym(pattern.LEFT_ARG) && Utils().isStrictExpression(pattern.TYPE)) {
		std::unordered_set<std::string> results = patternleftSynonymRightStrictExpression(pattern.LEFT_ARG, pattern.RIGHT_ARG);
		return results;
	}
	else if (Utils().isSynonym(pattern.LEFT_ARG) && Utils().isRelaxedExpression(pattern.TYPE)) {
		std::unordered_set<std::string> results = patternleftSynonymRightRelaxedExpression(pattern.LEFT_ARG, pattern.RIGHT_ARG);
		return results;
	}

	// left simple
	else if (Utils().isString(pattern.LEFT_ARG) && Utils().isUnderscore(pattern.RIGHT_ARG)) {
		std::unordered_set<std::string> results = patternleftSimpleRightUnderscore(pattern.RIGHT_ARG);
		return results;
	}
	else if (Utils().isString(pattern.LEFT_ARG) && Utils().isStrictExpression(pattern.TYPE)) {
		std::unordered_set<std::string> results = patternleftSimpleRightStrictExpression(pattern.LEFT_ARG, pattern.RIGHT_ARG);
		return results;
	}
	else if (Utils().isString(pattern.LEFT_ARG) && Utils().isRelaxedExpression(pattern.TYPE)) {
		std::unordered_set<std::string> results = patternleftSimpleRightRelaxedExpression(pattern.LEFT_ARG, pattern.RIGHT_ARG);
		return results;
	}


}
