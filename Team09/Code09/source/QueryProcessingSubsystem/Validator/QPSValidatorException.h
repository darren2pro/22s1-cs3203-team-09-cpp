#pragma once

#include <stdexcept>

class SyntaxError : public std::runtime_error {
public:
    explicit SyntaxError(const std::string& message);
};

class SemanticError : public std::runtime_error {
public:
    explicit SemanticError(const std::string& message);
};