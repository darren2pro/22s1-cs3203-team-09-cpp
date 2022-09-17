#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

typedef std::string PrevLine;
typedef std::string NextLine;

class FollowsEvaluator : public Evaluator {
public:
	FollowsEvaluator(std::vector<std::string> declarations, Relation relations, ResultsDatabase rdb, PKBManager pkb) :
	Evaluator(declarations, relations, rdb, pkb) {}; // Constructor

	std::unordered_set<std::string> FollowsEvaluator::leftSynonymRightSimple(std::string RIGHT_ARG) override {
		std::unordered_set<PrevLine> results = pkb.getFollowsPrevByNext(RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::pair<PrevLine, NextLine>, PairHasher::pairHash> FollowsEvaluator::leftSynonymRightSynonym() override {
		std::unordered_set<std::pair<PrevLine, NextLine>, PairHasher::pairHash> results = pkb.getAllFollows();
		return results;
	}

	std::unordered_set<std::string> FollowsEvaluator::leftSynonymRightUnderscore() override {
		std::unordered_set<PrevLine> results = pkb.getFollowsPrevByUS();
		return results;
	}

	std::unordered_set<std::string> FollowsEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG) override {
		std::unordered_set<NextLine> results = pkb.getFollowsNextByPrev(LEFT_ARG);
		return results;

	}

	bool FollowsEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		bool results = pkb.getFollowsByPrevUS(LEFT_ARG);
		return results;

	}

	bool FollowsEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		bool results = pkb.getFollows(LEFT_ARG, RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::string> FollowsEvaluator::leftUnderscoreRightSynonym() override {
		std::unordered_set<NextLine> results = pkb.getFollowsNextByUS();
		return results;
	}

	bool FollowsEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		bool results = pkb.getFollowsByUSNext(RIGHT_ARG);
		return results;
	}

	bool FollowsEvaluator::leftUnderscoreRightUnderScore() override {
		bool results = pkb.getFollowsByUSUS();
		return results;
	}
};

