#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"
#include <iostream>
#include <cassert>

typedef std::string LineNum;
typedef std::string Variable;

using namespace std;

class ModifiesSEvaluator : public Evaluator {
public:
	ModifiesSEvaluator(std::vector<Declaration> declarations, Relation relations, ResultsDatabase& rdb,PKBManager* pkb) :
	Evaluator(declarations, relations, rdb, pkb) {}; // Constructor

	std::unordered_set<LineNum> ModifiesSEvaluator::leftSynonymRightSimple(std::string RIGHT_ARG) override {
		// ModifiesS(a, 'x') List
		std::unordered_set<LineNum> results = pkb->getModifiesSStmtByVar(RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::pair<LineNum, Variable>, PKB::pairHash> ModifiesSEvaluator::leftSynonymRightSynonym() override {
		// ModifiesS(a, v) PairList
		//							w   v
		// ModifiesS(w, v) PairList {2   a}
		//						   {10  b}
		std::unordered_set<std::pair<LineNum, Variable>, PKB::pairHash> results = pkb->getAllModifiesS();
		return results;
	};

    //! Retrieves all the possible statement numbers which modifies something. Synonym type constraint is not enforced yet.
	std::unordered_set<LineNum> ModifiesSEvaluator::leftSynonymRightUnderscore() override {
		// ModifiesS(a, _) List
		std::unordered_set<LineNum> results = pkb->getModifiesSStmtByUS();
		return results;
	}

	std::unordered_set<Variable> ModifiesSEvaluator::leftSimpleRightSynonym(std::string LEFT_ARG) override {
		// ModifiesS(1, v) List 
		std::unordered_set<Variable> results = pkb->getModifiesSVarByStmt(LEFT_ARG);
		return results;

	}

	bool ModifiesSEvaluator::leftSimpleRightUnderscore(std::string LEFT_ARG) override {
		// ModifiesS(1, _) Boolean
		bool results = pkb->getModifiesSUS(LEFT_ARG);
		return results;

	}

	bool ModifiesSEvaluator::leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Left LineNum, Right Variable
		// ModifiesS(1, 'x')
		// Returns Boolean
		bool results = pkb->getModifiesS(LEFT_ARG, RIGHT_ARG);
		return results;
	}

	std::unordered_set<std::string> ModifiesSEvaluator::leftUnderscoreRightSynonym() override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
		return {};
	}

	bool ModifiesSEvaluator::leftUnderscoreRightSimple(std::string RIGHT_ARG) override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
		return false;
	}

	bool ModifiesSEvaluator::leftUnderscoreRightUnderScore() override {
		std::cout << "Not Valid Query" << std::endl;
		assert(false);
		return false;
	}
};
