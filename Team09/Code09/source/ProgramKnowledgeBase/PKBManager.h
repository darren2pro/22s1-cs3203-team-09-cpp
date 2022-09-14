#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PKBStorage.h"

namespace PKB {

class PKBManager {
private:
	std::shared_ptr<PKBStorage> pkbStorage = std::make_shared<PKBStorage>();
public:
	PKBManager();
	~PKBManager();

	std::shared_ptr<PKBStorage> PKBManager::getPKBStorage();

	//get entity set
	std::unordered_set<Variable> getVariableSet();
	std::unordered_set<Procedure> getProcedureSet();
	std::unordered_set<Constant> getConstantSet();
	std::unordered_set<LineNum> getWhileSet();
	std::unordered_set<LineNum> getIfSet();
	std::unordered_set<LineNum> getAssignSet();
	std::unordered_set<LineNum> getReadSet();
	std::unordered_set<LineNum> getPrintSet();


	//Modifies
	bool getModifies(const LineNum, const Variable);
	bool getModifiesUS(LineNum);
	std::unordered_set<Variable> getModifiesVarByStmt(const LineNum);
	std::unordered_set<LineNum> getModifiesStmtByVar(const Variable);
	std::unordered_set<std::pair<LineNum, Variable>> getAllModifies();
	std::unordered_set<LineNum> getModifiesStmtByUS();

	//Uses
	bool getUses(const LineNum, const Variable);
	bool getUsesUS(LineNum);
	std::unordered_set<Variable> getUsesVarByStmt(const LineNum);
	std::unordered_set<LineNum> getUsesStmtByVar(const Variable);
	std::unordered_set<std::pair<LineNum, Variable>> getAllUses();
	std::unordered_set<LineNum> getUsesStmtByUS();

	//Follows
	bool getFollows(const PrevLine, const NextLine);
	bool getFollowsByPrevUS(const PrevLine);
	bool getFollowsByUSNext(const NextLine);
	bool getFollowsByUSUS();
	std::unordered_set<NextLine> getFollowsNextByPrev(const PrevLine);
	std::unordered_set<PrevLine> getFollowsPrevByNext(const NextLine);
	std::unordered_set<PrevLine> getFollowsPrevByUS();
	std::unordered_set<NextLine> getFollowsNextByUS();
	std::unordered_set<std::pair<PrevLine, NextLine>> getAllFollows();

	//FollowsT
	bool getFollowsT(const PrevLine, const NextLine);
	bool getFollowsTByPrevUS(const PrevLine);
	bool getFollowsTByUSNext(const NextLine);
	bool getFollowsTByUSUS();
	std::unordered_set<NextLine> getFollowsTNextByPrev(const PrevLine);
	std::unordered_set<PrevLine> getFollowsTPrevByNext(const NextLine);
	std::unordered_set<PrevLine> getFollowsTPrevByUS();
	std::unordered_set<NextLine> getFollowsTNextByUS();
	std::unordered_set<std::pair<PrevLine, NextLine>> getAllFollowsT();


	//Parent
	bool getParent(ParentLine, ChildLine);
	bool getParentByParentUS(ParentLine);
	bool getParentByUSChild(ChildLine);
	bool getParentByUSUS();
	std::unordered_set<ChildLine> getParentChildByParent(ParentLine);
	std::unordered_set<ParentLine> getParentParentByChild(ChildLine);
	std::unordered_set<ParentLine> getParentParentByUS();
	std::unordered_set<ChildLine> getParentChildByUS();
	std::unordered_set<std::pair<ParentLine, ChildLine>> getAllParent();


	//ParentT
	bool getParentT(ParentLine, ChildLine);
	bool getParentTByParentUS(ParentLine);
	bool getParentTByUSChild(ChildLine);
	bool getParentTByUSUS();
	std::unordered_set<ChildLine> getParentTChildByParent(ParentLine);
	std::unordered_set<ParentLine> getParentTParentByChild(ChildLine);
	std::unordered_set<ParentLine> getParentTParentByUS();
	std::unordered_set<ChildLine> getParentTChildByUS();
	std::unordered_set<std::pair<ParentLine, ChildLine>> getAllParentT();
};

}





