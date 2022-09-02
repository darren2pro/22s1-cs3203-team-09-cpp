#pragma once

#include <stdexcept>

class QueryLexerException : public std::runtime_error {
public:
    explicit QueryLexerException(const std::string& message);
};

class QueryParserException : public std::runtime_error {
public:
    explicit QueryParserException(const std::string& message);
};

class QueryValidatorException : public std::runtime_error {
public:
    explicit QueryValidatorException(const std::string& message);
};