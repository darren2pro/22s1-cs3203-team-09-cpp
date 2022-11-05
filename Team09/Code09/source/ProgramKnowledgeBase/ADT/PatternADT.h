#pragma once

#include <unordered_map>
#include <unordered_set>
#include "../PKBUtils.h"
#include "../../SourceProcessor/SimpleInterface.h"

namespace PKB {
	class PatternADT {
	private:
		std::unordered_set<std::pair<LineNum, Variable>, pairHash> lineVarSet;
		std::unordered_map<ExprStr, std::unordered_set<std::pair<LineNum, Variable>, pairHash>> exprToLineVarMap;
		std::unordered_map<Variable, std::unordered_set<std::pair<LineNum, ExprStr>, pairHash>> varToLineExprMap;

	public:
		void add(const Variable, const LineNum, const ExprStr);
		std::unordered_set<LineNum> getLineByVar(const Variable);
		std::unordered_set<LineNum> getLineByVarMatchFull(const Variable, const ExprStr);
		std::unordered_set<LineNum> getLineByVarMatchPartial(const Variable, const ExprStr);
		std::unordered_set<std::pair<LineNum, Variable>, pairHash> getLineVarSet();
		std::unordered_set<std::pair<LineNum, Variable>, pairHash> getLineVarByMatchFull(const ExprStr);
		std::unordered_set<std::pair<LineNum, Variable>, pairHash> getLineVarByMatchPartial(const ExprStr);
		std::unordered_set<LineNum> getLineByUS();
		std::unordered_set<LineNum> getLineByUSMatchFull(const ExprStr);
		std::unordered_set<LineNum> getLineByUSMatchPartial(const ExprStr);
	};
}