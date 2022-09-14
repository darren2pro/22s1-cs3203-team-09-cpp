#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PKB.h"
#include "PKBManager.h"

PKBManager::PKBManager() {}

PKBManager::~PKBManager() {}

//getter for pkbStorage. Used by SimpleParser. DesignExtractor extractor(pkb.getPKBStorage).
std::shared_ptr<PKBStorage> PKBManager::getPKBStorage() {
    return pkbStorage;
}


std::unordered_set<Variable> PKB::PKBManager::getVariableSet() {
   
}

std::unordered_set<Procedure> PKB::PKBManager::getProcedureSet() {
    
}

std::unordered_set<Constant> PKB::PKBManager::getConstantSet() {
   
}

std::unordered_set<LineNum> PKB::PKBManager::getWhileSet() {
  
}

std::unordered_set<LineNum> PKB::PKBManager::getIfSet() {
  
}

std::unordered_set<LineNum> PKB::PKBManager::getAssignSet() {
  
}

std::unordered_set<LineNum> PKB::PKBManager::getReadSet() {
  
}

std::unordered_set<LineNum> PKB::PKBManager::getPrintSet() {
  
}


//Modifies
bool PKB::PKBManager::getModifies(const LineNum, const Variable)
{
    return false;
}

bool PKB::PKBManager::getModifiesUS(LineNum)
{
    return false;
}

std::unordered_set<Variable> PKB::PKBManager::getModifiesVarByStmt(const LineNum)
{
    return std::unordered_set<Variable>();
}

std::unordered_set<LineNum> PKB::PKBManager::getModifiesStmtByVar(const Variable)
{
    return std::unordered_set<LineNum>();
}

std::unordered_set<std::pair<LineNum, Variable>> PKB::PKBManager::getAllModifies()
{
    return std::unordered_set<std::pair<LineNum, Variable>>();
}

std::unordered_set<LineNum> PKB::PKBManager::getModifiesStmtByUS()
{
    return std::unordered_set<LineNum>();
}


//Uses
bool PKB::PKBManager::getUses(const LineNum, const Variable)
{
    return false;
}

bool PKB::PKBManager::getUsesUS(LineNum)
{
    return false;
}

std::unordered_set<Variable> PKB::PKBManager::getUsesVarByStmt(const LineNum)
{
    return std::unordered_set<Variable>();
}

std::unordered_set<LineNum> PKB::PKBManager::getUsesStmtByVar(const Variable)
{
    return std::unordered_set<LineNum>();
}

std::unordered_set<std::pair<LineNum, Variable>> PKB::PKBManager::getAllUses()
{
    return std::unordered_set<std::pair<LineNum, Variable>>();
}

std::unordered_set<LineNum> PKB::PKBManager::getUsesStmtByUS()
{
    return std::unordered_set<LineNum>();
}


//Follows
bool PKB::PKBManager::getFollows(const PrevLine, const NextLine)
{
    return false;
}

bool PKB::PKBManager::getFollowsByPrevUS(const PrevLine)
{
    return false;
}

bool PKB::PKBManager::getFollowsByUSNext(const NextLine)
{
    return false;
}

bool PKB::PKBManager::getFollowsByUSUS()
{
    return false;
}

std::unordered_set<NextLine> PKB::PKBManager::getFollowsNextByPrev(const PrevLine)
{
    return std::unordered_set<NextLine>();
}

std::unordered_set<PrevLine> PKB::PKBManager::getFollowsPrevByNext(const NextLine)
{
    return std::unordered_set<PrevLine>();
}

std::unordered_set<PrevLine> PKB::PKBManager::getFollowsPrevByUS()
{
    return std::unordered_set<PrevLine>();
}

std::unordered_set<NextLine> PKB::PKBManager::getFollowsNextByUS()
{
    return std::unordered_set<NextLine>();
}

std::unordered_set<std::pair<PrevLine, NextLine>> PKB::PKBManager::getAllFollows()
{
    return std::unordered_set<std::pair<PrevLine, NextLine>>();
}


//FollowsT
bool PKB::PKBManager::getFollowsT(const PrevLine, const NextLine)
{
    return false;
}

bool PKB::PKBManager::getFollowsTByPrevUS(const PrevLine)
{
    return false;
}

bool PKB::PKBManager::getFollowsTByUSNext(const NextLine)
{
    return false;
}

bool PKB::PKBManager::getFollowsTByUSUS()
{
    return false;
}

std::unordered_set<NextLine> PKB::PKBManager::getFollowsTNextByPrev(const PrevLine)
{
    return std::unordered_set<NextLine>();
}

std::unordered_set<PrevLine> PKB::PKBManager::getFollowsTPrevByNext(const NextLine)
{
    return std::unordered_set<PrevLine>();
}

std::unordered_set<PrevLine> PKB::PKBManager::getFollowsTPrevByUS()
{
    return std::unordered_set<PrevLine>();
}

std::unordered_set<NextLine> PKB::PKBManager::getFollowsTNextByUS()
{
    return std::unordered_set<NextLine>();
}

std::unordered_set<std::pair<PrevLine, NextLine>> PKB::PKBManager::getAllFollowsT()
{
    return std::unordered_set<std::pair<PrevLine, NextLine>>();
}


//Parent
bool PKB::PKBManager::getParent(ParentLine, ChildLine)
{
    return false;
}

bool PKB::PKBManager::getParentByParentUS(ParentLine)
{
    return false;
}

bool PKB::PKBManager::getParentByUSChild(ChildLine)
{
    return false;
}

bool PKB::PKBManager::getParentByUSUS()
{
    return false;
}

std::unordered_set<ChildLine> PKB::PKBManager::getParentChildByParent(ParentLine)
{
    return std::unordered_set<ChildLine>();
}

std::unordered_set<ParentLine> PKB::PKBManager::getParentParentByChild(ChildLine)
{
    return std::unordered_set<ParentLine>();
}

std::unordered_set<ChildLine> PKB::PKBManager::getParentChildByUS()
{
    return std::unordered_set<ChildLine>();
}

std::unordered_set<std::pair<ParentLine, ChildLine>> PKB::PKBManager::getAllParent()
{
    return std::unordered_set<std::pair<ParentLine, ChildLine>>();
}


//ParentT
bool PKB::PKBManager::getParentT(ParentLine, ChildLine)
{
    return false;
}

bool PKB::PKBManager::getParentTByParentUS(ParentLine)
{
    return false;
}

bool PKB::PKBManager::getParentTByUSChild(ChildLine)
{
    return false;
}

bool PKB::PKBManager::getParentTByUSUS()
{
    return false;
}

std::unordered_set<ChildLine> PKB::PKBManager::getParentTChildByParent(ParentLine)
{
    return std::unordered_set<ChildLine>();
}

std::unordered_set<ParentLine> PKB::PKBManager::getParentTParentByChild(ChildLine)
{
    return std::unordered_set<ParentLine>();
}

std::unordered_set<ParentLine> PKB::PKBManager::getParentTParentByUS()
{
    return std::unordered_set<ParentLine>();
}

std::unordered_set<ChildLine> PKB::PKBManager::getParentTChildByUS()
{
    return std::unordered_set<ChildLine>();
}

std::unordered_set<std::pair<ParentLine, ChildLine>> PKB::PKBManager::getAllParentT()
{
    return std::unordered_set<std::pair<ParentLine, ChildLine>>();
}


