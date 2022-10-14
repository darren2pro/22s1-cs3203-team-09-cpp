#pragma once
#include <string>
#include <vector>
#include "PatternEvaluator.h"
#include "../../../ProgramKnowledgeBase/PKBStorage.h"
#include <iostream>
#include <cassert>

typedef std::string LineNum;
typedef std::string Variable;

using namespace std;

class WhilePatternEvaluator: public PatternEvaluator {
public:
	WhilePatternEvaluator(std::vector<Declaration> declarations,Pattern patterns, ResultsDatabase& rdb, PKBStorage* pkb) :
	PatternEvaluator(declarations, patterns, rdb, pkb) {}; // Constructor

	std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> WhilePatternEvaluator::patternLeftSynonymRightStrictExpression(std::string expr) override {
		// Uses(a, 'x') List
		//std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> results = pkb->getWhileLineVarByMatchFull(expr);
		std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> results = {};
		return results;
	}

	std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> WhilePatternEvaluator::patternLeftSynonymRightRelaxedExpression(std::string expr) override {
		// Uses(a, v) ListPair
		//std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> results = pkb->getWhileLineVarByMatchPartial(expr);
		std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> results = {};
		return results;
	}

	std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> WhilePatternEvaluator::patternLeftSynonymRightUnderscore() override {
		// Uses(a, _) List
		//std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> results = pkb->getWhileLineVarByUS();
		std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> results = {};
		return results;
	}

	std::unordered_set<std::string> WhilePatternEvaluator::patternLeftSimpleRightRelaxedExpression(Variable var, std::string expr) override {
		// Uses(1, a) List
		//std::unordered_set<Variable> results = pkb->getWhileLineByVarMatchPartial(var, expr);
		std::unordered_set<Variable> results = {};
		return results;

	}

	std::unordered_set<std::string> WhilePatternEvaluator::patternLeftSimpleRightUnderscore(Variable var) override {
		// Uses(1, _) Boolean
		//std::unordered_set<std::string> results = pkb->getWhileLineByVarUS(var);
		std::unordered_set<Variable> results = {};
		return results;
	}

	std::unordered_set<std::string> WhilePatternEvaluator::patternLeftSimpleRightStrictExpression(Variable var, std::string expr) override {
		// Uses(1, 'x') Boolean
		//std::unordered_set<std::string> results = pkb->getWhileLineByVarMatchFull(var, expr);
		std::unordered_set<Variable> results = {};
		return results;
	}

	std::unordered_set<std::string> WhilePatternEvaluator::patternLeftUnderscoreRightRelaxedExpression(std::string expr) override {
		//std::unordered_set<std::string> results = pkb->getWhileLineByUSMatchPartial(expr);
		std::unordered_set<Variable> results = {};
		return results;
	}

	std::unordered_set<std::string> WhilePatternEvaluator::patternLeftUnderscoreRightStrictExpression(std::string expr) override {
		//std::unordered_set<std::string> results = pkb->getWhileLineByUSMatchFull(expr);
		std::unordered_set<Variable> results = {};
		return results;
	}

	std::unordered_set<std::string> WhilePatternEvaluator::patternLeftUnderscoreRightUnderScore() override {
		//std::unordered_set<std::string> results = pkb->getWhileLineByUSUS();
		std::unordered_set<Variable> results = {};
		return results;
	}
};
