#include <string>
#include <vector>
#include "QueryParser.h"
#include "../../../SPA/Query.h"

QueryParser::QueryParser(std::vector<std::string> tokens) {
	tokens = tokens;
}

Query QueryParser::parse() {
	return Query();
}