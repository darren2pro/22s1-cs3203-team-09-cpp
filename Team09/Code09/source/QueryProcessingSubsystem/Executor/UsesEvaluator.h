#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"
#include <iostream>
#include <cassert>

typedef std::string LineNum;
typedef std::string Variable;

using namespace std;
using namespace PairHasher;

class UsesEvaluator: public Evaluator {
public:
	UsesEvaluator(std::vector<Declaration> declarations, Relation relations, ResultsDatabase rdb, PKBManager pkb) :
	Evaluator(declarations, relations, rdb, pkb) {}; // Constructor

	std::unordered_set<std::string> UsesEvaluator::leftSynonymRightSimple(std::string RIGHT_ARG) override {
		// Uses(a, 'x') List
		std::unordered_set<LineNum> results = pkb.getUsesStmtByVar(RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::pair<std::string, std::string>, PairHasher::pairHash> UsesEvaluator::leftSynonymRightSynonym() override {
		// Uses(a, v) ListPair
		std::unordered_set<std::pair<LineNum, Variable>, PairHasher::pairHash> results = pkb.getAllUses();
		return results;
	}

	std::unordered_set<std::string> UsesEvaluator::leftSynonymRightUnderscore() override {
		// Uses(a, _) List
		std::unordered_set<LineNum> results = pkb.getUsesStmtByUS();
		return results;
	}

	std::unordered_set<std::string> UsesEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG) override {
		// Uses(1, a) List
		std::unordered_set<Variable> results = pkb.getUsesVarByStmt(LEFT_ARG);
		return results;

	}

	bool UsesEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		// Uses(1, _) Boolean
		bool results = pkb.getUsesUS(LEFT_ARG);
		return results;

	}

	bool UsesEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Uses(1, 'x') Boolean
		bool results = pkb.getUses(LEFT_ARG, RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::string> UsesEvaluator::leftUnderscoreRightSynonym() override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
		return {};
	}

	bool UsesEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
		return false;
	}

	bool UsesEvaluator::leftUnderscoreRightUnderScore() override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
		return false;
	}
};
