#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PKB.h"
#include "PKBManager.h"

PKBManager::PKBManager() {}

PKBManager::~PKBManager() {}

using namespace PKB;

//getter for pkbStorage. Used by SimpleParser. DesignExtractor extractor(pkb.getPKBStorage).
std::shared_ptr<PKBStorage> PKBManager::getPKBStorage() {
    return pkbStorage;
}


std::unordered_set<Variable> PKB::PKBManager::getVariableSet() {
    return pkbStorage->varSet;
}

//todo
std::unordered_set<Procedure> PKB::PKBManager::getProcedureSet() {

}

//todo
std::unordered_set<Constant> PKB::PKBManager::getConstantSet() {

}

//todo
std::unordered_set<LineNum> PKB::PKBManager::getWhileSet() {
  
}

//todo
std::unordered_set<LineNum> PKB::PKBManager::getIfSet() {
  
}

//todo
std::unordered_set<LineNum> PKB::PKBManager::getAssignSet() {
  
}

//todo
std::unordered_set<LineNum> PKB::PKBManager::getReadSet() {
  
}

//todo
std::unordered_set<LineNum> PKB::PKBManager::getPrintSet() {
  
}

//Modifies
bool PKB::PKBManager::getModifies(const LineNum lineNum, const Variable var) {
    return pkbStorage->modifiesSet.contains(std::make_pair(lineNum, var);
}

bool PKB::PKBManager::getModifiesUS(const LineNum lineNum) {
    return pkbStorage->modifiesLineToVarMap.contains(lineNum);
}

std::unordered_set<Variable> PKB::PKBManager::getModifiesVarByStmt(const LineNum lineNum) {
    return pkbStorage->modifiesLineToVarMap.at(lineNum);
}

std::unordered_set<LineNum> PKB::PKBManager::getModifiesStmtByVar(const Variable var) {
    return pkbStorage->modifiesVarToLineMap.at(var);
}

std::unordered_set<std::pair<LineNum, Variable>> PKB::PKBManager::getAllModifies() {
    return pkbStorage->modifiesSet;
}

std::unordered_set<LineNum> PKB::PKBManager::getModifiesStmtByUS() {
    std::unordered_set<LineNum> set;
    for (const auto elem : pkbStorage->modifiesLineToVarMap) {
        set.insert(elem.first);
    }
    return set;
}

//todo
//Uses
bool PKB::PKBManager::getUses(const LineNum, const Variable) {

}

bool PKB::PKBManager::getUsesUS(const LineNum) {

}

std::unordered_set<Variable> PKB::PKBManager::getUsesVarByStmt(const LineNum) {

}

std::unordered_set<LineNum> PKB::PKBManager::getUsesStmtByVar(const Variable) {

}

std::unordered_set<std::pair<LineNum, Variable>> PKB::PKBManager::getAllUses() {

}

std::unordered_set<LineNum> PKB::PKBManager::getUsesStmtByUS() {

}

//Follows
bool PKB::PKBManager::getFollows(const PrevLine prev, const NextLine next) {
    return pkbStorage->followsSet.contains(std::make_pair(prev, next));
}

bool PKB::PKBManager::getFollowsByPrevUS(const PrevLine prev) {
    return pkbStorage->followsPrevToNextMap.contains(prev);
}

bool PKB::PKBManager::getFollowsByUSNext(const NextLine next) {
    return pkbStorage->followsNextToPrevMap.contains(next);
}

bool PKB::PKBManager::getFollowsByUSUS() {
    return !(pkbStorage->followsSet.empty());
}

std::unordered_set<NextLine> PKB::PKBManager::getFollowsNextByPrev(const PrevLine prev) {
    return pkbStorage->followsPrevToNextMap.at(prev);
}

std::unordered_set<PrevLine> PKB::PKBManager::getFollowsPrevByNext(const NextLine next) {
    return pkbStorage->followsNextToPrevMap.at(next);
}

std::unordered_set<PrevLine> PKB::PKBManager::getFollowsPrevByUS() {
    std::unordered_set<PrevLine> set;
    for (const auto& elem : pkbStorage->followsPrevToNextMap) {
        set.insert(elem.first);
    }
    return set;
}

std::unordered_set<NextLine> PKB::PKBManager::getFollowsNextByUS() {
    std::unordered_set<NextLine> set;
    for (const auto& elem : pkbStorage->followsNextToPrevMap) {
        set.insert(elem.first);
    }
    return set;
}

std::unordered_set<std::pair<PrevLine, NextLine>> PKB::PKBManager::getAllFollows() {
    return pkbStorage->followsSet;
}

//FollowsT
bool PKB::PKBManager::getFollowsT(const PrevLine prev, const NextLine next) {
    getFollowsTNextByPrev(prev);
    return pkbStorage->followsTSet.contains(std::make_pair(prev, next));
}

bool PKB::PKBManager::getFollowsTByPrevUS(const PrevLine) {
    return getFollowsByPrevUS;
}

bool PKB::PKBManager::getFollowsTByUSNext(const NextLine) {
    return getFollowsByUSNext;
}

bool PKB::PKBManager::getFollowsTByUSUS() {
    return getFollowsByUSUS();
}

std::unordered_set<NextLine> PKB::PKBManager::getFollowsTNextByPrev(const PrevLine prev) {
    setStarFromBaseMap(pkbStorage->followsTSet, pkbStorage->followsTPrevToNextMap,
        pkbStorage->followsPrevToNextMap, prev);
    return pkbStorage->followsTPrevToNextMap.at(prev);
}

std::unordered_set<PrevLine> PKB::PKBManager::getFollowsTPrevByNext(const NextLine next) {
    setStarFromBaseMap(pkbStorage->followsTSet, pkbStorage->followsTNextToPrevMap,
        pkbStorage->followsNextToPrevMap, next);
    return pkbStorage->followsTNextToPrevMap.at(next);
}

std::unordered_set<PrevLine> PKB::PKBManager::getFollowsTPrevByUS() {
    return getFollowsPrevByUS();
}

std::unordered_set<NextLine> PKB::PKBManager::getFollowsTNextByUS() {
    return getFollowsNextByUS();
}

std::unordered_set<std::pair<PrevLine, NextLine>> PKB::PKBManager::getAllFollowsT() {
    for (const auto& elem : pkbStorage->followsSet) {
        getFollowsTNextByPrev(elem.first);
    }
    return pkbStorage->followsTSet;
}

//todo
//Parent
bool PKB::PKBManager::getParent(const ParentLine, const ChildLine) {

}

bool PKB::PKBManager::getParentByParentUS(const ParentLine) {

}

bool PKB::PKBManager::getParentByUSChild(const ChildLine) {

}

bool PKB::PKBManager::getParentByUSUS() {

}

std::unordered_set<ChildLine> PKB::PKBManager::getParentChildByParent(const ParentLine) {

}

std::unordered_set<ParentLine> PKB::PKBManager::getParentParentByChild(const ChildLine) {

}

std::unordered_set<ChildLine> PKB::PKBManager::getParentChildByUS() {

}

std::unordered_set<std::pair<ParentLine, ChildLine>> PKB::PKBManager::getAllParent() {

}

//ParentT
bool PKB::PKBManager::getParentT(const ParentLine, const ChildLine) {

}

bool PKB::PKBManager::getParentTByParentUS(const ParentLine) {

}

bool PKB::PKBManager::getParentTByUSChild(const ChildLine) {

}

bool PKB::PKBManager::getParentTByUSUS() {

}

std::unordered_set<ChildLine> PKB::PKBManager::getParentTChildByParent(const ParentLine) {

}

std::unordered_set<ParentLine> PKB::PKBManager::getParentTParentByChild(const ChildLine) {

}

std::unordered_set<ParentLine> PKB::PKBManager::getParentTParentByUS() {

}

std::unordered_set<ChildLine> PKB::PKBManager::getParentTChildByUS() {

}

std::unordered_set<std::pair<ParentLine, ChildLine>> PKB::PKBManager::getAllParentT() {

}

void PKB::PKBManager::setStarFromBaseMap(std::unordered_set<std::pair<std::string, std::string>>& set,
    std::unordered_map<std::string, std::unordered_set<std::string>>& star,
    const std::unordered_map<std::string, std::unordered_set<std::string>> base, std::string key) {
    std::string currKey = key;
    while (base.contains(currKey)) {
        std::string val = *(base.at(currKey).begin());
        set.insert(key, val);
        PKBStorage::addToSetInMap(star, key, val);
        currKey = val;
    }
}




