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

class UsesSEvaluator: public Evaluator {
public:
	UsesSEvaluator(std::vector<Declaration> declarations, Relation relations, ResultsDatabase& rdb, PKBManager* pkb) :
	Evaluator(declarations, relations, rdb, pkb) {}; // Constructor

	std::unordered_set<std::string> UsesSEvaluator::leftSynonymRightSimple(std::string RIGHT_ARG) override {
		// UsesS(a, 'x') List
		std::unordered_set<LineNum> results = pkb->getUsesSStmtByVar(RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> UsesSEvaluator::leftSynonymRightSynonym() override {
		// UsesS(a, v) ListPair
		std::unordered_set<std::pair<LineNum, Variable>, PKB::pairHash> results = pkb->getAllUsesS();
		return results;
	}

	std::unordered_set<std::string> UsesSEvaluator::leftSynonymRightUnderscore() override {
		// UsesS(a, _) List
		std::unordered_set<LineNum> results = pkb->getUsesSStmtByUS();
		return results;
	}

	std::unordered_set<std::string> UsesSEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG) override {
		// UsesS(1, a) List
		std::unordered_set<Variable> results = pkb->getUsesSVarByStmt(LEFT_ARG);
		return results;

	}

	bool UsesSEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		// UsesS(1, _) Boolean
		bool results = pkb->getUsesSUS(LEFT_ARG);
		return results;

	}

	bool UsesSEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// UsesS(1, 'x') Boolean
		bool results = pkb->getUsesS(LEFT_ARG, RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::string> UsesSEvaluator::leftUnderscoreRightSynonym() override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
		return {};
	}

	bool UsesSEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
		return false;
	}

	bool UsesSEvaluator::leftUnderscoreRightUnderScore() override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
		return false;
	}
};
