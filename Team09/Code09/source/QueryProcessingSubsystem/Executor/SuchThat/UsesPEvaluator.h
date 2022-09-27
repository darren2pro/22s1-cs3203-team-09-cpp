#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"
#include <iostream>
#include <cassert>

typedef std::string LineNum;
typedef std::string Variable;

using namespace std;
using namespace PKB;

class UsesPEvaluator: public Evaluator {
public:
	UsesPEvaluator(std::vector<Declaration> declarations, Relation relations, ResultsDatabase& rdb, PKBManager* pkb) :
	Evaluator(declarations, relations, rdb, pkb) {}; // Constructor

	std::unordered_set<std::string> UsesPEvaluator::leftSynonymRightSimple(std::string RIGHT_ARG) override {
		// UsesP(a, 'x') List
		std::unordered_set<LineNum> results = pkb->getUsesStmtByVar(RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> UsesPEvaluator::leftSynonymRightSynonym() override {
		// UsesP(a, v) ListPair
		std::unordered_set<std::pair<LineNum, Variable>, PKB::pairHash> results = pkb->getAllUses();
		return results;
	}

	std::unordered_set<std::string> UsesPEvaluator::leftSynonymRightUnderscore() override {
		// UsesP(a, _) List
		std::unordered_set<LineNum> results = pkb->getUsesStmtByUS();
		return results;
	}

	std::unordered_set<std::string> UsesPEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG) override {
		// UsesP(1, a) List
		std::unordered_set<Variable> results = pkb->getUsesVarByStmt(LEFT_ARG);
		return results;

	}

	bool UsesPEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		// UsesP(1, _) Boolean
		bool results = pkb->getUsesUS(LEFT_ARG);
		return results;

	}

	bool UsesPEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// UsesP(1, 'x') Boolean
		bool results = pkb->getUses(LEFT_ARG, RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::string> UsesPEvaluator::leftUnderscoreRightSynonym() override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
		return {};
	}

	bool UsesPEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
		return false;
	}

	bool UsesPEvaluator::leftUnderscoreRightUnderScore() override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
		return false;
	}
};
