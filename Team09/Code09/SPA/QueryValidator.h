#pragma once
#include <string>
#include <vector>
#include "Query.h"

class QueryValidator {
public:
	Query stored_query;
	QueryValidator(Query query);
	bool validate();
};

