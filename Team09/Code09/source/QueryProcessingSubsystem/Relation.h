#pragma once
#include <string>
#include <vector>
#include "Reference.h"

class Relation {
public:
	enum Types {
		Modifies,
		ModifiesS,
		ModifiesP,
		Uses,
		UsesS,
		UsesP,
		Follows,
		FollowsT,
		Parent,
		ParentT,
		Calls,
		CallsT,
		Next,
		NextT,
		Affects,
		AffectsT,
		NONE
	};

	Types Type;	
	Reference LEFT_ARG;
	Reference RIGHT_ARG;

	Relation(Types TYPE, Reference LEFT_ARG, Reference RIGHT_ARG) : Type(TYPE), LEFT_ARG(LEFT_ARG), RIGHT_ARG(RIGHT_ARG) {};
	Relation() : Type(Relation::NONE), LEFT_ARG(Reference()), RIGHT_ARG(Reference()) {}; // Needed to ensure initialization of empty relation in Query.h


	/**
	 * Returns the Relation::Types that is equivalent to the string.
	 * @returns a Realtion::Types that is equivalent to the string.
	 */
	static Types getType(std::string str) {
		if (str == "Follows") {
			return Relation::Types::Follows;
		}
		else if (str == "Follows*") {
			return Relation::Types::FollowsT;
		}
		else if (str == "Parent") {
			return Relation::Types::Parent;
		}
		else if (str == "Parent*") {
			return Relation::Types::ParentT;
		}
		else if (str == "Uses") {
			return Relation::Types::Uses;
		}
		else if (str == "Modifies") {
			return Relation::Types::Modifies;
		}		
		else if (str == "Calls") {
			return Relation::Types::Calls;
		}		
		else if (str == "Calls*") {
			return Relation::Types::CallsT;
		}		
		else if (str == "Next") {
			return Relation::Types::Next;
		}		
		else if (str == "Next*") {
			return Relation::Types::NextT;
		}		
		else if (str == "Affects") {
			return Relation::Types::Affects;
		}		
		else if (str == "Affects*") {
			return Relation::Types::AffectsT;
		}
		else {
			return Relation::Types::NONE;
		}
	}

	bool operator==(const Relation& r) const {
		return Type == r.Type && LEFT_ARG == r.LEFT_ARG && RIGHT_ARG == r.RIGHT_ARG;
	}
};
