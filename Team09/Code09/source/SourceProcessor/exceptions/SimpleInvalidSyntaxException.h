#pragma once

#include <stdexcept>

using namespace std;

class SimpleInvalidSyntaxException : public std::runtime_error {
public:
    explicit SimpleInvalidSyntaxException(const string& msg);
};
