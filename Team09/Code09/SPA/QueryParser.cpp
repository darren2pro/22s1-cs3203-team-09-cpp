#include <string>
#include <vector>
#include "QueryParser.h"

QueryParser::QueryParser(std::vector<std::string> tokens) {
	tokens = tokens;
}

Query QueryParser::parse() {
	return Query(); // Return empty query first
}