#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>
#include "Evaluator.h"
#include "Utils.h"

std::unordered_set<std::string> Evaluator::evaluate() {
	// Check what type of argument it has, then call appropriate API

	//std::cout << LEFT_ARG << ":" << Utils().isSynonym(LEFT_ARG) << std::endl;
	//std::cout << RIGHT_ARG << ":" << Utils().isString(RIGHT_ARG) << std::endl;
	//Utils().isString(RIGHT_ARG);

	//std::unordered_set<std::string> result = {"yyyy"};
	//return result;

	if (Utils().isSynonym(LEFT_ARG) && Utils().isSynonym(RIGHT_ARG)) {
		std::unordered_set<std::string> result = leftSynonymRightString(LEFT_ARG, RIGHT_ARG);
		return result;
		//std::unordered_set<std::string> result = {"yyyy"};
		//return result;
	}
}
