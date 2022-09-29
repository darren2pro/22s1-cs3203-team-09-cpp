#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

typedef std::string PrevLine;
typedef std::string NextLine;

class CallsTEvaluator : public Evaluator {
public:
	CallsTEvaluator(std::vector<Declaration> declarations, Relation relations, ResultsDatabase& rdb, PKBManager* pkb) :
	Evaluator(declarations, relations, rdb, pkb) {}; // Constructor

	std::unordered_set<std::string> CallsTEvaluator::leftSynonymRightSimple(std::string RIGHT_ARG) override {
		//std::unordered_set<PrevLine> results = pkb->getCallsTPrevByNext(RIGHT_ARG);
		std::unordered_set<PrevLine> results = {};
		return results;
	}

	std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> CallsTEvaluator::leftSynonymRightSynonym() override {
		//std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> results = pkb->getAllCallsT();
		std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> results = {};
		return results;
	}

	std::unordered_set<std::string> CallsTEvaluator::leftSynonymRightUnderscore() override {
		//std::unordered_set<PrevLine> results = pkb->getCallsTPrevByUS();
		std::unordered_set<PrevLine> results = {};
		return results;
	}

	std::unordered_set<std::string> CallsTEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG) override {
		//std::unordered_set<NextLine> results = pkb->getCallsTNextByPrev(LEFT_ARG);
		std::unordered_set<PrevLine> results = {};
		return results;

	}

	bool CallsTEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		//bool results = pkb->getCallsTByPrevUS(LEFT_ARG);
		bool results = true;
		return results;

	}

	bool CallsTEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		//bool results = pkb->getCallsT(LEFT_ARG, RIGHT_ARG);
		bool results = true;
		return results;
	}

	std::unordered_set<std::string> CallsTEvaluator::leftUnderscoreRightSynonym() override {
		//std::unordered_set<NextLine> results = pkb->getCallsTNextByUS();
		std::unordered_set<PrevLine> results = {};
		return results;
	}

	bool CallsTEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		//bool results = pkb->getCallsTByUSNext(RIGHT_ARG);
		bool results = true;
		return results;
	}

	bool CallsTEvaluator::leftUnderscoreRightUnderScore() override {
		//bool results = pkb->getCallsTByUSUS();
		bool results = true;
		return results;
	}
};
