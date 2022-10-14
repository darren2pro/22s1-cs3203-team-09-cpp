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

class ModifiesPEvaluator : public Evaluator {
public:
	ModifiesPEvaluator(std::vector<Declaration> declarations, Relation relations, ResultsDatabase& rdb,PKBStorage* pkb) :
	Evaluator(declarations, relations, rdb, pkb) {}; // Constructor

	std::unordered_set<LineNum> ModifiesPEvaluator::leftSynonymRightSimple(std::string RIGHT_ARG) override {
		// ModifiesP(a, 'x') List
		std::unordered_set<LineNum> results = pkb->getModifiesPProcByVar(RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::pair<LineNum, Variable>, PKB::pairHash> ModifiesPEvaluator::leftSynonymRightSynonym() override {
		std::unordered_set<std::pair<LineNum, Variable>, PKB::pairHash> results = pkb->getAllModifiesP();
		return results;
	};

    //! Retrieves all the possible statement numbers which modifies something. Synonym type constraint is not enforced yet.
	std::unordered_set<LineNum> ModifiesPEvaluator::leftSynonymRightUnderscore() override {
		// ModifiesP(a, _) List
		std::unordered_set<LineNum> results = pkb->getModifiesPProcByUS();
		return results;
	}

	std::unordered_set<Variable> ModifiesPEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG) override {
		// ModifiesP(1, v) List 
		std::unordered_set<Variable> results = pkb->getModifiesPVarByProc(LEFT_ARG);
		return results;

	}

	bool ModifiesPEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		// ModifiesP(1, _) Boolean
		bool results = pkb->getModifiesPUS(LEFT_ARG);
		return results;

	}

	bool ModifiesPEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Left LineNum, Right Variable
		// ModifiesP(1, 'x')
		// Returns Boolean
		bool results = pkb->getModifiesP(LEFT_ARG, RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::string> ModifiesPEvaluator::leftUnderscoreRightSynonym() override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
		return {};
	}

	bool ModifiesPEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
		return false;
	}

	bool ModifiesPEvaluator::leftUnderscoreRightUnderScore() override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
		return false;
	}
};
