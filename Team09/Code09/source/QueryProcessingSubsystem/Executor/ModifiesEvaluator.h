#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"
#include <iostream>
#include <cassert>

using namespace std;

class ModifiesEvaluator : public Evaluator {
public:
	ModifiesEvaluator(std::vector<std::string> declarations, Relation relations, PKB::PKBStorage pkb) :
	Evaluator(declarations, relations, pkb) {}; // Constructor

	std::unordered_set<std::string> ModifiesEvaluator::leftSynonymRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Modifies(a, 'x') List
		// getStatementBy
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> ModifiesEvaluator::leftSynonymRightSynonym(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Modifies(a, v) PairList
		//							w   v
		// Modifies(w, v) PairList {2   a}
		//						   {10  b}
		std::unordered_set<std::string> results = pkb.getAllModify();
		return results;
	};

	std::unordered_set<std::string> ModifiesEvaluator::leftSynonymRightUnderscore(std::string LEFT_ARG) override {
		// Modifies(a, _) List
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> ModifiesEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Modifies(1, v) List 
		std::unordered_set<std::string> results = {};
		return results;

	}

	std::unordered_set<std::string> ModifiesEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		// Modifies(1, _) Boolean
		std::unordered_set<std::string> results = {};
		return results;

	}

	std::unordered_set<std::string> ModifiesEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Modifies(1, 'x')
		// Returns Boolean
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> ModifiesEvaluator::leftUnderscoreRightSynonym(std::string RIGHT_ARG) override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
	}

	std::unordered_set<std::string> ModifiesEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
	}

	std::unordered_set<std::string> ModifiesEvaluator::leftUnderscoreRightUnderScore() override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
	}
};
