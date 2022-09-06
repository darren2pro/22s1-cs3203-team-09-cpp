#include "QueryLexer.h"
#include "QueryParser.h"
#include "QueryValidator.h"
#include "QueryBuilder.h"
#include "../Query.h"
#include <string>

Query* QueryBuilder::buildQuery(std::string query_string) {

    // Lexer
    QueryLexer lexer = QueryLexer(query_string);
    std::vector<std::string> tokens = lexer.lex();

    // Parser
    QueryParser parser = QueryParser(tokens);
    Query* query = parser.parse();
    return query;

    // Validator
    /*
    QueryValidator validator = QueryValidator(query);

    if (validator.validate()) {
        return query;
    }
    else {
        // Query is invalid. Throw error? Return null?
        return query;
    }
     */
}
