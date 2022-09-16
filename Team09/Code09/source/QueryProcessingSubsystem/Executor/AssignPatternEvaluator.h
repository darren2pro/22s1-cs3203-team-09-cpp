#pragma once
#include <string>
#include <vector>
#include "PatternEvaluator.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"
#include <iostream>
#include <cassert>

typedef std::string LineNum;
typedef std::string Variable;

using namespace std;

class AssignPatternEvaluator: public PatternEvaluator {
public:
	AssignPatternEvaluator(std::vector<std::string> declarations,Pattern patterns, PKB::PKBStorage pkb) :
	PatternEvaluator(declarations, patterns, pkb) {}; // Constructor

	std::unordered_set<std::pair<std::string, std::string>> AssignPatternEvaluator::patternLeftSynonymRightStrictExpression(Expression expr) override {
		// Uses(a, 'x') List
		std::unordered_set<std::pair<std::string, std::string>> results = pkb.getAssignLineVarByMatchFull(expr);
		return results;
	}

	std::unordered_set<std::pair<std::string, std::string>> AssignPatternEvaluator::patternLeftSynonymRightRelaxedExpression(Expression expr) override {
		// Uses(a, v) ListPair
		std::unordered_set<std::pair<std::string, std::string>> results = pkb.getAssignLineVarByMatchPartial(expr);
		return results;
	}

	std::unordered_set<std::pair<std::string, std::string>> AssignPatternEvaluator::patternLeftSynonymRightUnderscore() override {
		// Uses(a, _) List
		std::unordered_set<std::pair<std::string, std::string>> results = pkb.getAssignLineVarByUS();
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

	std::unordered_set<std::string> AssignPatternEvaluator::patternLeftSimpleRightStrictExpression(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Uses(1, 'x') Boolean
		std::unordered_set<std::string> results = pkb.getAssignLineByVarMatchFull(var, expr);
		return results;
	}

	std::unordered_set<std::string> AssignPatternEvaluator::patternLeftUnderscoreRightRelaxedExpression(Expression expr) override {
		std::unordered_set<std::string> results = pkb.getAssignLineByUSVarMatchPartial(expr);
		assert(false);
	}

	std::unordered_set<std::string> AssignPatternEvaluator::patternLeftUnderscoreRightStrictExpression(Expression expr) override {
		std::unordered_set<std::string> results = pkb.getAssignLineByUSMatchFull(expr);
		assert(false);
	}

	std::unordered_set<std::string> AssignPatternEvaluator::patternLeftUnderscoreRightUnderScore() override {
		std::unordered_set<std::string> results = pkb.getAssignLineByUSUS();
		assert(false);
	}
};
