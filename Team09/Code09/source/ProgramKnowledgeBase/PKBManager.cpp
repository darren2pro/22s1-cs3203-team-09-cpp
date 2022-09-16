#include "PKBManager.h"

PKBManager::PKBManager() {}
PKBManager::~PKBManager() {}

//getter for pkbStorage. Used by SimpleParser. DesignExtractor extractor(pkb.getPKBStorage).
std::shared_ptr<PKBStorage> PKBManager::getPKBStorage() {
    return pkbStorage;
}

std::unordered_set<PKBStorage::Variable> PKBManager::getVariableSet() {
    return pkbStorage->varSet;
}


std::unordered_set<PKBStorage::Procedure> PKBManager::getProcedureSet() {
    return pkbStorage->procSet;
}


std::unordered_set<PKBStorage::Constant> PKBManager::getConstantSet() {
    return pkbStorage->constSet;
}


std::unordered_set<PKBStorage::LineNum> PKBManager::getWhileSet() {
    return pkbStorage->whileSet;
}


std::unordered_set<PKBStorage::LineNum> PKBManager::getIfSet() {
    return pkbStorage->ifSet;
}


std::unordered_set<PKBStorage::LineNum> PKBManager::getAssignSet() {
    return pkbStorage->assignSet;
}


std::unordered_set<PKBStorage::LineNum> PKBManager::getReadSet() {
    return pkbStorage->readSet;
}


std::unordered_set<PKBStorage::LineNum> PKBManager::getPrintSet() {
    return pkbStorage->printSet;
}

//Modifies
bool PKBManager::getModifies(const PKBStorage::LineNum lineNum, const PKBStorage::Variable var) {
    return pkbStorage->modifiesSet.find(std::make_pair(lineNum, var)) != pkbStorage->modifiesSet.end();
}

bool PKBManager::getModifiesUS(const PKBStorage::LineNum lineNum) {
    return pkbStorage->modifiesLineToVarMap.find(lineNum) != pkbStorage->modifiesLineToVarMap.end();
}

std::unordered_set<PKBStorage::Variable> PKBManager::getModifiesVarByStmt(const PKBStorage::LineNum lineNum) {
    return pkbStorage->modifiesLineToVarMap.at(lineNum);
}

std::unordered_set<PKBStorage::LineNum> PKBManager::getModifiesStmtByVar(const PKBStorage::Variable var) {
    return pkbStorage->modifiesVarToLineMap.at(var);
}

std::unordered_set<std::pair<PKBStorage::LineNum, PKBStorage::Variable>, PairHasher::pairHash> PKBManager::getAllModifies() {
    return pkbStorage->modifiesSet;
}

std::unordered_set<PKBStorage::LineNum> PKBManager::getModifiesStmtByUS() {
    std::unordered_set<PKBStorage::LineNum> set;
    for (const auto& elem : pkbStorage->modifiesLineToVarMap) {
        set.insert(elem.first);
    }
    return set;
}

//Uses
bool PKBManager::getUses(const PKBStorage::LineNum lineNum, const PKBStorage::Variable var) {
    return pkbStorage->usesSet.find(std::make_pair(lineNum, var)) != pkbStorage->usesSet.end();
}

bool PKBManager::getUsesUS(const PKBStorage::LineNum lineNum) {
    return pkbStorage->usesLineToVarMap.find(lineNum) != pkbStorage->usesLineToVarMap.end();
}

std::unordered_set<PKBStorage::Variable> PKBManager::getUsesVarByStmt(const PKBStorage::LineNum lineNum) {
    return pkbStorage->usesVarToLineMap.at(lineNum);
}

std::unordered_set<PKBStorage::LineNum> PKBManager::getUsesStmtByVar(const PKBStorage::Variable var) {
    return pkbStorage->usesVarToLineMap.at(var);
}

