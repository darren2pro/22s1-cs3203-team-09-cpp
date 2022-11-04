#pragma once
#include <string>
#include "../ResultsDatabase/ResultsDatabase.h"
#include "Strategy.h"

class ClauseStrategyContext {
public:
	std::shared_ptr<Strategy> strategy_;
	explicit ClauseStrategyContext(std::shared_ptr<Strategy> strategy = {}) : strategy_(strategy) {};
	void setStrategy(std::shared_ptr<Strategy> strategy);
	bool execute(Clause clause, ResultsDatabase& rdb) const;
	bool execute(Pattern pattern, ResultsDatabase& rdb) const;
	bool execute(Relation relation, ResultsDatabase& rdb) const;
	bool execute(With with, ResultsDatabase& rdb) const;
};