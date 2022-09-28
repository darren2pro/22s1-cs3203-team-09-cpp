#include "PKBManager.h"
#include "../SourceProcessor/SimpleInterface.h"

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

    std::unordered_set<LineNum> PKBManager::getStmtSet() {
        return pkbStorage->stmtSet;
    }

    //Modifies
    bool PKBManager::getModifies(const LineNum lineNum, const Variable var) {
        return pkbStorage->modifiesSet.find(std::make_pair(lineNum, var)) != pkbStorage->modifiesSet.end();
    }

    bool PKBManager::getModifiesUS(const LineNum lineNum) {
        return pkbStorage->modifiesLineToVarMap.find(lineNum) != pkbStorage->modifiesLineToVarMap.end();
    }

    std::unordered_set<Variable> PKBManager::getModifiesVarByStmt(const LineNum lineNum) {
        if (pkbStorage->modifiesLineToVarMap.find(lineNum) != pkbStorage->modifiesLineToVarMap.end()) {
            return pkbStorage->modifiesLineToVarMap.at(lineNum);
        } else {
            return std::unordered_set<Variable>();
        }
    
    
    }

    std::unordered_set<LineNum> PKBManager::getModifiesStmtByVar(const Variable var) {
        if (pkbStorage->modifiesVarToLineMap.find(var) != pkbStorage->modifiesVarToLineMap.end()) {
            return pkbStorage->modifiesVarToLineMap.at(var);
        } else {
            return std::unordered_set<LineNum>();
        }
    
    
    }

    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PKBManager::getAllModifies() {
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
        if (pkbStorage->usesLineToVarMap.find(lineNum) != pkbStorage->usesLineToVarMap.end()) {
            return pkbStorage->usesLineToVarMap.at(lineNum);
        } else {
            return std::unordered_set<Variable>();
        }
    }

    std::unordered_set<LineNum> PKBManager::getUsesStmtByVar(const Variable var) {
        if (pkbStorage->usesVarToLineMap.find(var) != pkbStorage->usesVarToLineMap.end()) {
            return pkbStorage->usesVarToLineMap.at(var);
        } else {
            return std::unordered_set<LineNum>();
        }
    }

    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PKBManager::getAllUses() {
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
        if (pkbStorage->followsPrevToNextMap.find(prev) != pkbStorage->followsPrevToNextMap.end()) {
            return pkbStorage->followsPrevToNextMap.at(prev);
        } else {
            return std::unordered_set<NextLine>();
        }
    }

    std::unordered_set<PrevLine> PKBManager::getFollowsPrevByNext(const NextLine next) {
        if (pkbStorage->followsNextToPrevMap.find(next) != pkbStorage->followsNextToPrevMap.end()) {
            return pkbStorage->followsNextToPrevMap.at(next);
        } else {
            return std::unordered_set<PrevLine>();
        }
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

    std::unordered_set<std::pair<PrevLine, NextLine>, pairHash> PKBManager::getAllFollows() {
        return pkbStorage->followsSet;
    }

    //FollowsT
    bool PKBManager::getFollowsT(const PrevLine prev, const NextLine next) {
        return pkbStorage->followsTSet.find(std::make_pair(prev, next)) != pkbStorage->followsTSet.end();
    }

    bool PKBManager::getFollowsTByPrevUS(const PrevLine prev) {
        return getFollowsByPrevUS(prev);
    }

    bool PKBManager::getFollowsTByUSNext(const NextLine next) {
        return getFollowsByUSNext(next);
    }

    bool PKBManager::getFollowsTByUSUS() {
        return getFollowsByUSUS();
    }

    std::unordered_set<NextLine> PKBManager::getFollowsTNextByPrev(const PrevLine prev) {
        if (pkbStorage->followsTPrevToNextMap.find(prev) != pkbStorage->followsTPrevToNextMap.end()) {
            return pkbStorage->followsTPrevToNextMap.at(prev);
        } else {
            return std::unordered_set<NextLine>();
        }
    }

    std::unordered_set<PrevLine> PKBManager::getFollowsTPrevByNext(const NextLine next) {
        if (pkbStorage->followsTNextToPrevMap.find(next) != pkbStorage->followsTNextToPrevMap.end()) {
            return pkbStorage->followsTNextToPrevMap.at(next);
        } else {
            return std::unordered_set<PrevLine>();
        }
    
    }

    std::unordered_set<PrevLine> PKBManager::getFollowsTPrevByUS() {
        return getFollowsPrevByUS();
    }

    std::unordered_set<NextLine> PKBManager::getFollowsTNextByUS() {
        return getFollowsNextByUS();
    }

    std::unordered_set<std::pair<PrevLine, NextLine>, pairHash> PKBManager::getAllFollowsT() {
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
        if (pkbStorage->parentParentToChildMap.find(parent) != pkbStorage->parentParentToChildMap.end()) {
            return pkbStorage->parentParentToChildMap.at(parent);
        } else {
            return std::unordered_set<ChildLine>();
        }
    }

    std::unordered_set<ParentLine> PKBManager::getParentParentByChild(const ChildLine child) {
        if (pkbStorage->parentChildToParentMap.find(child) != pkbStorage->parentChildToParentMap.end()) {
            return pkbStorage->parentChildToParentMap.at(child);
        } else {
            return std::unordered_set<ParentLine>();
        }
    }
    std::unordered_set<ParentLine> PKBManager::getParentParentByUS() {
        std::unordered_set<ParentLine> set;
        for (const auto& elem : pkbStorage->parentParentToChildMap) {
            set.insert(elem.first);
        }
        return set;
    }
    std::unordered_set<ChildLine> PKBManager::getParentChildByUS() {
        std::unordered_set<ChildLine> set;
        for (const auto& elem : pkbStorage->parentChildToParentMap) {
            set.insert(elem.first);
        }
        return set;
    }

    std::unordered_set<std::pair<ParentLine, ChildLine>, pairHash> PKBManager::getAllParent() {
        return pkbStorage->parentSet;
    }

    //ParentT
    bool PKBManager::getParentT(const ParentLine parent, const ChildLine child) {
        return pkbStorage->parentTSet.find(std::make_pair(parent, child)) != pkbStorage->parentTSet.end();
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
        if (pkbStorage->parentTParentToChildMap.find(parent) != pkbStorage->parentTParentToChildMap.end()) {
            return pkbStorage->parentTParentToChildMap.at(parent);
        }
        else {
            return std::unordered_set<ChildLine>();
        }
    }

    std::unordered_set<ParentLine> PKBManager::getParentTParentByChild(const ChildLine child) {
        if (pkbStorage->parentTChildToParentMap.find(child) != pkbStorage->parentTChildToParentMap.end()) {
            return pkbStorage->parentTChildToParentMap.at(child);
        }
        else {
            return std::unordered_set<ParentLine>();
        }
    }

    std::unordered_set<ParentLine> PKBManager::getParentTParentByUS() {
        return getParentParentByUS();
    }

    std::unordered_set<ChildLine> PKBManager::getParentTChildByUS() {
        return getParentChildByUS();
    }

    std::unordered_set<std::pair<ParentLine, ChildLine>, pairHash> PKBManager::getAllParentT() {
        return pkbStorage->parentTSet;
    }

    //Calls
    bool PKBManager::getCalls(const CallerProc caller, const CalleeProc callee) {
        return pkbStorage->callsSet.find(std::make_pair(caller, callee)) != pkbStorage->callsSet.end();
    }

    bool PKBManager::getCallsByCallerUS(const CallerProc caller) {
        return pkbStorage->callsCallerToCalleeMap.find(caller) != pkbStorage->callsCallerToCalleeMap.end();
    }

    bool PKBManager::getCallsByUSCallee(const CalleeProc callee) {
        return pkbStorage->callsCalleeToCallerMap.find(callee) != pkbStorage->callsCalleeToCallerMap.end();
    }

    bool PKBManager::getCallsByUSUS() {
        return !(pkbStorage->callsSet.empty());
    }

    std::unordered_set<CalleeProc> PKBManager::getCallsCalleeByCaller(const CallerProc caller) {
        if (pkbStorage->callsCallerToCalleeMap.find(caller) != pkbStorage->callsCallerToCalleeMap.end()) {
            return pkbStorage->callsCallerToCalleeMap.at(caller);
        }
        else {
            return std::unordered_set<CalleeProc>();
        }
    }

    std::unordered_set<CallerProc> PKBManager::getCallsCallerByCallee(const CalleeProc callee) {
        if (pkbStorage->callsCalleeToCallerMap.find(callee) != pkbStorage->callsCalleeToCallerMap.end()) {
            return pkbStorage->callsCalleeToCallerMap.at(callee);
        }
        else {
            return std::unordered_set<CallerProc>();
        }
    }

    std::unordered_set<CallerProc> PKBManager::getCallsCallerByUS() {
        std::unordered_set<CallerProc> set;
        for (const auto& elem : pkbStorage->callsCallerToCalleeMap) {
            set.insert(elem.first);
        }
        return set;
    }

    std::unordered_set<CalleeProc> PKBManager::getCallsCalleeByUS() {
        std::unordered_set<CalleeProc> set;
        for (const auto& elem : pkbStorage->callsCalleeToCallerMap) {
            set.insert(elem.first);
        }
        return set;
    }

    std::unordered_set<std::pair<CallerProc, CalleeProc>, pairHash> PKBManager::getAllCalls() {
        return pkbStorage->callsSet;
    }

    //CallsT
    bool PKBManager::getCallsT(const CallerProc caller, const CalleeProc callee) {
        return pkbStorage->callsTSet.find(std::make_pair(caller, callee)) != pkbStorage->callsTSet.end();
    }

    bool PKBManager::getCallsTByCallerUS(const CallerProc caller) {
        return getCallsByCallerUS(caller);
    }

    bool PKBManager::getCallsTByUSCallee(const CalleeProc callee) {
        return getCallsByUSCallee(callee);
    }

    bool PKBManager::getCallsTByUSUS() {
        return getCallsByUSUS();
    }

    std::unordered_set<CalleeProc> PKBManager::getCallsTCalleeByCaller(const CallerProc caller) {
        if (pkbStorage->callsTCallerToCalleeMap.find(caller) != pkbStorage->callsTCallerToCalleeMap.end()) {
            return pkbStorage->callsTCallerToCalleeMap.at(caller);
        }
        else {
            return std::unordered_set<CalleeProc>();
        }
    }

    std::unordered_set<CallerProc> PKBManager::getCallsTCallerByCallee(const CalleeProc callee) {
        if (pkbStorage->callsTCalleeToCallerMap.find(callee) != pkbStorage->callsTCalleeToCallerMap.end()) {
            return pkbStorage->callsTCalleeToCallerMap.at(callee);
        }
        else {
            return std::unordered_set<CallerProc>();
        }

    }

    std::unordered_set<CallerProc> PKBManager::getCallsTCallerByUS() {
        return getCallsCallerByUS();
    }

    std::unordered_set<CalleeProc> PKBManager::getCallsTCalleeByUS() {
        return getCallsCalleeByUS();
    }

    std::unordered_set<std::pair<CallerProc, CalleeProc>, pairHash> PKBManager::getAllCallsT() {
        for (const auto& elem : pkbStorage->callsSet) {
            getCallsTCalleeByCaller(elem.first);
        }
        return pkbStorage->callsTSet;
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
        if (pkbStorage->assignExprToLineVarMap.find(expr) != pkbStorage->assignExprToLineVarMap.end()) {
            for (const auto& elem : pkbStorage->assignExprToLineVarMap.at(expr)) {
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

    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PKBManager::getAssignLineVarByMatchFull(const  ExprStr expr) {
        if (pkbStorage->assignExprToLineVarMap.find(expr) != pkbStorage->assignExprToLineVarMap.end()) {
            return pkbStorage->assignExprToLineVarMap.at(expr);
        } else {
            return std::unordered_set<std::pair<LineNum, Variable>, pairHash>();
        }
    }

    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PKBManager::getAssignLineVarByMatchPartial(const  ExprStr expr) {
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

    std::unordered_set<LineNum> PKBManager::getAssignLineByUSMatchFull(const  ExprStr expr) {
        std::unordered_set<LineNum> set;
        if (pkbStorage->assignExprToLineVarMap.find(expr) != pkbStorage->assignExprToLineVarMap.end()) {
            for (const auto& elem : pkbStorage->assignExprToLineVarMap.at(expr)) {
                set.insert(elem.first);
            }
        }
        return set;
    }

    std::unordered_set<LineNum> PKBManager::getAssignLineByUSMatchPartial(const  ExprStr expr) {
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