std::unordered_set<std::pair<PKBStorage::LineNum, PKBStorage::Variable>, PairHasher::pairHash> PKBManager::getAllUses() {
    return pkbStorage->usesSet;
}

std::unordered_set<PKBStorage::LineNum> PKBManager::getUsesStmtByUS() {
    std::unordered_set<PKBStorage::LineNum> set;
    for (const auto& elem : pkbStorage->usesLineToVarMap) {
        set.insert(elem.first);
    }
    return set;
}

//Follows
bool PKBManager::getFollows(const PKBStorage::PrevLine prev, const PKBStorage::NextLine next) {
    return pkbStorage->followsSet.find(std::make_pair(prev, next)) != pkbStorage->followsSet.end();
}

bool PKBManager::getFollowsByPrevUS(const PKBStorage::PrevLine prev) {
    return pkbStorage->followsPrevToNextMap.find(prev) != pkbStorage->followsPrevToNextMap.end();
}

bool PKBManager::getFollowsByUSNext(const PKBStorage::NextLine next) {
    return pkbStorage->followsNextToPrevMap.find(next) != pkbStorage->followsNextToPrevMap.end();
}

bool PKBManager::getFollowsByUSUS() {
    return !(pkbStorage->followsSet.empty());
}

std::unordered_set<PKBStorage::NextLine> PKBManager::getFollowsNextByPrev(const PKBStorage::PrevLine prev) {
    return pkbStorage->followsPrevToNextMap.at(prev);
}

std::unordered_set<PKBStorage::PrevLine> PKBManager::getFollowsPrevByNext(const PKBStorage::NextLine next) {
    return pkbStorage->followsNextToPrevMap.at(next);
}

std::unordered_set<PKBStorage::PrevLine> PKBManager::getFollowsPrevByUS() {
    std::unordered_set<PKBStorage::PrevLine> set;
    for (const auto& elem : pkbStorage->followsPrevToNextMap) {
        set.insert(elem.first);
    }
    return set;
}

std::unordered_set<PKBStorage::NextLine> PKBManager::getFollowsNextByUS() {
    std::unordered_set<PKBStorage::NextLine> set;
    for (const auto& elem : pkbStorage->followsNextToPrevMap) {
        set.insert(elem.first);
    }
    return set;
}

std::unordered_set<std::pair<PKBStorage::PrevLine, PKBStorage::NextLine>, PairHasher::pairHash> PKBManager::getAllFollows() {
    return pkbStorage->followsSet;
}

//FollowsT
bool PKBManager::getFollowsT(const PKBStorage::PrevLine prev, const PKBStorage::NextLine next) {
    getFollowsTNextByPrev(prev);
    return pkbStorage->followsTSet.find(std::make_pair(prev, next)) != pkbStorage->followsTSet.end();
}

bool PKBManager::getFollowsTByPrevUS(const PKBStorage::PrevLine prev) {
    return getFollowsByPrevUS(prev);
}

bool PKBManager::getFollowsTByUSNext(const PKBStorage::NextLine next) {
    return getFollowsByUSNext(next);
}

bool PKBManager::getFollowsTByUSUS() {
    return getFollowsByUSUS();
}

std::unordered_set<PKBStorage::NextLine> PKBManager::getFollowsTNextByPrev(const PKBStorage::PrevLine prev) {
    setStarFromBaseMap(pkbStorage->followsTSet, pkbStorage->followsTPrevToNextMap,
                       pkbStorage->followsPrevToNextMap, prev);
    return pkbStorage->followsTPrevToNextMap.at(prev);
}

std::unordered_set<PKBStorage::PrevLine> PKBManager::getFollowsTPrevByNext(const PKBStorage::NextLine next) {
    setStarFromBaseMap(pkbStorage->followsTSet, pkbStorage->followsTNextToPrevMap,
                       pkbStorage->followsNextToPrevMap, next);
    return pkbStorage->followsTNextToPrevMap.at(next);
}

