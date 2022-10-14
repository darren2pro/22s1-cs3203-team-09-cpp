#include "PKBManager.h"
#include "../SourceProcessor/SimpleInterface.h"

namespace PKB {
    PKBManager::PKBManager() {}
    PKBManager::~PKBManager() {}

    //getter for pkbStorage. Used by SimpleParser. DesignExtractor extractor(pkb.getPKBStorage).
    std::shared_ptr<PKBStorage> PKBManager::getPKBStorage() {
        return pkbStorage;
    }

    std::unordered_map<PrevLine, std::unordered_set<NextLine>> PKBManager::getCFG() {
        return pkbStorage->cfgPrevLineToNextLineMap;
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

    std::unordered_set<LineNum> PKBManager::getStmtSet() {
        return pkbStorage->stmtSet;
    }

    std::unordered_set<LineNum> PKBManager::getCallSet() {
        return pkbStorage->callSet;
    }

    //ModifiesS
    bool PKBManager::getModifiesS(const LineNum lineNum, const Variable var) {
        return pkbStorage->modifiesSRelations.containsSet(lineNum, var);
        //return pkbStorage->modifiesSSet.find(std::make_pair(lineNum, var)) != pkbStorage->modifiesSSet.end();
    }

    bool PKBManager::getModifiesSUS(const LineNum lineNum) {
        return pkbStorage->modifiesSRelations.containsFirst(lineNum);
        //return pkbStorage->modifiesSLineToVarMap.find(lineNum) != pkbStorage->modifiesSLineToVarMap.end();
    }

    std::unordered_set<Variable> PKBManager::getModifiesSVarByStmt(const LineNum lineNum) {
        return pkbStorage->modifiesSRelations.getSecondFromFirst(lineNum);
        //if (pkbStorage->modifiesSLineToVarMap.find(lineNum) != pkbStorage->modifiesSLineToVarMap.end()) {
        //    return pkbStorage->modifiesSLineToVarMap.at(lineNum);
        //} else {
        //    return std::unordered_set<Variable>();
        //}
    }

    std::unordered_set<LineNum> PKBManager::getModifiesSStmtByVar(const Variable var) {
        return pkbStorage->modifiesSRelations.getFirstFromSecond(var);
        //if (pkbStorage->modifiesSVarToLineMap.find(var) != pkbStorage->modifiesSVarToLineMap.end()) {
        //    return pkbStorage->modifiesSVarToLineMap.at(var);
        //} else {
        //    return std::unordered_set<LineNum>();
        //}
    }

    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PKBManager::getAllModifiesS() {
        return pkbStorage->modifiesSRelations.getSet();
        //return pkbStorage->modifiesSSet;
    }

    std::unordered_set<LineNum> PKBManager::getModifiesSStmtByUS() {
        return pkbStorage->modifiesSRelations.getAllFirst();
        //std::unordered_set<LineNum> set;
        //for (const auto& elem : pkbStorage->modifiesSLineToVarMap) {
        //    set.insert(elem.first);
        //}
        //return set;
    }

    //ModifiesP
    bool PKBManager::getModifiesP(const Procedure proc, const Variable var) {
        return pkbStorage->modifiesPRelations.containsSet(proc, var);
        //return pkbStorage->modifiesPSet.find(std::make_pair(proc, var)) != pkbStorage->modifiesPSet.end();
    }

    bool PKBManager::getModifiesPUS(const Procedure proc) {
        return pkbStorage->modifiesPRelations.containsFirst(proc);
        //return pkbStorage->modifiesPProcToVarMap.find(proc) != pkbStorage->modifiesPProcToVarMap.end();
    }

    std::unordered_set<Variable> PKBManager::getModifiesPVarByProc(const Procedure proc) {
        return pkbStorage->modifiesPRelations.getSecondFromFirst(proc);
        //if (pkbStorage->modifiesPProcToVarMap.find(proc) != pkbStorage->modifiesPProcToVarMap.end()) {
        //    return pkbStorage->modifiesPProcToVarMap.at(proc);
        //}
        //else {
        //    return std::unordered_set<Variable>();
        //}
    }

    std::unordered_set<Procedure> PKBManager::getModifiesPProcByVar(const Variable var) {
        return pkbStorage->modifiesPRelations.getFirstFromSecond(var);
        //if (pkbStorage->modifiesPVarToProcMap.find(var) != pkbStorage->modifiesPVarToProcMap.end()) {
        //    return pkbStorage->modifiesPVarToProcMap.at(var);
        //}
        //else {
        //    return std::unordered_set<Procedure>();
        //}
    }

    std::unordered_set<std::pair<Procedure, Variable>, pairHash> PKBManager::getAllModifiesP() {
        return pkbStorage->modifiesPRelations.getSet();
        //return pkbStorage->modifiesPSet;
    }

    std::unordered_set<Procedure> PKBManager::getModifiesPProcByUS() {
        return pkbStorage->modifiesPRelations.getAllFirst();
        //std::unordered_set<Procedure> set;
        //for (const auto& elem : pkbStorage->modifiesPProcToVarMap) {
        //    set.insert(elem.first);
        //}
        //return set;
    }

    //UsesS
    bool PKBManager::getUsesS(const LineNum lineNum, const Variable var) {
        return pkbStorage->usesSRelations.containsSet(lineNum, var);
        //return pkbStorage->usesSSet.find(std::make_pair(lineNum, var)) != pkbStorage->usesSSet.end();
    }

    bool PKBManager::getUsesSUS(const LineNum lineNum) {
        return pkbStorage->usesSRelations.containsFirst(lineNum);
        //return pkbStorage->usesSLineToVarMap.find(lineNum) != pkbStorage->usesSLineToVarMap.end();
    }

    std::unordered_set<Variable> PKBManager::getUsesSVarByStmt(const LineNum lineNum) {
        return pkbStorage->usesSRelations.getSecondFromFirst(lineNum);
        //if (pkbStorage->usesSLineToVarMap.find(lineNum) != pkbStorage->usesSLineToVarMap.end()) {
        //    return pkbStorage->usesSLineToVarMap.at(lineNum);
        //} else {
        //    return std::unordered_set<Variable>();
        //}
    }

    std::unordered_set<LineNum> PKBManager::getUsesSStmtByVar(const Variable var) {
        return pkbStorage->usesSRelations.getFirstFromSecond(var);
        //if (pkbStorage->usesSVarToLineMap.find(var) != pkbStorage->usesSVarToLineMap.end()) {
        //    return pkbStorage->usesSVarToLineMap.at(var);
        //} else {
        //    return std::unordered_set<LineNum>();
        //}
    }

    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PKBManager::getAllUsesS() {
        return pkbStorage->usesSRelations.getSet();
        //return pkbStorage->usesSSet;
    }

    std::unordered_set<LineNum> PKBManager::getUsesSStmtByUS() {
        return pkbStorage->usesSRelations.getAllFirst();
        //std::unordered_set<LineNum> set;
        //for (const auto& elem : pkbStorage->usesSLineToVarMap) {
        //    set.insert(elem.first);
        //}
        //return set;
    }

    //UsesP
    bool PKBManager::getUsesP(const Procedure proc, const Variable var) {
        return pkbStorage->usesPRelations.containsSet(proc, var);
        //return pkbStorage->usesPSet.find(std::make_pair(proc, var)) != pkbStorage->usesPSet.end();
    }

    bool PKBManager::getUsesPUS(const Procedure proc) {
        return pkbStorage->usesPRelations.containsFirst(proc);
        //return pkbStorage->usesPProcToVarMap.find(proc) != pkbStorage->usesPProcToVarMap.end();
    }

    std::unordered_set<Variable> PKBManager::getUsesPVarByProc(const Procedure proc) {
        return pkbStorage->usesPRelations.getSecondFromFirst(proc);
        //if (pkbStorage->usesPProcToVarMap.find(proc) != pkbStorage->usesPProcToVarMap.end()) {
        //    return pkbStorage->usesPProcToVarMap.at(proc);
        //}
        //else {
        //    return std::unordered_set<Variable>();
        //}
    }

    std::unordered_set<Procedure> PKBManager::getUsesPProcByVar(const Variable var) {
        return pkbStorage->usesPRelations.getFirstFromSecond(var);
        //if (pkbStorage->usesPVarToProcMap.find(var) != pkbStorage->usesPVarToProcMap.end()) {
        //    return pkbStorage->usesPVarToProcMap.at(var);
        //}
        //else {
        //    return std::unordered_set<Procedure>();
        //}
    }

    std::unordered_set<std::pair<Procedure, Variable>, pairHash> PKBManager::getAllUsesP() {
        return pkbStorage->usesPRelations.getSet();
        //return pkbStorage->usesPSet;
    }

    std::unordered_set<Procedure> PKBManager::getUsesPProcByUS() {
        return pkbStorage->usesPRelations.getAllFirst();
        //std::unordered_set<Procedure> set;
        //for (const auto& elem : pkbStorage->usesPProcToVarMap) {
        //    set.insert(elem.first);
        //}
        //return set;
    }

    //Follows
    bool PKBManager::getFollows(const PrevLine prev, const NextLine next) {
        return pkbStorage->followsRelations.containsSet(prev, next);
        //return pkbStorage->followsSet.find(std::make_pair(prev, next)) != pkbStorage->followsSet.end();
    }

    bool PKBManager::getFollowsByPrevUS(const PrevLine prev) {
        return pkbStorage->followsRelations.containsFirst(prev);
        //return pkbStorage->followsPrevToNextMap.find(prev) != pkbStorage->followsPrevToNextMap.end();
    }

    bool PKBManager::getFollowsByUSNext(const NextLine next) {
        return pkbStorage->followsRelations.containsSecond(next);
        //return pkbStorage->followsNextToPrevMap.find(next) != pkbStorage->followsNextToPrevMap.end();
    }

    bool PKBManager::getFollowsByUSUS() {
        return !pkbStorage->followsRelations.isEmpty();
        //return !(pkbStorage->followsSet.empty());
    }

    std::unordered_set<NextLine> PKBManager::getFollowsNextByPrev(const PrevLine prev) {
        return pkbStorage->followsRelations.getSecondFromFirst(prev);
        //if (pkbStorage->followsPrevToNextMap.find(prev) != pkbStorage->followsPrevToNextMap.end()) {
        //    return pkbStorage->followsPrevToNextMap.at(prev);
        //} else {
        //    return std::unordered_set<NextLine>();
        //}
    }

    std::unordered_set<PrevLine> PKBManager::getFollowsPrevByNext(const NextLine next) {
        return pkbStorage->followsRelations.getFirstFromSecond(next);
        //if (pkbStorage->followsNextToPrevMap.find(next) != pkbStorage->followsNextToPrevMap.end()) {
        //    return pkbStorage->followsNextToPrevMap.at(next);
        //} else {
        //    return std::unordered_set<PrevLine>();
        //}
    }

    std::unordered_set<PrevLine> PKBManager::getFollowsPrevByUS() {
        return pkbStorage->followsRelations.getAllFirst();
        //std::unordered_set<PrevLine> set;
        //for (const auto& elem : pkbStorage->followsPrevToNextMap) {
        //    set.insert(elem.first);
        //}
        //return set;
    }

    std::unordered_set<NextLine> PKBManager::getFollowsNextByUS() {
        return pkbStorage->followsRelations.getAllSecond();
        //std::unordered_set<NextLine> set;
        //for (const auto& elem : pkbStorage->followsNextToPrevMap) {
        //    set.insert(elem.first);
        //}
        //return set;
    }

    std::unordered_set<std::pair<PrevLine, NextLine>, pairHash> PKBManager::getAllFollows() {
        return pkbStorage->followsRelations.getSet();
        //return pkbStorage->followsSet;
    }

    //FollowsT
    bool PKBManager::getFollowsT(const PrevLine prev, const NextLine next) {
        return pkbStorage->followsTRelations.containsSet(prev, next);
        //return pkbStorage->followsTSet.find(std::make_pair(prev, next)) != pkbStorage->followsTSet.end();
    }

    bool PKBManager::getFollowsTByPrevUS(const PrevLine prev) {
        return pkbStorage->followsTRelations.containsFirst(prev);
        //return getFollowsByPrevUS(prev);
    }

    bool PKBManager::getFollowsTByUSNext(const NextLine next) {
        return pkbStorage->followsTRelations.containsSecond(next);
        //return getFollowsByUSNext(next);
    }

    bool PKBManager::getFollowsTByUSUS() {
        return !pkbStorage->followsTRelations.isEmpty();
        //return getFollowsByUSUS();
    }

    std::unordered_set<NextLine> PKBManager::getFollowsTNextByPrev(const PrevLine prev) {
        return pkbStorage->followsTRelations.getSecondFromFirst(prev);
        //if (pkbStorage->followsTPrevToNextMap.find(prev) != pkbStorage->followsTPrevToNextMap.end()) {
        //    return pkbStorage->followsTPrevToNextMap.at(prev);
        //} else {
        //    return std::unordered_set<NextLine>();
        //}
    }

    std::unordered_set<PrevLine> PKBManager::getFollowsTPrevByNext(const NextLine next) {
        return pkbStorage->followsTRelations.getFirstFromSecond(next);
        //if (pkbStorage->followsTNextToPrevMap.find(next) != pkbStorage->followsTNextToPrevMap.end()) {
        //    return pkbStorage->followsTNextToPrevMap.at(next);
        //} else {
        //    return std::unordered_set<PrevLine>();
        //}    
    }

    std::unordered_set<PrevLine> PKBManager::getFollowsTPrevByUS() {
        return pkbStorage->followsTRelations.getAllFirst();
        //return getFollowsPrevByUS();
    }

    std::unordered_set<NextLine> PKBManager::getFollowsTNextByUS() {
        return pkbStorage->followsTRelations.getAllSecond();
        //return getFollowsNextByUS();
    }

    std::unordered_set<std::pair<PrevLine, NextLine>, pairHash> PKBManager::getAllFollowsT() {
        return pkbStorage->followsTRelations.getSet();
        //return pkbStorage->followsTSet;
    }

    //Parent
    bool PKBManager::getParent(const ParentLine parent, const ChildLine child) {
        return pkbStorage->parentRelations.containsSet(parent, child);
        //return pkbStorage->parentSet.find(std::make_pair(parent, child)) != pkbStorage->parentSet.end();
    }

    bool PKBManager::getParentByParentUS(const ParentLine parent) {
        return pkbStorage->parentRelations.containsFirst(parent);
        //return pkbStorage->parentParentToChildMap.find(parent) != pkbStorage->parentParentToChildMap.end();
    }

    bool PKBManager::getParentByUSChild(const ChildLine child) {
        return pkbStorage->parentRelations.containsSecond(child);
        //return pkbStorage->parentChildToParentMap.find(child) != pkbStorage->parentChildToParentMap.end();
    }

    bool PKBManager::getParentByUSUS() {
        return !pkbStorage->parentRelations.isEmpty();
        //return !(pkbStorage->parentSet.empty());
    }

    std::unordered_set<ChildLine> PKBManager::getParentChildByParent(const ParentLine parent) {
        return pkbStorage->parentRelations.getSecondFromFirst(parent);
        //if (pkbStorage->parentParentToChildMap.find(parent) != pkbStorage->parentParentToChildMap.end()) {
        //    return pkbStorage->parentParentToChildMap.at(parent);
        //} else {
        //    return std::unordered_set<ChildLine>();
        //}
    }

    std::unordered_set<ParentLine> PKBManager::getParentParentByChild(const ChildLine child) {
        return pkbStorage->parentRelations.getFirstFromSecond(child);
        //if (pkbStorage->parentChildToParentMap.find(child) != pkbStorage->parentChildToParentMap.end()) {
        //    return pkbStorage->parentChildToParentMap.at(child);
        //} else {
        //    return std::unordered_set<ParentLine>();
        //}
    }
    std::unordered_set<ParentLine> PKBManager::getParentParentByUS() {
        return pkbStorage->parentRelations.getAllFirst();
        //std::unordered_set<ParentLine> set;
        //for (const auto& elem : pkbStorage->parentParentToChildMap) {
        //    set.insert(elem.first);
        //}
        //return set;
    }
    std::unordered_set<ChildLine> PKBManager::getParentChildByUS() {
        return pkbStorage->parentRelations.getAllSecond();
        //std::unordered_set<ChildLine> set;
        //for (const auto& elem : pkbStorage->parentChildToParentMap) {
        //    set.insert(elem.first);
        //}
        //return set;
    }

    std::unordered_set<std::pair<ParentLine, ChildLine>, pairHash> PKBManager::getAllParent() {
        return pkbStorage->parentRelations.getSet();
        //return pkbStorage->parentSet;
    }

    //ParentT
    bool PKBManager::getParentT(const ParentLine parent, const ChildLine child) {
        return pkbStorage->parentTRelations.containsSet(parent, child);
        //return pkbStorage->parentTSet.find(std::make_pair(parent, child)) != pkbStorage->parentTSet.end();
    }

    bool PKBManager::getParentTByParentUS(const ParentLine parent) {
        return pkbStorage->parentTRelations.containsFirst(parent);
        //return getParentByParentUS(parent);
    }

    bool PKBManager::getParentTByUSChild(const ChildLine child) {
        return pkbStorage->parentTRelations.containsSecond(child);
        //return getParentByUSChild(child);
    }

    bool PKBManager::getParentTByUSUS() {
        return !pkbStorage->parentTRelations.isEmpty();
        //return getParentByUSUS();
    }

    std::unordered_set<ChildLine> PKBManager::getParentTChildByParent(const ParentLine parent) {
        return pkbStorage->parentTRelations.getSecondFromFirst(parent);
        //if (pkbStorage->parentTParentToChildMap.find(parent) != pkbStorage->parentTParentToChildMap.end()) {
        //    return pkbStorage->parentTParentToChildMap.at(parent);
        //}
        //else {
        //    return std::unordered_set<ChildLine>();
        //}
    }

    std::unordered_set<ParentLine> PKBManager::getParentTParentByChild(const ChildLine child) {
        return pkbStorage->parentTRelations.getFirstFromSecond(child);
        //if (pkbStorage->parentTChildToParentMap.find(child) != pkbStorage->parentTChildToParentMap.end()) {
        //    return pkbStorage->parentTChildToParentMap.at(child);
        //}
        //else {
        //    return std::unordered_set<ParentLine>();
        //}
    }

    std::unordered_set<ParentLine> PKBManager::getParentTParentByUS() {
        return pkbStorage->parentTRelations.getAllFirst();
        //return getParentParentByUS();
    }

    std::unordered_set<ChildLine> PKBManager::getParentTChildByUS() {
        return pkbStorage->parentTRelations.getAllSecond();
        //return getParentChildByUS();
    }

    std::unordered_set<std::pair<ParentLine, ChildLine>, pairHash> PKBManager::getAllParentT() {
        return pkbStorage->parentTRelations.getSet();
        //return pkbStorage->parentTSet;
    }

    //Calls
    bool PKBManager::getCalls(const CallerProc caller, const CalleeProc callee) {
        return pkbStorage->callsRelations.containsSet(caller, callee);
        //return pkbStorage->callsSet.find(std::make_pair(caller, callee)) != pkbStorage->callsSet.end();
    }

    bool PKBManager::getCallsByCallerUS(const CallerProc caller) {
        return pkbStorage->callsRelations.containsFirst(caller);
        //return pkbStorage->callsCallerToCalleeMap.find(caller) != pkbStorage->callsCallerToCalleeMap.end();
    }

    bool PKBManager::getCallsByUSCallee(const CalleeProc callee) {
        return pkbStorage->callsRelations.containsSecond(callee);
        //return pkbStorage->callsCalleeToCallerMap.find(callee) != pkbStorage->callsCalleeToCallerMap.end();
    }

    bool PKBManager::getCallsByUSUS() {
        return !pkbStorage->callsRelations.isEmpty();
        //return !(pkbStorage->callsSet.empty());
    }

    std::unordered_set<CalleeProc> PKBManager::getCallsCalleeByCaller(const CallerProc caller) {
        return pkbStorage->callsRelations.getSecondFromFirst(caller);
        //if (pkbStorage->callsCallerToCalleeMap.find(caller) != pkbStorage->callsCallerToCalleeMap.end()) {
        //    return pkbStorage->callsCallerToCalleeMap.at(caller);
        //}
        //else {
        //    return std::unordered_set<CalleeProc>();
        //}
    }

    std::unordered_set<CallerProc> PKBManager::getCallsCallerByCallee(const CalleeProc callee) {
        return pkbStorage->callsRelations.getFirstFromSecond(callee);
        //if (pkbStorage->callsCalleeToCallerMap.find(callee) != pkbStorage->callsCalleeToCallerMap.end()) {
        //    return pkbStorage->callsCalleeToCallerMap.at(callee);
        //}
        //else {
        //    return std::unordered_set<CallerProc>();
        //}
    }

    std::unordered_set<CallerProc> PKBManager::getCallsCallerByUS() {
        return pkbStorage->callsRelations.getAllFirst();
        //std::unordered_set<CallerProc> set;
        //for (const auto& elem : pkbStorage->callsCallerToCalleeMap) {
        //    set.insert(elem.first);
        //}
        //return set;
    }

    std::unordered_set<CalleeProc> PKBManager::getCallsCalleeByUS() {
        return pkbStorage->callsRelations.getAllSecond();
        //std::unordered_set<CalleeProc> set;
        //for (const auto& elem : pkbStorage->callsCalleeToCallerMap) {
        //    set.insert(elem.first);
        //}
        //return set;
    }

    std::unordered_set<std::pair<CallerProc, CalleeProc>, pairHash> PKBManager::getAllCalls() {
        return pkbStorage->callsRelations.getSet();
        //return pkbStorage->callsSet;
    }

    //CallsT
    bool PKBManager::getCallsT(const CallerProc caller, const CalleeProc callee) {
        return pkbStorage->callsTRelations.containsSet(caller, callee);
        //return pkbStorage->callsTSet.find(std::make_pair(caller, callee)) != pkbStorage->callsTSet.end();
    }

    bool PKBManager::getCallsTByCallerUS(const CallerProc caller) {
        return pkbStorage->callsTRelations.containsFirst(caller);
        //return getCallsByCallerUS(caller);
    }

    bool PKBManager::getCallsTByUSCallee(const CalleeProc callee) {
        return pkbStorage->callsTRelations.containsSecond(callee);
        //return getCallsByUSCallee(callee);
    }

    bool PKBManager::getCallsTByUSUS() {
        return !pkbStorage->callsTRelations.isEmpty();
        //return getCallsByUSUS();
    }

    std::unordered_set<CalleeProc> PKBManager::getCallsTCalleeByCaller(const CallerProc caller) {
        return pkbStorage->callsTRelations.getSecondFromFirst(caller);
        //if (pkbStorage->callsTCallerToCalleeMap.find(caller) != pkbStorage->callsTCallerToCalleeMap.end()) {
        //    return pkbStorage->callsTCallerToCalleeMap.at(caller);
        //}
        //else {
        //    return std::unordered_set<CalleeProc>();
        //}
    }

    std::unordered_set<CallerProc> PKBManager::getCallsTCallerByCallee(const CalleeProc callee) {
        return pkbStorage->callsTRelations.getFirstFromSecond(callee);
        //if (pkbStorage->callsTCalleeToCallerMap.find(callee) != pkbStorage->callsTCalleeToCallerMap.end()) {
        //    return pkbStorage->callsTCalleeToCallerMap.at(callee);
        //}
        //else {
        //    return std::unordered_set<CallerProc>();
        //}
    }

    std::unordered_set<CallerProc> PKBManager::getCallsTCallerByUS() {
        return pkbStorage->callsTRelations.getAllFirst();
        //return getCallsCallerByUS();
    }

    std::unordered_set<CalleeProc> PKBManager::getCallsTCalleeByUS() {
        return pkbStorage->callsTRelations.getAllSecond();
        //return getCallsCalleeByUS();
    }

    std::unordered_set<std::pair<CallerProc, CalleeProc>, pairHash> PKBManager::getAllCallsT() {
        return pkbStorage->callsTRelations.getSet();
        //return pkbStorage->callsTSet;
    }

    //AssignPattern
    std::unordered_set<LineNum> PKBManager::getAssignLineByVarUS(const Variable var) {
        std::unordered_set<LineNum> set;
        if (pkbStorage->assignVarToLineExprMap.find(var) != pkbStorage->assignVarToLineExprMap.end()) {
            for (const auto& elem : pkbStorage->assignVarToLineExprMap.at(var)) {
                set.insert(elem.first);
            }
        }
        return set;
    }

    std::unordered_set<LineNum> PKBManager::getAssignLineByVarMatchFull(const Variable var, const  ExprStr expr) {
        std::unordered_set<LineNum> set;
        std::string pattern = expr;
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '_'), pattern.end());
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '\"'), pattern.end());

        Expr exprNode = SimpleInterface::parseExpression(pattern);
        ExprStr patternStr = std::visit([](const auto& node) { return node->toString(); }, exprNode);

        if (pkbStorage->assignExprToLineVarMap.find(patternStr) != pkbStorage->assignExprToLineVarMap.end()) {
            for (const auto& elem : pkbStorage->assignExprToLineVarMap.at(patternStr)) {
                if (elem.second == var) {
                    set.insert(elem.first);
                }
            }
        }
        return set;
    }

    std::unordered_set<LineNum> PKBManager::getAssignLineByVarMatchPartial(const Variable var, const  ExprStr expr) {
        //get pattern string
        std::unordered_set<LineNum> set;
        std::string pattern = expr;
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '_'), pattern.end());
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '\"'), pattern.end());
        Expr exprNode = SimpleInterface::parseExpression(pattern);
        ExprStr patternStr = std::visit([](const auto& node) { return node->toString(); }, exprNode);

        if (pkbStorage->assignVarToLineExprMap.find(var) != pkbStorage->assignVarToLineExprMap.end()) {
            for (const auto& elem : pkbStorage->assignVarToLineExprMap.at(var)) {
                LineNum line = elem.first;
                ExprStr exprStr = elem.second;
                if (exprStr.find(patternStr) != std::string::npos) {
                    set.insert(line);
                }
            }
        }
        return set;
    }

    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PKBManager::getAssignLineVarByUS() {
        return pkbStorage->assignLineVarSet;
    }

    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PKBManager::getAssignLineVarByMatchFull(const ExprStr expr) {
        std::string pattern = expr;
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '_'), pattern.end());
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '\"'), pattern.end());

        Expr exprNode = SimpleInterface::parseExpression(pattern);
        ExprStr patternStr = std::visit([](const auto& node) { return node->toString(); }, exprNode);

        if (pkbStorage->assignExprToLineVarMap.find(patternStr) != pkbStorage->assignExprToLineVarMap.end()) {
            return pkbStorage->assignExprToLineVarMap.at(patternStr);
        } else {
            return std::unordered_set<std::pair<LineNum, Variable>, pairHash>();
        }
    }

    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PKBManager::getAssignLineVarByMatchPartial(const ExprStr expr) {
        //get pattern string
        std::unordered_set<std::pair<LineNum, Variable>, pairHash> set;
        std::string pattern = expr;
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '_'), pattern.end());
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '\"'), pattern.end());
        Expr exprNode = SimpleInterface::parseExpression(pattern);
        ExprStr patternStr = std::visit([](const auto& node) { return node->toString(); }, exprNode);

        for (const auto& elem : pkbStorage->assignExprToLineVarMap) {
            ExprStr exprStr = elem.first;
            if (exprStr.find(patternStr) != std::string::npos) {
                for (const auto& ele : elem.second) {
                    set.insert(ele);
                }
            }
        }
        return set;
    }

    std::unordered_set<LineNum> PKBManager::getAssignLineByUSUS() {
        return pkbStorage->assignSet;
    }

    std::unordered_set<LineNum> PKBManager::getAssignLineByUSMatchFull(const ExprStr expr) {
        std::unordered_set<LineNum> set;
        std::string pattern = expr;
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '_'), pattern.end());
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '\"'), pattern.end());

        Expr exprNode = SimpleInterface::parseExpression(pattern);
        ExprStr patternStr = std::visit([](const auto& node) { return node->toString(); }, exprNode);

        if (pkbStorage->assignExprToLineVarMap.find(patternStr) != pkbStorage->assignExprToLineVarMap.end()) {
            for (const auto& elem : pkbStorage->assignExprToLineVarMap.at(patternStr)) {
                set.insert(elem.first);
            }
        }
        return set;
    }

    std::unordered_set<LineNum> PKBManager::getAssignLineByUSMatchPartial(const ExprStr expr) {
        //get pattern string
        std::unordered_set<LineNum> set;
        std::string pattern = expr;
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '_'), pattern.end());
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '\"'), pattern.end());

        Expr exprNode = SimpleInterface::parseExpression(pattern);
        ExprStr patternStr = std::visit([](const auto& node) { return node->toString(); }, exprNode);

        for (const auto& elem : pkbStorage->assignExprToLineVarMap) {
            ExprStr exprStr = elem.first;
            if (exprStr.find(patternStr) != std::string::npos) {
                for (const auto& ele : elem.second) {
                    set.insert(ele.first);
                }
            }
        }
        return set;
    }
}


