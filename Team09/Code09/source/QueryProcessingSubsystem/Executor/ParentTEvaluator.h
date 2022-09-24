#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

typedef std::string ParentTLine;
typedef std::string ChildLine;

class ParentTEvaluator : public Evaluator {
public:
	ParentTEvaluator(std::vector<Declaration> declarations, Relation relations, ResultsDatabase& rdb, PKBManager* pkb) :
	Evaluator(declarations, relations, rdb, pkb) {}; // Constructor

	std::unordered_set<std::string> ParentTEvaluator::leftSynonymRightSimple(std::string RIGHT_ARG) override {
		// ParentT(a, 1) List
		std::unordered_set<ParentTLine> results = pkb->getParentTParentByChild(RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> ParentTEvaluator::leftSynonymRightSynonym() override {
		// ParentT(a, w) ListPair
		std::unordered_set<std::pair<ParentLine, ChildLine>, PKB::pairHash> results = pkb->getAllParentT();
		return results;
	}

	std::unordered_set<std::string> ParentTEvaluator::leftSynonymRightUnderscore() override {
		// ParentT(a, _) List
		std::unordered_set<ParentTLine> results = pkb->getParentTParentByUS();
		return results;
	}

	std::unordered_set<std::string> ParentTEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG) override {
		// ParentT(a, _) List
		std::unordered_set<ChildLine> results = pkb->getParentTChildByParent(LEFT_ARG);
		return results;

	}

	bool ParentTEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		bool results = pkb->getParentTByParentUS(LEFT_ARG);
		return results;

	}

	bool ParentTEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		bool results = pkb->getParentT(LEFT_ARG, RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::string> ParentTEvaluator::leftUnderscoreRightSynonym() override {
		std::unordered_set<ChildLine> results = pkb->getParentTChildByUS();
		return results;
	}

	bool ParentTEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		bool results = pkb->getParentTByUSChild(RIGHT_ARG);
		return results;
	}

	bool ParentTEvaluator::leftUnderscoreRightUnderScore() override {
		bool results = pkb->getParentTByUSUS();
		return results;
	}
};
