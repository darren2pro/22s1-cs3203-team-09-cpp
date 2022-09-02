#include <stdexcept>
#include "QPSValidatorException.h"

class QueryLexerException : public std::runtime_error {
public:
	QueryLexerException(const std::string& message) : runtime_error(message) {}
};

class QueryParserException : public std::runtime_error {
public:
	QueryParserException(const std::string& message) : runtime_error(message) {}
};

class QueryValidatorException : public std::runtime_error {
public:
	QueryValidatorException(const std::string& message) : runtime_error(message) {}
};