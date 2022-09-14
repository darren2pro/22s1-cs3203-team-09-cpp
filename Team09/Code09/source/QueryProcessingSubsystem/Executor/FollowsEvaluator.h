#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"
#include "FollowsEvaluator.h"

class FollowsEvaluator : public Evaluator {
public:
	FollowsEvaluator(std::vector<std::string> declarations, Relation relations, PKB::PKBStorage pkb) :
	Evaluator(declarations, relations, pkb) {}; // Constructor

	std::unordered_set<std::string> FollowsEvaluator::leftSynonymRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> FollowsEvaluator::leftSynonymRightSynonym(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> FollowsEvaluator::leftSynonymRightUnderscore(std::string LEFT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> FollowsEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;

	}

	std::unordered_set<std::string> FollowsEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;

	}

	std::unordered_set<std::string> FollowsEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> FollowsEvaluator::leftUnderscoreRightSynonym(std::string RIGHT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> FollowsEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> FollowsEvaluator::leftUnderscoreRightUnderScore() override {
		std::unordered_set<std::string> results = {};
		return results;
	}
};

