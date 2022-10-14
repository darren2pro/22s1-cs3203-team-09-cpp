#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

typedef std::string PrevLine;
typedef std::string NextLine;

class NextTEvaluator : public Evaluator {
public:
	NextTEvaluator(std::vector<Declaration> declarations, Relation relations, ResultsDatabase& rdb, PKBManager* pkb) :
	Evaluator(declarations, relations, rdb, pkb) {}; // Constructor

	std::unordered_set<std::string> NextTEvaluator::leftSynonymRightSimple(std::string RIGHT_ARG) override {
		//std::unordered_set<PrevLine> results = pkb->getNextTCallerByCallee(RIGHT_ARG);
		std::unordered_set<PrevLine> results = {};
		return results;
	}

	std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> NextTEvaluator::leftSynonymRightSynonym() override {
		//std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> results = pkb->getAllNextT();
		std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> results = {};
		return results;
	}

	std::unordered_set<std::string> NextTEvaluator::leftSynonymRightUnderscore() override {
		//std::unordered_set<PrevLine> results = pkb->getNextTCallerByUS();
		std::unordered_set<PrevLine> results = {};
		return results;
	}

	std::unordered_set<std::string> NextTEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG) override {
		//std::unordered_set<NextLine> results = pkb->getNextTCalleeByCaller(LEFT_ARG);
		std::unordered_set<PrevLine> results = {};
		return results;

	}

	bool NextTEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		// NextT(1, _)
		//bool results = pkb->getNextTByCallerUS(LEFT_ARG);
		bool results = true;
		return results;

	}

	bool NextTEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		//bool results = pkb->getNextT(LEFT_ARG, RIGHT_ARG);
		bool results = true;
		return results;
	}

	std::unordered_set<std::string> NextTEvaluator::leftUnderscoreRightSynonym() override {
		//std::unordered_set<NextLine> results = pkb->getNextTCalleeByUS();
		std::unordered_set<PrevLine> results = {};
		return results;
	}

	bool NextTEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		// NextT(_, 1)
		//bool results = pkb->getNextTByUSCallee(RIGHT_ARG);
		bool results = true;
		return results;
	}

	bool NextTEvaluator::leftUnderscoreRightUnderScore() override {
		//bool results = pkb->getNextTByUSUS();
		bool results = true;
		return results;
	}
};
