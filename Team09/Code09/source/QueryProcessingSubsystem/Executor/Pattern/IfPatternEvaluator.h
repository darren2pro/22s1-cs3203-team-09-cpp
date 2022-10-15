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

class IfPatternEvaluator: public PatternEvaluator {
public:
	IfPatternEvaluator(std::vector<Declaration> declarations,Pattern patterns, ResultsDatabase& rdb, PKBStorage* pkb) :
	PatternEvaluator(declarations, patterns, rdb, pkb) {}; // Constructor

	std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> IfPatternEvaluator::patternLeftSynonymRightStrictExpression(std::string expr) override {
		// Uses(a, 'x') List
		//std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> results = pkb->getIfLineVarByMatchFull(expr);
		std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> results = {};
		return results;
	}

	std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> IfPatternEvaluator::patternLeftSynonymRightRelaxedExpression(std::string expr) override {
		// Uses(a, v) ListPair
		//std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> results = pkb->getIfLineVarByMatchPartial(expr);
		std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> results = {};
		return results;
	}

	std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> IfPatternEvaluator::patternLeftSynonymRightUnderscore() override {
		// Uses(a, _) List
		//std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> results = pkb->getIfLineVarByUS();
		std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> results = {};
		return results;
	}

	std::unordered_set<std::string> IfPatternEvaluator::patternLeftSimpleRightRelaxedExpression(Variable var, std::string expr) override {
		// Uses(1, a) List
		//std::unordered_set<Variable> results = pkb->getIfLineByVarMatchPartial(var, expr);
		std::unordered_set<Variable> results = {};
		return results;

	}

	std::unordered_set<std::string> IfPatternEvaluator::patternLeftSimpleRightUnderscore(Variable var) override {
		// Uses(1, _) Boolean
		//std::unordered_set<std::string> results = pkb->getIfLineByVarUS(var);
		std::unordered_set<Variable> results = {};
		return results;
	}

	std::unordered_set<std::string> IfPatternEvaluator::patternLeftSimpleRightStrictExpression(Variable var, std::string expr) override {
		// Uses(1, 'x') Boolean
		//std::unordered_set<std::string> results = pkb->getIfLineByVarMatchFull(var, expr);
		std::unordered_set<Variable> results = {};
		return results;
	}

	std::unordered_set<std::string> IfPatternEvaluator::patternLeftUnderscoreRightRelaxedExpression(std::string expr) override {
		//std::unordered_set<std::string> results = pkb->getIfLineByUSMatchPartial(expr);
		std::unordered_set<Variable> results = {};
		return results;
	}

	std::unordered_set<std::string> IfPatternEvaluator::patternLeftUnderscoreRightStrictExpression(std::string expr) override {
		//std::unordered_set<std::string> results = pkb->getIfLineByUSMatchFull(expr);
		std::unordered_set<Variable> results = {};
		return results;
	}

	std::unordered_set<std::string> IfPatternEvaluator::patternLeftUnderscoreRightUnderScore() override {
		//std::unordered_set<std::string> results = pkb->getIfLineByUSUS();
		std::unordered_set<Variable> results = {};
		return results;
	}
};