std::unordered_set<PKBStorage::PrevLine> PKBManager::getFollowsTPrevByUS() {
    return getFollowsPrevByUS();
}

std::unordered_set<PKBStorage::NextLine> PKBManager::getFollowsTNextByUS() {
    return getFollowsNextByUS();
}

std::unordered_set<std::pair<PKBStorage::PrevLine, PKBStorage::NextLine>, PairHasher::pairHash> PKBManager::getAllFollowsT() {
    for (const auto& elem : pkbStorage->followsSet) {
        getFollowsTNextByPrev(elem.first);
    }
    return pkbStorage->followsTSet;
}

//Parent
bool PKBManager::getParent(const  PKBStorage::ParentLine parent, const  PKBStorage::ChildLine child) {
    return pkbStorage->parentSet.find(std::make_pair(parent, child)) != pkbStorage->parentSet.end();
}

bool PKBManager::getParentByParentUS(const  PKBStorage::ParentLine parent) {
    return pkbStorage->parentParentToChildMap.find(parent) != pkbStorage->parentParentToChildMap.end();
}

bool PKBManager::getParentByUSChild(const  PKBStorage::ChildLine child) {
    return pkbStorage->parentChildToParentMap.find(child) != pkbStorage->parentChildToParentMap.end();
}

bool PKBManager::getParentByUSUS() {
    return !(pkbStorage->parentSet.empty());
}

std::unordered_set< PKBStorage::ChildLine> PKBManager::getParentChildByParent(const  PKBStorage::ParentLine parent) {
    return pkbStorage->parentParentToChildMap.at(parent);
}

std::unordered_set< PKBStorage::ParentLine> PKBManager::getParentParentByChild(const  PKBStorage::ChildLine child) {
    return pkbStorage->parentChildToParentMap.at(child);
}
std::unordered_set<PKBStorage::ParentLine> PKBManager::getParentParentByUS() {
    std::unordered_set<PKBStorage::ParentLine> set;
    for (const auto& elem : pkbStorage->parentParentToChildMap) {
        set.insert(elem.first);
    }
    return set;
}
std::unordered_set< PKBStorage::ChildLine> PKBManager::getParentChildByUS() {
    std::unordered_set< PKBStorage::ChildLine> set;
    for (const auto& elem : pkbStorage->parentParentToChildMap) {
        set.insert(elem.first);
    }
    return set;
}

std::unordered_set<std::pair< PKBStorage::ParentLine,  PKBStorage::ChildLine>, PairHasher::pairHash> PKBManager::getAllParent() {
    return pkbStorage->parentSet;
}

//ParentT
bool PKBManager::getParentT(const  PKBStorage::ParentLine parent, const  PKBStorage::ChildLine child) {
    getParentTChildByParent(child);
    return pkbStorage->followsTSet.find(std::make_pair(parent, child)) != pkbStorage->followsTSet.end();
}

bool PKBManager::getParentTByParentUS(const  PKBStorage::ParentLine parent) {
    return getParentByParentUS(parent);
}

bool PKBManager::getParentTByUSChild(const  PKBStorage::ChildLine child) {
    return getParentByUSChild(child);
}

bool PKBManager::getParentTByUSUS() {
    return getParentByUSUS();
}

std::unordered_set< PKBStorage::ChildLine> PKBManager::getParentTChildByParent(const  PKBStorage::ParentLine parent) {
    setStarFromBaseMap(pkbStorage->parentTSet, pkbStorage->parentTChildToParentMap,
                       pkbStorage->parentChildToParentMap, parent);
    return pkbStorage->parentTChildToParentMap.at(parent);
}

std::unordered_set< PKBStorage::ParentLine> PKBManager::getParentTParentByChild(const  PKBStorage::ChildLine child) {
    setStarFromBaseMap(pkbStorage->parentTSet, pkbStorage->parentTParentToChildMap,
                       pkbStorage->parentParentToChildMap, child);
    return pkbStorage->parentTParentToChildMap.at(child);
}

