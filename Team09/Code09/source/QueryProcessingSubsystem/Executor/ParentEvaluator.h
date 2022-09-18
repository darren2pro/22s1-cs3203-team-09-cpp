#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

typedef std::string ParentLine;
typedef std::string ChildLine;

class ParentEvaluator : public Evaluator {
public:
	ParentEvaluator(std::vector<std::string> declarations, Relation relations, ResultsDatabase rdb, PKBManager pkb) :
	Evaluator(declarations, relations, rdb, pkb) {}; // Constructor

	std::unordered_set<std::string> ParentEvaluator::leftSynonymRightSimple(std::string RIGHT_ARG) override {
		// Parent(a, 1) List
		std::unordered_set<ParentLine> results = pkb.getParentParentByChild(RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::pair<std::string, std::string>, PairHasher::pairHash> ParentEvaluator::leftSynonymRightSynonym() override {
		// Parent(a, w) ListPair
		std::unordered_set<std::pair<ParentLine, ChildLine>, PairHasher::pairHash> results = pkb.getAllParent();
		return results;
	}

	std::unordered_set<std::string> ParentEvaluator::leftSynonymRightUnderscore() override {
		// Parent(a, _) List
		std::unordered_set<ParentLine> results = pkb.getParentParentByUS();
		return results;
	}

	std::unordered_set<std::string> ParentEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG) override {
		// Parent(a, _) List
		std::unordered_set<ChildLine> results = pkb.getParentChildByParent(LEFT_ARG);
		return results;

	}

	bool ParentEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		bool results = pkb.getParentByParentUS(LEFT_ARG);
		return results;

	}

	bool ParentEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		bool results = pkb.getParent(LEFT_ARG, RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::string> ParentEvaluator::leftUnderscoreRightSynonym() override {
		std::unordered_set<ChildLine> results = pkb.getParentChildByUS();
		return results;
	}

	bool ParentEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		bool results = pkb.getParentByUSChild(RIGHT_ARG);
		return results;
	}

	bool ParentEvaluator::leftUnderscoreRightUnderScore() override {
		bool results = pkb.getParentByUSUS();
		return results;
	}
};
