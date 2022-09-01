#pragma once

#include <stdexcept>

class SimpleInvalidSyntaxException : public std::runtime_error {
public:
    explicit SimpleInvalidSyntaxException(const char* message);
};
