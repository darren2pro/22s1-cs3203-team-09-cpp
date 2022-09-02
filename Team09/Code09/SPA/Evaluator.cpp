#include <string>
#include <vector>
#include "Evaluator.h"
#include "Utils.h"

std::vector<std::string> Evaluator::evaluate() {
	// Check what type of argument it has, then call appropriate API

	if (Utils::isSynonym(LEFT_ARG) && Utils::isString(RIGHT_ARG)) {
		std::vector<std::string> result = leftSynonymRightString(LEFT_ARG, RIGHT_ARG);
		return result;
	}
}
