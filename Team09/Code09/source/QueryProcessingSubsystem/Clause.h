#pragma once
class Clause {
public:
	std::variant<Relation, Pattern> clause;

	Clause(Relation relation) : clause(relation) {};
	Clause(Pattern pattern) : clause(pattern) {};
	//Clause(With with) : clause(relation) {};
};
