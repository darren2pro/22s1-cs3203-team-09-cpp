#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PKBManager.h"

namespace PKB {

PKBManager::PKBManager() {}
PKBManager::~PKBManager() {}

//getter for pkbStorage. Used by SimpleParser. DesignExtractor extractor(pkb.getPKBStorage).
std::shared_ptr<PKBStorage> PKBManager::getPKBStorage() {
    return pkbStorage;
}


std::unordered_set<Variable> PKBManager::getVariableSet() {
    return pkbStorage->varSet;
}


std::unordered_set<Procedure> PKBManager::getProcedureSet() {
    return pkbStorage->procSet;
}


std::unordered_set<Constant> PKBManager::getConstantSet() {
    return pkbStorage->constSet;
}


std::unordered_set<LineNum> PKBManager::getWhileSet() {
    return pkbStorage->whileSet;
}


std::unordered_set<LineNum> PKBManager::getIfSet() {
    return pkbStorage->ifSet;
}


std::unordered_set<LineNum> PKBManager::getAssignSet() {
    return pkbStorage->assignSet;
}


std::unordered_set<LineNum> PKBManager::getReadSet() {
    return pkbStorage->readSet;
}


std::unordered_set<LineNum> PKBManager::getPrintSet() {
    return pkbStorage->printSet;
}

//Modifies
bool PKBManager::getModifies(const LineNum lineNum, const Variable var) {
    return pkbStorage->modifiesSet.contains(std::make_pair(lineNum, var));
}

bool PKBManager::getModifiesUS(const LineNum lineNum) {
    return pkbStorage->modifiesLineToVarMap.contains(lineNum);
}

std::unordered_set<Variable> PKBManager::getModifiesVarByStmt(const LineNum lineNum) {
    return pkbStorage->modifiesLineToVarMap.at(lineNum);
}

std::unordered_set<LineNum> PKBManager::getModifiesStmtByVar(const Variable var) {
    return pkbStorage->modifiesVarToLineMap.at(var);
}

std::unordered_set<std::pair<LineNum, Variable>> PKBManager::getAllModifies() {
    return pkbStorage->modifiesSet;
}

std::unordered_set<LineNum> PKBManager::getModifiesStmtByUS() {
    std::unordered_set<LineNum> set;
    for (const auto& elem : pkbStorage->modifiesLineToVarMap) {
        set.insert(elem.first);
    }
    return set;
}

//Uses
bool PKBManager::getUses(const LineNum lineNum, const Variable var) {
    return pkbStorage->usesSet.contains(std::make_pair(lineNum, var));
}

bool PKBManager::getUsesUS(const LineNum lineNum) {
    return pkbStorage->usesLineToVarMap.contains(lineNum);
}

std::unordered_set<Variable> PKBManager::getUsesVarByStmt(const LineNum lineNum) {
    return pkbStorage->usesVarToLineMap.at(lineNum);
}

std::unordered_set<LineNum> PKBManager::getUsesStmtByVar(const Variable var) {
    return pkbStorage->usesVarToLineMap.at(var);
}

std::unordered_set<std::pair<LineNum, Variable>> PKBManager::getAllUses() {
    return pkbStorage->usesSet;
}

std::unordered_set<LineNum> PKBManager::getUsesStmtByUS() {
    std::unordered_set<LineNum> set;
    for (const auto& elem : pkbStorage->usesLineToVarMap) {
        set.insert(elem.first);
    }
    return set;
}

//Follows
bool PKBManager::getFollows(const PrevLine prev, const NextLine next) {
    return pkbStorage->followsSet.contains(std::make_pair(prev, next));
}

bool PKBManager::getFollowsByPrevUS(const PrevLine prev) {
    return pkbStorage->followsPrevToNextMap.contains(prev);
}

bool PKBManager::getFollowsByUSNext(const NextLine next) {
    return pkbStorage->followsNextToPrevMap.contains(next);
}

bool PKBManager::getFollowsByUSUS() {
    return !(pkbStorage->followsSet.empty());
}

std::unordered_set<NextLine> PKBManager::getFollowsNextByPrev(const PrevLine prev) {
    return pkbStorage->followsPrevToNextMap.at(prev);
}

std::unordered_set<PrevLine> PKBManager::getFollowsPrevByNext(const NextLine next) {
    return pkbStorage->followsNextToPrevMap.at(next);
}

std::unordered_set<PrevLine> PKBManager::getFollowsPrevByUS() {
    std::unordered_set<PrevLine> set;
    for (const auto& elem : pkbStorage->followsPrevToNextMap) {
        set.insert(elem.first);
    }
    return set;
}

std::unordered_set<NextLine> PKBManager::getFollowsNextByUS() {
    std::unordered_set<NextLine> set;
    for (const auto& elem : pkbStorage->followsNextToPrevMap) {
        set.insert(elem.first);
    }
    return set;
}

std::unordered_set<std::pair<PrevLine, NextLine>> PKBManager::getAllFollows() {
    return pkbStorage->followsSet;
}

//FollowsT
bool PKBManager::getFollowsT(const PrevLine prev, const NextLine next) {
    getFollowsTNextByPrev(prev);
    return pkbStorage->followsTSet.contains(std::make_pair(prev, next));
}

bool PKBManager::getFollowsTByPrevUS(const PrevLine prev) {
    return getFollowsByPrevUS;
}

bool PKBManager::getFollowsTByUSNext(const NextLine next) {
    return getFollowsByUSNext;
}

bool PKBManager::getFollowsTByUSUS() {
    return getFollowsByUSUS();
}

std::unordered_set<NextLine> PKBManager::getFollowsTNextByPrev(const PrevLine prev) {
    setStarFromBaseMap(pkbStorage->followsTSet, pkbStorage->followsTPrevToNextMap,
        pkbStorage->followsPrevToNextMap, prev);
    return pkbStorage->followsTPrevToNextMap.at(prev);
}

std::unordered_set<PrevLine> PKBManager::getFollowsTPrevByNext(const NextLine next) {
    setStarFromBaseMap(pkbStorage->followsTSet, pkbStorage->followsTNextToPrevMap,
        pkbStorage->followsNextToPrevMap, next);
    return pkbStorage->followsTNextToPrevMap.at(next);
}

std::unordered_set<PrevLine> PKBManager::getFollowsTPrevByUS() {
    return getFollowsPrevByUS();
}

std::unordered_set<NextLine> PKBManager::getFollowsTNextByUS() {
    return getFollowsNextByUS();
}

std::unordered_set<std::pair<PrevLine, NextLine>> PKBManager::getAllFollowsT() {
    for (const auto& elem : pkbStorage->followsSet) {
        getFollowsTNextByPrev(elem.first);
    }
    return pkbStorage->followsTSet;
}


//Parent
bool PKBManager::getParent(const ParentLine parent, const ChildLine child) {
    return pkbStorage->parentSet.contains(std::make_pair(parent, child));
}

bool PKBManager::getParentByParentUS(const ParentLine parent) {
    return pkbStorage->parentParentToChildMap.contains(parent);
}

bool PKBManager::getParentByUSChild(const ChildLine child) {
    return pkbStorage->parentChildToParentMap.contains(child);
}

bool PKBManager::getParentByUSUS() {
    return !(pkbStorage->parentSet.empty());
}

std::unordered_set<ChildLine> PKBManager::getParentChildByParent(const ParentLine parent) {
    return pkbStorage->parentParentToChildMap.at(parent);
}

std::unordered_set<ParentLine> PKBManager::getParentParentByChild(const ChildLine child) {
    return pkbStorage->parentChildToParentMap.at(child);
}

std::unordered_set<ChildLine> PKBManager::getParentChildByUS() {
    std::unordered_set<ChildLine> set;
    for (const auto& elem : pkbStorage->followsPrevToNextMap) {
        set.insert(elem.first);
    }
    return set;
}

std::unordered_set<std::pair<ParentLine, ChildLine>> PKBManager::getAllParent() {
    return pkbStorage->parentSet;
}

//ParentT
bool PKBManager::getParentT(const ParentLine parent, const ChildLine child) {
    getParentTChildByParent(child);
    return pkbStorage->followsTSet.contains(std::make_pair(parent, child));
}

bool PKBManager::getParentTByParentUS(const ParentLine) {
    return getParentByParentUS;
}

bool PKBManager::getParentTByUSChild(const ChildLine) {
    return getParentByUSChild;
}

bool PKBManager::getParentTByUSUS() {
    return getParentByUSUS;
}

std::unordered_set<ChildLine> PKBManager::getParentTChildByParent(const ParentLine parent) {
    setStarFromBaseMap(pkbStorage->parentTSet, pkbStorage->parentTChildToParentMap,
        pkbStorage->parentChildToParentMap);
    return pkbStorage->parentTChildToParentMap.at(parent);
}

std::unordered_set<ParentLine> PKBManager::getParentTParentByChild(const ChildLine child) {
    setStarFromBaseMap(pkbStorage->parentTSet, pkbStorage->parentTParentToChildMap,
        pkbStorage->parentParentToChildMap);
    return pkbStorage->parentTParentToChildMap.at(child);
}

std::unordered_set<ParentLine> PKBManager::getParentTParentByUS() {
    return getParentParentByUS();
}

std::unordered_set<ChildLine> PKBManager::getParentTChildByUS() {
    return getParentChildByUS();
}

std::unordered_set<std::pair<ParentLine, ChildLine>> PKBManager::getAllParentT() {
    for (const auto& elem : pkbStorage->parentSet) {
        getParentTChildByParent(elem.first);
    }
    return pkbStorage->parentTSet;
}

void PKBManager::setStarFromBaseMap(std::unordered_set<std::pair<std::string, std::string>>& set,
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

}
