#pragma once

#include "../Query.h"
#include <string>

class QueryBuilder {
public:
    Query* buildQuery(std::string query_string);
};
