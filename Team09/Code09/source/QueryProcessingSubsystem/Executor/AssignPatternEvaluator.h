#pragma once
#include <string>
#include <vector>
#include "PatternEvaluator.h"
#include "../../ProgramKnowledgeBase/PKBManager.h"
#include <iostream>
#include <cassert>

typedef std::string LineNum;
typedef std::string Variable;

using namespace std;

class AssignPatternEvaluator: public PatternEvaluator {
public:
	AssignPatternEvaluator(std::vector<Declaration> declarations,Pattern patterns, ResultsDatabase rdb, PKBManager pkb) :
	PatternEvaluator(declarations, patterns, rdb, pkb) {}; // Constructor

	std::unordered_set<std::pair<std::string, std::string>, PairHasher::pairHash> AssignPatternEvaluator::patternLeftSynonymRightStrictExpression(Expression expr) override {
		// Uses(a, 'x') List
		std::unordered_set<std::pair<std::string, std::string>, PairHasher::pairHash> results = pkb.getAssignLineVarByMatchFull(expr);
		return results;
	}

	std::unordered_set<std::pair<std::string, std::string>, PairHasher::pairHash> AssignPatternEvaluator::patternLeftSynonymRightRelaxedExpression(Expression expr) override {
		// Uses(a, v) ListPair
		std::unordered_set<std::pair<std::string, std::string>, PairHasher::pairHash> results = pkb.getAssignLineVarByMatchPartial(expr);
		return results;
	}

	std::unordered_set<std::pair<std::string, std::string>, PairHasher::pairHash> AssignPatternEvaluator::patternLeftSynonymRightUnderscore() override {
		// Uses(a, _) List
		std::unordered_set<std::pair<std::string, std::string>, PairHasher::pairHash> results = pkb.getAssignLineVarByUS();
		return results;
	}

	std::unordered_set<std::string> AssignPatternEvaluator::patternLeftSimpleRightRelaxedExpression(Variable var, Expression expr) override {
		// Uses(1, a) List
		std::unordered_set<Variable> results = pkb.getAssignLineByVarMatchPartial(var, expr);
		return results;

	}

	std::unordered_set<std::string> AssignPatternEvaluator::patternLeftSimpleRightUnderscore(Variable var) override {
		// Uses(1, _) Boolean
		std::unordered_set<std::string> results = pkb.getAssignLineByVarUS(var);
		return results;

	}

	std::unordered_set<std::string> AssignPatternEvaluator::patternLeftSimpleRightStrictExpression(Variable var, Expression expr) override {
		// Uses(1, 'x') Boolean
		std::unordered_set<std::string> results = pkb.getAssignLineByVarMatchFull(var, expr);
		return results;
	}

	std::unordered_set<std::string> AssignPatternEvaluator::patternLeftUnderscoreRightRelaxedExpression(Expression expr) override {
		std::unordered_set<std::string> results = pkb.getAssignLineByUSVarMatchPartial(expr);
		return results;
	}

	std::unordered_set<std::string> AssignPatternEvaluator::patternLeftUnderscoreRightStrictExpression(Expression expr) override {
		std::unordered_set<std::string> results = pkb.getAssignLineByUSMatchFull(expr);
		return results;
	}

	std::unordered_set<std::string> AssignPatternEvaluator::patternLeftUnderscoreRightUnderScore() override {
		std::unordered_set<std::string> results = pkb.getAssignLineByUSUS();
		return results;
	}
};
