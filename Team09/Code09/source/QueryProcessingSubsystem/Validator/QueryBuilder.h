#pragma once

#include "../Query.h"
#include <string>

/**
 * A QueryBuilder class.
 */
class QueryBuilder {
public:
	/**
	 * Builds the query from the query string by tokenizing, parsing, and validating the query.
	 * @return A valid query.
	 * @throws SyntaxError or SemanticError.
	 */
    Query* buildQuery(std::string query_string);
};
