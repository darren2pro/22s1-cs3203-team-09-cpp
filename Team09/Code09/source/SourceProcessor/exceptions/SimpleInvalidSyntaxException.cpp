#include "SimpleInvalidSyntaxException.h"

SimpleInvalidSyntaxException::SimpleInvalidSyntaxException(const string& msg)
        : runtime_error(msg) {}
