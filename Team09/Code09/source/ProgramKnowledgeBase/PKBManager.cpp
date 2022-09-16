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
    return pkbStorage->modifiesSet.find(std::make_pair(lineNum, var)) != pkbStorage->modifiesSet.end();
}

bool PKBManager::getModifiesUS(const LineNum lineNum) {
    return pkbStorage->modifiesLineToVarMap.find(lineNum) != pkbStorage->modifiesLineToVarMap.end();
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
    return pkbStorage->usesSet.find(std::make_pair(lineNum, var)) != pkbStorage->usesSet.end();
}

bool PKBManager::getUsesUS(const LineNum lineNum) {
    return pkbStorage->usesLineToVarMap.find(lineNum) != pkbStorage->usesLineToVarMap.end();
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
    return pkbStorage->followsSet.find(std::make_pair(prev, next)) != pkbStorage->followsSet.end();
}

bool PKBManager::getFollowsByPrevUS(const PrevLine prev) {
    return pkbStorage->followsPrevToNextMap.find(prev) != pkbStorage->followsPrevToNextMap.end();
}

bool PKBManager::getFollowsByUSNext(const NextLine next) {
    return pkbStorage->followsNextToPrevMap.find(next) != pkbStorage->followsNextToPrevMap.end();
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
    return pkbStorage->followsTSet.find(std::make_pair(prev, next)) != pkbStorage->followsTSet.end();
}

bool PKBManager::getFollowsTByPrevUS(const PrevLine) {
    return getFollowsByPrevUS;
}

bool PKBManager::getFollowsTByUSNext(const NextLine) {
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
    return pkbStorage->parentSet.find(std::make_pair(parent, child)) != pkbStorage->parentSet.end();
}

bool PKBManager::getParentByParentUS(const ParentLine parent) {
    return pkbStorage->parentParentToChildMap.find(parent) != pkbStorage->parentParentToChildMap.end();
}

bool PKBManager::getParentByUSChild(const ChildLine child) {
    return pkbStorage->parentChildToParentMap.find(child) != pkbStorage->parentChildToParentMap.end();
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
    for (const auto& elem : pkbStorage->parentParentToChildMap) {
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
    return pkbStorage->followsTSet.find(std::make_pair(parent, child)) != pkbStorage->followsTSet.end();
}

bool PKBManager::getParentTByParentUS(const ParentLine parent) {
    return getParentByParentUS(parent);
}

bool PKBManager::getParentTByUSChild(const ChildLine child) {
    return getParentByUSChild(child);
}

bool PKBManager::getParentTByUSUS() {
    return getParentByUSUS();
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

}

