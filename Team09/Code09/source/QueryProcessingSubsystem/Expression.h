#pragma once
#include <string>
#include <vector>

class Expression {
public:
	enum Types {
		Underscore,		// '_'
		Strict,			// 'x + 1'
		Relaxed,		// '_"x + 1"_'
		NONE
	};

	Types TYPE;
	std::string value;

	bool isUnderscore();
	bool isStrict();
	bool isRelaxed();
};
