#include <stdexcept>
#include "QPSValidatorException.h"

QueryLexerException::QueryLexerException(const std::string& message) : runtime_error(message) {}

QueryParserException::QueryParserException(const std::string& message) : runtime_error(message) {}

QueryValidatorException::QueryValidatorException(const std::string& message) : runtime_error(message) {}