#include "SimpleInvalidSyntaxException.h"

SimpleInvalidSyntaxException::SimpleInvalidSyntaxException(const char* message)
        : runtime_error(message) {}
