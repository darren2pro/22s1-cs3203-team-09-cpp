#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

class ParentEvaluator : public Evaluator {
public:
	ParentEvaluator(std::vector<std::string> declarations, Relation relations, PKB::PKBStorage pkb) :
	Evaluator(declarations, relations, pkb) {}; // Constructor

	std::unordered_set<std::string> ParentEvaluator::leftSynonymRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Parent(a, 1) List
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> ParentEvaluator::leftSynonymRightSynonym(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Parent(a, w) ListPair
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> ParentEvaluator::leftSynonymRightUnderscore(std::string LEFT_ARG) override {
		// Parent(a, _) List
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> ParentEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Parent(a, _) List
		std::unordered_set<std::string> results = {};
		std::unordered_set<std::string> results = {};
		return results;

	}

	std::unordered_set<std::string> ParentEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;

	}

	std::unordered_set<std::string> ParentEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> ParentEvaluator::leftUnderscoreRightSynonym(std::string RIGHT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> ParentEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> ParentEvaluator::leftUnderscoreRightUnderScore() override {
		std::unordered_set<std::string> results = {};
		return results;
	}
};
