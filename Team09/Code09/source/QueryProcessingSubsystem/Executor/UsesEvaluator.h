#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"
#include <iostream>
#include <cassert>

using namespace std;

class UsesEvaluator: public Evaluator {
public:
	UsesEvaluator(std::vector<std::string> declarations, Relation relations, PKB::PKBStorage pkb) :
	Evaluator(declarations, relations, pkb) {}; // Constructor

	std::unordered_set<std::string> UsesEvaluator::leftSynonymRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Uses(a, 'x') List
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> UsesEvaluator::leftSynonymRightSynonym(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Uses(a, v) ListPair
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> UsesEvaluator::leftSynonymRightUnderscore(std::string LEFT_ARG) override {
		// Uses(a, _) List
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> UsesEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Uses(1, a) List
		std::unordered_set<std::string> results = {};
		return results;

	}

	std::unordered_set<std::string> UsesEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		// Uses(1, _) Boolean
		std::unordered_set<std::string> results = {};
		return results;

	}

	std::unordered_set<std::string> UsesEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Uses(1, 'x') Boolean
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> UsesEvaluator::leftUnderscoreRightSynonym(std::string RIGHT_ARG) override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
	}

	std::unordered_set<std::string> UsesEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
	}

	std::unordered_set<std::string> UsesEvaluator::leftUnderscoreRightUnderScore() override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
	}
};
