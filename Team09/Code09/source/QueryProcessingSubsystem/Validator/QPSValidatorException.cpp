#include <stdexcept>
#include "QPSValidatorException.h"

SyntaxError::SyntaxError(const std::string& message) : runtime_error(message) {}

SemanticError::SemanticError(const std::string& message) : runtime_error(message) {}