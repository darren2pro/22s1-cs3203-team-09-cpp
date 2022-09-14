#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

class FollowsTEvaluator : public Evaluator {
public:
	FollowsTEvaluator(std::vector<std::string> declarations, Relation relations, PKB::PKBStorage pkb) :
	Evaluator(declarations, relations, pkb) {}; // Constructor

	std::unordered_set<std::string> FollowsTEvaluator::leftSynonymRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Follows(a, 1) List 
		// Follows*(a, 1) List
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> FollowsTEvaluator::leftSynonymRightSynonym(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Follows(a, w) ListPair 
		// Follows*(a, w) ListPair
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> FollowsTEvaluator::leftSynonymRightUnderscore(std::string LEFT_ARG) override {
		// Follows*(a, _) List 
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> FollowsTEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Follows(1, a) List
		std::unordered_set<std::string> results = {};
		return results;

	}

	std::unordered_set<std::string> FollowsTEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		// Follows(1, _) Boolean
		std::unordered_set<std::string> results = {};
		return results;

	}

	std::unordered_set<std::string> FollowsTEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Follows(1,2) Boolean
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> FollowsTEvaluator::leftUnderscoreRightSynonym(std::string RIGHT_ARG) override {
		// Follows(_, a) List
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> FollowsTEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		// Follows(_, 1) Boolean
		std::unordered_set<std::string> results = {};
		return results;
	}

	std::unordered_set<std::string> FollowsTEvaluator::leftUnderscoreRightUnderScore() override {
		// Follows(_, _) Boolean
		std::unordered_set<std::string> results = {};
		return results;
	}
};
