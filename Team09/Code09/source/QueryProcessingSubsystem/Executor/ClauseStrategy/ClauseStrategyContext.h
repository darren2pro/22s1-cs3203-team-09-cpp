#pragma once
#include <string>
#include "../ResultsDatabase/ResultsDatabase.h"
#include "Strategy.h"

class ClauseStrategyContext {
public:
	std::unique_ptr<Strategy> strategy_;
	explicit ClauseStrategyContext(std::unique_ptr<Strategy>&& strategy = {}) : strategy_(std::move(strategy)) {};
	void setStrategy(std::unique_ptr<Strategy>&& strategy);
	bool execute(Clause clause, ResultsDatabase& rdb) const;
	bool execute(Pattern pattern, ResultsDatabase& rdb) const;
	bool execute(Relation relation, ResultsDatabase& rdb) const;
};