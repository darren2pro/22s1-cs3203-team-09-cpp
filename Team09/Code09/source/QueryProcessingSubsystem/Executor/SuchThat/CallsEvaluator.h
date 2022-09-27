#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

typedef std::string PrevLine;
typedef std::string NextLine;

class CallsEvaluator : public Evaluator {
public:
	CallsEvaluator(std::vector<Declaration> declarations, Relation relations, ResultsDatabase& rdb, PKBManager* pkb) :
	Evaluator(declarations, relations, rdb, pkb) {}; // Constructor

	std::unordered_set<std::string> CallsEvaluator::leftSynonymRightSimple(std::string RIGHT_ARG) override {
		//std::unordered_set<PrevLine> results = pkb->getCallsPrevByNext(RIGHT_ARG);
		std::unordered_set<PrevLine> results = {};
		return results;
	}

	std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> CallsEvaluator::leftSynonymRightSynonym() override {
		//std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> results = pkb->getAllCalls();
		std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> results = {};
		return results;
	}

	std::unordered_set<std::string> CallsEvaluator::leftSynonymRightUnderscore() override {
		//std::unordered_set<PrevLine> results = pkb->getCallsPrevByUS();
		std::unordered_set<PrevLine> results = {};
		return results;
	}

	std::unordered_set<std::string> CallsEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG) override {
		//std::unordered_set<NextLine> results = pkb->getCallsNextByPrev(LEFT_ARG);
		std::unordered_set<PrevLine> results = {};
		return results;

	}

	bool CallsEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		//bool results = pkb->getCallsByPrevUS(LEFT_ARG);
		bool results = true;
		return results;

	}

	bool CallsEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		//bool results = pkb->getCalls(LEFT_ARG, RIGHT_ARG);
		bool results = true;
		return results;
	}

	std::unordered_set<std::string> CallsEvaluator::leftUnderscoreRightSynonym() override {
		//std::unordered_set<NextLine> results = pkb->getCallsNextByUS();
		std::unordered_set<PrevLine> results = {};
		return results;
	}

	bool CallsEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		//bool results = pkb->getCallsByUSNext(RIGHT_ARG);
		bool results = true;
		return results;
	}

	bool CallsEvaluator::leftUnderscoreRightUnderScore() override {
		//bool results = pkb->getCallsByUSUS();
		bool results = true;
		return results;
	}
};
