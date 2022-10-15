#pragma once
#include <string>
#include <vector>
#include <variant>
#include "AttrReference.h"
#include "Declaration.h"

class Result {
public:
	enum Types {
		Tuple,
		Boolean,
		NONE
	};

	Types TYPE;
	std::vector<std::variant<Declaration, AttrReference>> target;

	Result() : TYPE(Types::NONE), target(std::vector<std::variant<Declaration, AttrReference>>()) {}
	Result(Types TYPE) : TYPE(TYPE), target(std::vector<std::variant<Declaration, AttrReference>>()) {}
	Result(Types TYPE, std::vector<std::variant<Declaration, AttrReference>> target) : TYPE(TYPE), target(target) {}

	bool operator==(const Result& r) const {
		return TYPE == r.TYPE && target == r.target;
	}

	bool isBoolean() { return TYPE == Types::Boolean; }
	bool isTuple() { return TYPE == Types::Tuple; }
};