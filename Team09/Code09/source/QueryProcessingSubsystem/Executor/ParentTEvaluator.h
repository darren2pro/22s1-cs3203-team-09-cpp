#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

typedef std::string ParentTLine;
typedef std::string ChildLine;

class ParentTEvaluator : public Evaluator {
public:
	ParentTEvaluator(std::vector<std::string> declarations, Relation relations, PKB::PKBStorage pkb) :
	Evaluator(declarations, relations, pkb) {}; // Constructor

	std::unordered_set<std::string> ParentTEvaluator::leftSynonymRightSimple(std::string RIGHT_ARG) override {
		// ParentT(a, 1) List
		std::unordered_set<ParentTLine> results = pkb.getParentTByChild(RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::pair<std::string, std::string>> ParentTEvaluator::leftSynonymRightSynonym() override {
		// ParentT(a, w) ListPair
		std::unordered_set<std::pair<ParentTLine, ChildLine>> results = pkb.getAllParentT();
		return results;
	}

	std::unordered_set<std::string> ParentTEvaluator::leftSynonymRightUnderscore() override {
		// ParentT(a, _) List
		std::unordered_set<ParentTLine> results = pkb.getParentTByUS();
		return results;
	}

	std::unordered_set<std::string> ParentTEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG) override {
		// ParentT(a, _) List
		std::unordered_set<ChildLine> results = pkb.getParentTChildByParentT(LEFT_ARG);
		return results;

	}

	bool ParentTEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		bool results = pkb.getParentTByParentTUS(LEFT_ARG);
		return results;

	}

	bool ParentTEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		bool results = pkb.getParentT(LEFT_ARG, RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::string> ParentTEvaluator::leftUnderscoreRightSynonym() override {
		std::unordered_set<ChildLine> results = pkb.getParentTChildByUS();
		return results;
	}

	bool ParentTEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		bool results = pkb.getParentTByUSChild(RIGHT_ARG);
		return results;
	}

	bool ParentTEvaluator::leftUnderscoreRightUnderScore() override {
		bool results = pkb.getParentTByUSUS();
		return results;
	}
};
