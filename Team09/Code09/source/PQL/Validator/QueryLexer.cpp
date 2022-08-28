#include <cctype>
#include "QueryLexer.h"
#include <string>
#include <vector>

QueryLexer::QueryLexer(std::string query) {
	query_string = query;
}

std::vector<std::string> QueryLexer::lex() {
	return {};
}

bool QueryLexer::processNonAlphabetics() {
	bool status = true;

	return status;
}

bool QueryLexer::processAlphabetics() {
	bool status = true;

	return status;
}
