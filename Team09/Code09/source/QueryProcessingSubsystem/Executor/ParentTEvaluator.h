#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

class ParentTEvaluator : public Evaluator {
public:
	ParentTEvaluator(std::vector<std::string> declarations, Relation relations, PKB::PKBStorage pkb) :
	Evaluator(declarations, relations, pkb) {}; // Constructor

	std::unordered_set<std::string> ParentTEvaluator::leftSynonymRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> ParentTEvaluator::leftSynonymRightSynonym(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> ParentTEvaluator::leftSynonymRightUnderscore(std::string LEFT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> ParentTEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;

	}

	std::unordered_set<std::string> ParentTEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;

	}

	std::unordered_set<std::string> ParentTEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> ParentTEvaluator::leftUnderscoreRightSynonym(std::string RIGHT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> ParentTEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> ParentTEvaluator::leftUnderscoreRightUnderScore() override {
		std::unordered_set<std::string> results = {};
		return results;
	}
};
