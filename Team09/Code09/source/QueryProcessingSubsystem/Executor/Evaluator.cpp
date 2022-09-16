#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>
#include "Evaluator.h"
#include "../Utils.h"

std::unordered_set<std::string> Evaluator::evaluate() {
	// Check what type of argument it has, then call appropriate API

	//std::cout <<LEFT_ARG << ":" << Utils().isSynonymLEFT_ARG) << std::endl;
	//std::cout <<RIGHT_ARG << ":" << Utils().isStringRIGHT_ARG) << std::endl;
	//Utils().isStringRIGHT_ARG);

	//std::unordered_set<std::string> result = {"yyyy"};
	//return result;

	if (Utils().isSynonym(LEFT_ARG) && Utils().isSynonym(RIGHT_ARG)) {
		std::unordered_set<std::string> result = leftSynonymRightSynonymLEFT_ARG,RIGHT_ARG);
		//std::unordered_set<std::string> result = {"yyyy"};
		return result;
	}
	else if (Utils().isSynonym(LEFT_ARG) && Utils().isUnderscore(RIGHT_ARG)) {
		std::unordered_set<std::string> result = leftSynonymRightUnderscore();
		//std::unordered_set<std::string> result = {"yyyy"};
		return result;
	}
	else if (Utils().isSynonym(LEFT_ARG) && Utils().isUnderscore(RIGHT_ARG)) {
		std::unordered_set<std::string> result = leftSynonymRightUnderscore();
		//std::unordered_set<std::string> result = {"yyyy"};
		return result;
	}
	else if (Utils().isSynonym(LEFT_ARG) && Utils().isUnderscore(RIGHT_ARG)) {
		std::unordered_set<std::string> result = leftSynonymRightUnderscore();
		//std::unordered_set<std::string> result = {"yyyy"};
		return result;
	}
	else if (Utils().isSynonym(LEFT_ARG) && Utils().isUnderscore(RIGHT_ARG)) {
		std::unordered_set<std::string> result = leftSynonymRightUnderscore();
		//std::unordered_set<std::string> result = {"yyyy"};
		return result;
	}
	else if (Utils().isSynonym(LEFT_ARG) && Utils().isUnderscore(RIGHT_ARG)) {
		std::unordered_set<std::string> result = leftSynonymRightUnderscore();
		//std::unordered_set<std::string> result = {"yyyy"};
		return result;
	}
	else if (Utils().isSynonym(LEFT_ARG) && Utils().isUnderscore(RIGHT_ARG)) {
		std::unordered_set<std::string> result = leftSynonymRightUnderscore();
		//std::unordered_set<std::string> result = {"yyyy"};
		return result;
	}
	else if (Utils().isSynonym(LEFT_ARG) && Utils().isUnderscore(RIGHT_ARG)) {
		std::unordered_set<std::string> result = leftSynonymRightUnderscore();
		//std::unordered_set<std::string> result = {"yyyy"};
		return result;
	}
	else if (Utils().isSynonym(LEFT_ARG) && Utils().isUnderscore(RIGHT_ARG)) {
		std::unordered_set<std::string> result = leftSynonymRightUnderscore();
		//std::unordered_set<std::string> result = {"yyyy"};
		return result;
	}
	


	// After getting all results, save the results to database, together with variable it represents

}
