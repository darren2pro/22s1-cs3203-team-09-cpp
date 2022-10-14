#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

typedef std::string PrevLine;
typedef std::string NextLine;

class AffectsEvaluator : public Evaluator {
public:
	AffectsEvaluator(std::vector<Declaration> declarations, Relation relations, ResultsDatabase& rdb, PKBManager* pkb) :
	Evaluator(declarations, relations, rdb, pkb) {}; // Constructor

	std::unordered_set<std::string> AffectsEvaluator::leftSynonymRightSimple(std::string RIGHT_ARG) override {
		//std::unordered_set<PrevLine> results = pkb->getAffectsCallerByCallee(RIGHT_ARG);
		std::unordered_set<PrevLine> results = {};
		return results;
	}

	std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> AffectsEvaluator::leftSynonymRightSynonym() override {
		//std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> results = pkb->getAllAffects();
		std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> results = {};
		return results;
	}

	std::unordered_set<std::string> AffectsEvaluator::leftSynonymRightUnderscore() override {
		//std::unordered_set<PrevLine> results = pkb->getAffectsCallerByUS();
		std::unordered_set<PrevLine> results = {};
		return results;
	}

	std::unordered_set<std::string> AffectsEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG) override {
		//std::unordered_set<NextLine> results = pkb->getAffectsCalleeByCaller(LEFT_ARG);
		std::unordered_set<PrevLine> results = {};
		return results;

	}

	bool AffectsEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		// Affects(1, _)
		//bool results = pkb->getAffectsByCallerUS(LEFT_ARG);
		bool results = true;
		return results;

	}

	bool AffectsEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		//bool results = pkb->getAffects(LEFT_ARG, RIGHT_ARG);
		bool results = true;
		return results;
	}

	std::unordered_set<std::string> AffectsEvaluator::leftUnderscoreRightSynonym() override {
		//std::unordered_set<NextLine> results = pkb->getAffectsCalleeByUS();
		std::unordered_set<PrevLine> results = {};
		return results;
	}

	bool AffectsEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		// Affects(_, 1)
		//bool results = pkb->getAffectsByUSCallee(RIGHT_ARG);
		bool results = true;
		return results;
	}

	bool AffectsEvaluator::leftUnderscoreRightUnderScore() override {
		//bool results = pkb->getAffectsByUSUS();
		bool results = true;
		return results;
	}
};
