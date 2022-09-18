#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <unordered_set>
#include "PKBStorage.h"
#include "simple_parser/interface.h"
#include "../TNode/TNode.h"

class PKBManager {
private:
	std::shared_ptr<PKBStorage> pkbStorage = std::make_shared<PKBStorage>();

public:

    PKBManager();
    ~PKBManager();

    std::shared_ptr<PKBStorage> PKBManager::getPKBStorage();

    //get entity set
    std::unordered_set<PKBStorage::Variable> getVariableSet();
    std::unordered_set<PKBStorage::Procedure> getProcedureSet();
    std::unordered_set<PKBStorage::Constant> getConstantSet();
    std::unordered_set<PKBStorage::LineNum> getWhileSet();
    std::unordered_set<PKBStorage::LineNum> getIfSet();
    std::unordered_set<PKBStorage::LineNum> getAssignSet();
    std::unordered_set<PKBStorage::LineNum> getReadSet();
    std::unordered_set<PKBStorage::LineNum> getPrintSet();
    std::unordered_set<PKBStorage::LineNum> getStmtSet();


    //Modifies
    bool getModifies(const PKBStorage::LineNum, const PKBStorage::Variable);
    bool getModifiesUS(const PKBStorage::LineNum);
    std::unordered_set<PKBStorage::Variable> getModifiesVarByStmt(const PKBStorage::LineNum);
    std::unordered_set<PKBStorage::LineNum> getModifiesStmtByVar(const PKBStorage::Variable);
    std::unordered_set<std::pair<PKBStorage::LineNum, PKBStorage::Variable>, PairHasher::pairHash> getAllModifies();
    std::unordered_set<PKBStorage::LineNum> getModifiesStmtByUS();

    //Uses
    bool getUses(const PKBStorage::LineNum, const PKBStorage::Variable);
    bool getUsesUS(const PKBStorage::LineNum);
    std::unordered_set<PKBStorage::Variable> getUsesVarByStmt(const PKBStorage::LineNum);
    std::unordered_set<PKBStorage::LineNum> getUsesStmtByVar(const PKBStorage::Variable);
    std::unordered_set<std::pair<PKBStorage::LineNum, PKBStorage::Variable>, PairHasher::pairHash> getAllUses();
    std::unordered_set<PKBStorage::LineNum> getUsesStmtByUS();

    //Follows
    bool getFollows(const PKBStorage::PrevLine, const PKBStorage::NextLine);
    bool getFollowsByPrevUS(const PKBStorage::PrevLine);
    bool getFollowsByUSNext(const PKBStorage::NextLine);
    bool getFollowsByUSUS();
    std::unordered_set<PKBStorage::NextLine> getFollowsNextByPrev(const PKBStorage::PrevLine);
    std::unordered_set<PKBStorage::PrevLine> getFollowsPrevByNext(const PKBStorage::NextLine);
    std::unordered_set<PKBStorage::PrevLine> getFollowsPrevByUS();
    std::unordered_set<PKBStorage::NextLine> getFollowsNextByUS();
    std::unordered_set<std::pair<PKBStorage::PrevLine, PKBStorage::NextLine>, PairHasher::pairHash> getAllFollows();

    //FollowsT
    bool getFollowsT(const PKBStorage::PrevLine, const PKBStorage::NextLine);
    bool getFollowsTByPrevUS(const PKBStorage::PrevLine);
    bool getFollowsTByUSNext(const PKBStorage::NextLine);
    bool getFollowsTByUSUS();
    std::unordered_set<PKBStorage::NextLine> getFollowsTNextByPrev(const PKBStorage::PrevLine);
    std::unordered_set<PKBStorage::PrevLine> getFollowsTPrevByNext(const PKBStorage::NextLine);
    std::unordered_set<PKBStorage::PrevLine> getFollowsTPrevByUS();
    std::unordered_set<PKBStorage::NextLine> getFollowsTNextByUS();
    std::unordered_set<std::pair<PKBStorage::PrevLine, PKBStorage::NextLine>, PairHasher::pairHash> getAllFollowsT();

    //Parent
    bool getParent(const PKBStorage::ParentLine, const PKBStorage::ChildLine);
    bool getParentByParentUS(const PKBStorage::ParentLine);
    bool getParentByUSChild(const PKBStorage::ChildLine);
    bool getParentByUSUS();
    std::unordered_set<PKBStorage::ChildLine> getParentChildByParent(const PKBStorage::ParentLine);
    std::unordered_set<PKBStorage::ParentLine> getParentParentByChild(const PKBStorage::ChildLine);
    std::unordered_set<PKBStorage::ParentLine> getParentParentByUS();
    std::unordered_set<PKBStorage::ChildLine> getParentChildByUS();
    std::unordered_set<std::pair<PKBStorage::ParentLine, PKBStorage::ChildLine>, PairHasher::pairHash> getAllParent();

    //ParentT
    bool getParentT(const PKBStorage::ParentLine, const PKBStorage::ChildLine);
    bool getParentTByParentUS(const PKBStorage::ParentLine);
    bool getParentTByUSChild(const PKBStorage::ChildLine);
    bool getParentTByUSUS();
    std::unordered_set<PKBStorage::ChildLine> getParentTChildByParent(const PKBStorage::ParentLine);
    std::unordered_set<PKBStorage::ParentLine> getParentTParentByChild(const PKBStorage::ChildLine);
    std::unordered_set<PKBStorage::ParentLine> getParentTParentByUS();
    std::unordered_set<PKBStorage::ChildLine> getParentTChildByUS();
    std::unordered_set<std::pair<PKBStorage::ParentLine, PKBStorage::ChildLine>, PairHasher::pairHash> getAllParentT();

    //AssignPattern
    std::unordered_set<PKBStorage::LineNum> getAssignLineByVarUS(const PKBStorage::Variable);
    std::unordered_set<PKBStorage::LineNum> getAssignLineByVarMatchFull(const PKBStorage::Variable, const PKBStorage::ExprStr);
    std::unordered_set<PKBStorage::LineNum> getAssignLineByVarMatchPartial(const PKBStorage::Variable, const PKBStorage::ExprStr);
    std::unordered_set<std::pair<PKBStorage::LineNum, PKBStorage::Variable>, PairHasher::pairHash> getAssignLineVarByUS();
    std::unordered_set<std::pair<PKBStorage::LineNum, PKBStorage::Variable>, PairHasher::pairHash> getAssignLineVarByMatchFull(const PKBStorage::ExprStr);
    std::unordered_set<std::pair<PKBStorage::LineNum, PKBStorage::Variable>, PairHasher::pairHash> getAssignLineVarByMatchPartial(const PKBStorage::ExprStr);
    std::unordered_set<PKBStorage::LineNum> getAssignLineByUSUS();
    std::unordered_set<PKBStorage::LineNum> getAssignLineByUSMatchFull(const PKBStorage::ExprStr);
    std::unordered_set<PKBStorage::LineNum> getAssignLineByUSMatchPartial(const PKBStorage::ExprStr);

    // utility
    void setStarFromBaseMap(std::unordered_set<std::pair<std::string, std::string>, PairHasher::pairHash>& set,
                            std::unordered_map<std::string, std::unordered_set<std::string>>& star,
                            const std::unordered_map<std::string, std::unordered_set<std::string>> base, std::string key);
};
