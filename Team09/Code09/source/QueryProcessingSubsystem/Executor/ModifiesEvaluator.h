#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"
#include <iostream>
#include <cassert>

typedef std::string LineNum;
typedef std::string Variable;

using namespace std;

class ModifiesEvaluator : public Evaluator {
public:
	ModifiesEvaluator(std::vector<Declaration> declarations, Relation relations, ResultsDatabase& rdb,PKBManager pkb) :
	Evaluator(declarations, relations, rdb, pkb) {}; // Constructor

	std::unordered_set<LineNum> ModifiesEvaluator::leftSynonymRightSimple(std::string RIGHT_ARG) override {
		// Modifies(a, 'x') List
		std::unordered_set<LineNum> results = pkb.getModifiesStmtByVar(RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::pair<LineNum, Variable>, PairHasher::pairHash> ModifiesEvaluator::leftSynonymRightSynonym() override {
		// Modifies(a, v) PairList
		//							w   v
		// Modifies(w, v) PairList {2   a}
		//						   {10  b}
		std::unordered_set<std::pair<LineNum, Variable>, PairHasher::pairHash> results = pkb.getAllModifies();
		return results;
	};

	std::unordered_set<LineNum> ModifiesEvaluator::leftSynonymRightUnderscore() override {
		// Modifies(a, _) List
		std::unordered_set<LineNum> results = pkb.getModifiesStmtByUS();
		return results;
	}

	std::unordered_set<Variable> ModifiesEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG) override {
		// Modifies(1, v) List 
		std::unordered_set<Variable> results = pkb.getModifiesVarByStmt(LEFT_ARG);
		return results;

	}

	bool ModifiesEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		// Modifies(1, _) Boolean
		bool results = pkb.getModifiesUS(LEFT_ARG);
		return results;

	}

	bool ModifiesEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Left LineNum, Right Variable
		// Modifies(1, 'x')
		// Returns Boolean
		bool results = pkb.getModifies(LEFT_ARG, RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::string> ModifiesEvaluator::leftUnderscoreRightSynonym() override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
		return {};
	}

	bool ModifiesEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
		return false;
	}

	bool ModifiesEvaluator::leftUnderscoreRightUnderScore() override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
		return false;
	}
};
