#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

typedef std::string PrevLine;
typedef std::string NextLine;

class FollowsTEvaluator : public Evaluator {
public:
	FollowsTEvaluator(std::vector<Declaration> declarations, Relation relations, ResultsDatabase& rdb, PKBStorage* pkb) :
	Evaluator(declarations, relations, rdb, pkb) {}; // Constructor

	std::unordered_set<std::string> FollowsTEvaluator::leftSynonymRightSimple(std::string RIGHT_ARG) override {
		std::unordered_set<PrevLine> results = pkb->getFollowsTPrevByNext(RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> FollowsTEvaluator::leftSynonymRightSynonym() override {
		std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> results = pkb->getAllFollowsT();
		return results;
	}

	std::unordered_set<std::string> FollowsTEvaluator::leftSynonymRightUnderscore() override {
		std::unordered_set<PrevLine> results = pkb->getFollowsTPrevByUS();
		return results;
	}

	std::unordered_set<std::string> FollowsTEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG) override {
		std::unordered_set<NextLine> results = pkb->getFollowsTNextByPrev(LEFT_ARG);
		return results;

	}

	bool FollowsTEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		bool results = pkb->getFollowsTByPrevUS(LEFT_ARG);
		return results;

	}

	bool FollowsTEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		bool results = pkb->getFollowsT(LEFT_ARG, RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::string> FollowsTEvaluator::leftUnderscoreRightSynonym() override {
		std::unordered_set<NextLine> results = pkb->getFollowsTNextByUS();
		return results;
	}

	bool FollowsTEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		bool results = pkb->getFollowsTByUSNext(RIGHT_ARG);
		return results;
	}

	bool FollowsTEvaluator::leftUnderscoreRightUnderScore() override {
		bool results = pkb->getFollowsTByUSUS();
		return results;
	}
};
