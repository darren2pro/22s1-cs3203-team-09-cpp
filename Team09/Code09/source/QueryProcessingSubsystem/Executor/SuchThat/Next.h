#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

typedef std::string PrevLine;
typedef std::string NextLine;

class NextEvaluator : public Evaluator {
public:
	NextEvaluator(std::vector<Declaration> declarations, Relation relations, ResultsDatabase& rdb, PKBManager* pkb) :
	Evaluator(declarations, relations, rdb, pkb) {}; // Constructor

	std::unordered_set<std::string> NextEvaluator::leftSynonymRightSimple(std::string RIGHT_ARG) override {
		//std::unordered_set<PrevLine> results = pkb->getNextCallerByCallee(RIGHT_ARG);
		std::unordered_set<PrevLine> results = {};
		return results;
	}

	std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> NextEvaluator::leftSynonymRightSynonym() override {
		//std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> results = pkb->getAllNext();
		std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> results = {};
		return results;
	}

	std::unordered_set<std::string> NextEvaluator::leftSynonymRightUnderscore() override {
		//std::unordered_set<PrevLine> results = pkb->getNextCallerByUS();
		std::unordered_set<PrevLine> results = {};
		return results;
	}

	std::unordered_set<std::string> NextEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG) override {
		//std::unordered_set<NextLine> results = pkb->getNextCalleeByCaller(LEFT_ARG);
		std::unordered_set<PrevLine> results = {};
		return results;

	}

	bool NextEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		// Next(1, _)
		//bool results = pkb->getNextByCallerUS(LEFT_ARG);
		bool results = true;
		return results;

	}

	bool NextEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		//bool results = pkb->getNext(LEFT_ARG, RIGHT_ARG);
		bool results = true;
		return results;
	}

	std::unordered_set<std::string> NextEvaluator::leftUnderscoreRightSynonym() override {
		//std::unordered_set<NextLine> results = pkb->getNextCalleeByUS();
		std::unordered_set<PrevLine> results = {};
		return results;
	}

	bool NextEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		// Next(_, 1)
		//bool results = pkb->getNextByUSCallee(RIGHT_ARG);
		bool results = true;
		return results;
	}

	bool NextEvaluator::leftUnderscoreRightUnderScore() override {
		//bool results = pkb->getNextByUSUS();
		bool results = true;
		return results;
	}
};
