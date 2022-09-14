#include "QueryLexer.h"
#include "QueryParser.h"
#include "QueryValidator.h"
#include "QueryBuilder.h"
#include "../Query.h"
#include <string>

Query* QueryBuilder::buildQuery(std::string query_string) {

    // Tokenize the query
    QueryLexer lexer = QueryLexer(query_string);
    std::vector<std::string> tokens = lexer.lex();

    // Parse and validate
    QueryParser parser = QueryParser(tokens);
    Query* query = parser.parse();
    return query;
}
