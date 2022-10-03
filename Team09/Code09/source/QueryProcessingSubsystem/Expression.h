#pragma once
#include <string>
#include <vector>
#include <regex>
#include "Validator/SyntaxException.h"

class Expression {
public:
	enum Types {
		Underscore,		// '_'
		Strict,			// '"x + 1"'
		Relaxed,		// '_"x + 1"_'
		NONE
	};

	Types TYPE;
	std::string value;

	Expression(std::string value) {
		if (value == "_") {
			this->TYPE = Types::Underscore;
		}
		else if (std::regex_match(value, std::regex("_\"(.*)\"_"))) {
			value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
			value.erase(std::remove(value.begin(), value.end(), '_'), value.end());
			this->TYPE = Types::Relaxed;
		} else if (std::regex_match(value, std::regex("\"(.*)\""))) {
			value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
			this->TYPE = Types::Strict;
		}
		else {
			throw SyntaxError("invalid expression value");
		}
		this->value = value;
	}

	Expression() : TYPE(Types::NONE), value("") {};

	bool operator==(const Expression& e) const {
		return TYPE == e.TYPE && value == e.value;
	}

	bool isUnderscore() { return TYPE == Types::Underscore; }
	bool isStrict() { return TYPE == Types::Strict; }
	bool isRelaxed() { return TYPE == Types::Relaxed; }
};
