#pragma once
#include "../ResultsDatabase/ResultsDatabase.h"
#include "../../Clause.h"

class Strategy {
public:
	std::vector<Declaration> declarations;
	PKBStorage* pkb;

	explicit Strategy(std::vector<Declaration> declarations, PKBStorage* pkb) : declarations(declarations), pkb(pkb) {};
	virtual bool execute(Clause& clause, ResultsDatabase& rdb) const = 0;
	virtual ~Strategy() = default;
};
