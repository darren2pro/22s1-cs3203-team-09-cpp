#pragma once
#include <string>
#include <vector>

class Relation {
public:
	enum Types {		// Relations
		Modifies,
		Uses,
		Follows,
		FollowsT,
		Parent,
		ParentT,
		NONE
	};

	Types TYPE;
	std::string LEFT_ARG;		// Reference type
	std::string RIGHT_ARG;		// Reference type

	Relation(Types TYPE, std::string LEFT_ARG, std::string RIGHT_ARG) : TYPE(TYPE), LEFT_ARG(LEFT_ARG), RIGHT_ARG(RIGHT_ARG) {};
	Relation() : TYPE(Relation::NONE), LEFT_ARG(""), RIGHT_ARG("") {}; // Needed to ensure initialization of empty relation in Query.h
};
