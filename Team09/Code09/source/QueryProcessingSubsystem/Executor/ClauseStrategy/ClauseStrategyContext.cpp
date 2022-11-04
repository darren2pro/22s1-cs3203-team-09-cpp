#include "ClauseStrategyContext.h"
#include "Strategy.h"
#include "../../Clause.h"

void ClauseStrategyContext::setStrategy(std::shared_ptr<Strategy> strategy) {
		strategy_ = strategy;
	}

bool ClauseStrategyContext::execute(Clause clause, ResultsDatabase& rdb) const {
	if (strategy_) {
		return strategy_->execute(clause, rdb);
	}
	else {
		return false;
	}
}

bool ClauseStrategyContext::execute(Pattern pattern, ResultsDatabase& rdb) const
{
	return execute(Clause(pattern), rdb);
}

bool ClauseStrategyContext::execute(Relation relation, ResultsDatabase& rdb) const
{
	return execute(Clause(relation), rdb);
}

bool ClauseStrategyContext::execute(With with, ResultsDatabase& rdb) const
{
	return execute(Clause(with), rdb);
}
