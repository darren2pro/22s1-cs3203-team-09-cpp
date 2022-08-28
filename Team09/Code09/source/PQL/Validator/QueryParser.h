#include <string>
#include <vector>
#include "../../../SPA/Query.h"

class QueryParser {
public:
	std::vector<std::string> tokens;
	QueryParser(std::vector<std::string> tokens);
	Query parse();
};