std::unordered_set< PKBStorage::ParentLine> PKBManager::getParentTParentByUS() {
    return getParentParentByUS();
}

std::unordered_set< PKBStorage::ChildLine> PKBManager::getParentTChildByUS() {
    return getParentChildByUS();
}

std::unordered_set<std::pair< PKBStorage::ParentLine,  PKBStorage::ChildLine>, PairHasher::pairHash> PKBManager::getAllParentT() {
    for (const auto& elem : pkbStorage->parentSet) {
        getParentTChildByParent(elem.first);
    }
    return pkbStorage->parentTSet;
}

//AssignPattern
std::unordered_set<PKBStorage::LineNum> PKBManager::getAssignLineByVarUS(const PKBStorage::Variable var) {
    std::unordered_set<PKBStorage::LineNum> set;
    for (const auto& elem : pkbStorage->assignVarToLineExprMap.at(var)) {
        set.insert(elem.first);
    }
    return set;
}

std::unordered_set<PKBStorage::LineNum> PKBManager::getAssignLineByVarMatchFull(const PKBStorage::Variable var, const  PKBStorage::ExprStr expr) {
    std::unordered_set<PKBStorage::LineNum> set;
    for (const auto& elem : pkbStorage->assignExprToLineVarMap.at(expr)) {
        if (elem.second == var) {
            set.insert(elem.first);
        }
    }
    return set;
}

//todo
std::unordered_set<PKBStorage::LineNum> PKBManager::getAssignLineByVarMatchPartial(const PKBStorage::Variable var, const  PKBStorage::ExprStr expr) {
    return std::unordered_set<PKBStorage::LineNum>();
}

std::unordered_set<std::pair<PKBStorage::LineNum, PKBStorage::Variable>, PairHasher::pairHash> PKBManager::getAssignLineVarByUS() {
    return pkbStorage->assignLineVarSet;
}

std::unordered_set<std::pair<PKBStorage::LineNum, PKBStorage::Variable>, PairHasher::pairHash> PKBManager::getAssignLineVarByMatchFull(const  PKBStorage::ExprStr expr) {
    return pkbStorage->assignExprToLineVarMap.at(expr);
}

//todo
std::unordered_set<std::pair<PKBStorage::LineNum, PKBStorage::Variable>, PairHasher::pairHash> PKBManager::getAssignLineVarByMatchPartial(const  PKBStorage::ExprStr expr) {
    return std::unordered_set<std::pair<PKBStorage::LineNum, PKBStorage::Variable>, PairHasher::pairHash>();
}

std::unordered_set<PKBStorage::LineNum> PKBManager::getAssignLineByUSUS() {
    return pkbStorage->assignSet;
}

std::unordered_set<PKBStorage::LineNum> PKBManager::getAssignLineByUSMatchFull(const  PKBStorage::ExprStr expr) {
    std::unordered_set<PKBStorage::LineNum> set;
    for (const auto& elem : pkbStorage->assignExprToLineVarMap.at(expr)) {
        set.insert(elem.first);
    }
    return set;
}

//todo
std::unordered_set<PKBStorage::LineNum> PKBManager::getAssignLineByUSVarMatchPartial(const  PKBStorage::ExprStr expr) {
    return std::unordered_set<PKBStorage::LineNum>();
}
void PKBManager::setStarFromBaseMap(std::unordered_set<std::pair<std::string, std::string>, PairHasher::pairHash>& set,
                                    std::unordered_map<std::string, std::unordered_set<std::string>>& star,
                                    const std::unordered_map<std::string, std::unordered_set<std::string>> base,
                                    std::string key) {
    std::string currKey = key;
    while (base.find(currKey) != base.end()) {
        std::string val = *(base.at(currKey).begin());
        set.insert(std::make_pair(key, val));
        this->pkbStorage->addToSetInMap(star, key, val);
        currKey = val;
    }
}
