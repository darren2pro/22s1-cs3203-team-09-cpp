#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

typedef std::string PrevLine;
typedef std::string NextLine;

class AffectsTEvaluator : public Evaluator {
public:
	AffectsTEvaluator(std::vector<Declaration> declarations, Relation relations, ResultsDatabase& rdb, PKBManager* pkb) :
	Evaluator(declarations, relations, rdb, pkb) {}; // Constructor

	std::unordered_set<std::string> AffectsTEvaluator::leftSynonymRightSimple(std::string RIGHT_ARG) override {
		//std::unordered_set<PrevLine> results = pkb->getAffectsTCallerByCallee(RIGHT_ARG);
		std::unordered_set<PrevLine> results = {};
		return results;
	}

	std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> AffectsTEvaluator::leftSynonymRightSynonym() override {
		//std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> results = pkb->getAllAffectsT();
		std::unordered_set<std::pair<PrevLine, NextLine>, PKB::pairHash> results = {};
		return results;
	}

	std::unordered_set<std::string> AffectsTEvaluator::leftSynonymRightUnderscore() override {
		//std::unordered_set<PrevLine> results = pkb->getAffectsTCallerByUS();
		std::unordered_set<PrevLine> results = {};
		return results;
	}

	std::unordered_set<std::string> AffectsTEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG) override {
		//std::unordered_set<NextLine> results = pkb->getAffectsTCalleeByCaller(LEFT_ARG);
		std::unordered_set<PrevLine> results = {};
		return results;

	}

	bool AffectsTEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		// AffectsT(1, _)
		//bool results = pkb->getAffectsTByCallerUS(LEFT_ARG);
		bool results = true;
		return results;

	}

	bool AffectsTEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		//bool results = pkb->getAffectsT(LEFT_ARG, RIGHT_ARG);
		bool results = true;
		return results;
	}

	std::unordered_set<std::string> AffectsTEvaluator::leftUnderscoreRightSynonym() override {
		//std::unordered_set<NextLine> results = pkb->getAffectsTCalleeByUS();
		std::unordered_set<PrevLine> results = {};
		return results;
	}

	bool AffectsTEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		// AffectsT(_, 1)
		//bool results = pkb->getAffectsTByUSCallee(RIGHT_ARG);
		bool results = true;
		return results;
	}

	bool AffectsTEvaluator::leftUnderscoreRightUnderScore() override {
		//bool results = pkb->getAffectsTByUSUS();
		bool results = true;
		return results;
	}
};
