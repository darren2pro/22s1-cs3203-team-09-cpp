#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <unordered_set>
#include "PKBStorage.h"
#include "PKBUtils.h"
#include "../TNode/TNode.h"

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
        std::unordered_set<LineNum> getStmtSet();


        //Modifies
        bool getModifies(const LineNum, const Variable);
        bool getModifiesUS(const LineNum);
        std::unordered_set<Variable> getModifiesVarByStmt(const LineNum);
        std::unordered_set<LineNum> getModifiesStmtByVar(const Variable);
        std::unordered_set<std::pair<LineNum, Variable>, pairHash> getAllModifies();
        std::unordered_set<LineNum> getModifiesStmtByUS();

        //Uses
        bool getUses(const LineNum, const Variable);
        bool getUsesUS(const LineNum);
        std::unordered_set<Variable> getUsesVarByStmt(const LineNum);
        std::unordered_set<LineNum> getUsesStmtByVar(const Variable);
        std::unordered_set<std::pair<LineNum, Variable>, pairHash> getAllUses();
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
        std::unordered_set<std::pair<PrevLine, NextLine>, pairHash> getAllFollows();

        //FollowsT
        bool getFollowsT(const PrevLine, const NextLine);
        bool getFollowsTByPrevUS(const PrevLine);
        bool getFollowsTByUSNext(const NextLine);
        bool getFollowsTByUSUS();
        std::unordered_set<NextLine> getFollowsTNextByPrev(const PrevLine);
        std::unordered_set<PrevLine> getFollowsTPrevByNext(const NextLine);
        std::unordered_set<PrevLine> getFollowsTPrevByUS();
        std::unordered_set<NextLine> getFollowsTNextByUS();
        std::unordered_set<std::pair<PrevLine, NextLine>, pairHash> getAllFollowsT();

        //Parent
        bool getParent(const ParentLine, const ChildLine);
        bool getParentByParentUS(const ParentLine);
        bool getParentByUSChild(const ChildLine);
        bool getParentByUSUS();
        std::unordered_set<ChildLine> getParentChildByParent(const ParentLine);
        std::unordered_set<ParentLine> getParentParentByChild(const ChildLine);
        std::unordered_set<ParentLine> getParentParentByUS();
        std::unordered_set<ChildLine> getParentChildByUS();
        std::unordered_set<std::pair<ParentLine, ChildLine>, pairHash> getAllParent();

        //ParentT
        bool getParentT(const ParentLine, const ChildLine);
        bool getParentTByParentUS(const ParentLine);
        bool getParentTByUSChild(const ChildLine);
        bool getParentTByUSUS();
        std::unordered_set<ChildLine> getParentTChildByParent(const ParentLine);
        std::unordered_set<ParentLine> getParentTParentByChild(const ChildLine);
        std::unordered_set<ParentLine> getParentTParentByUS();
        std::unordered_set<ChildLine> getParentTChildByUS();
        std::unordered_set<std::pair<ParentLine, ChildLine>, pairHash> getAllParentT();

        //Calls
        bool getCalls(const CallerProc, const CalleeProc);
        bool getCallsByCallerUS(const CallerProc);
        bool getCallsByUSCallee(const CalleeProc);
        bool getCallsByUSUS();
        std::unordered_set<CalleeProc> getCallsCalleeByCaller(const CallerProc);
        std::unordered_set<CallerProc> getCallsCallerByCallee(const CalleeProc);
        std::unordered_set<CallerProc> getCallsCallerByUS();
        std::unordered_set<CalleeProc> getCallsCalleeByUS();
        std::unordered_set<std::pair<CallerProc, CalleeProc>, pairHash> getAllCalls();

        //CallsT
        bool getCallsT(const CallerProc, const CalleeProc);
        bool getCallsTByCallerUS(const CallerProc);
        bool getCallsTByUSCallee(const CalleeProc);
        bool getCallsTByUSUS();
        std::unordered_set<CalleeProc> getCallsTCalleeByCaller(const CallerProc);
        std::unordered_set<CallerProc> getCallsTCallerByCallee(const CalleeProc);
        std::unordered_set<CallerProc> getCallsTCallerByUS();
        std::unordered_set<CalleeProc> getCallsTCalleeByUS();
        std::unordered_set<std::pair<CallerProc, CalleeProc>, pairHash> getAllCallsT();

        //AssignPattern
        std::unordered_set<LineNum> getAssignLineByVarUS(const Variable);
        std::unordered_set<LineNum> getAssignLineByVarMatchFull(const Variable, const ExprStr);
        std::unordered_set<LineNum> getAssignLineByVarMatchPartial(const Variable, const ExprStr);
        std::unordered_set<std::pair<LineNum, Variable>, pairHash> getAssignLineVarByUS();
        std::unordered_set<std::pair<LineNum, Variable>, pairHash> getAssignLineVarByMatchFull(const ExprStr);
        std::unordered_set<std::pair<LineNum, Variable>, pairHash> getAssignLineVarByMatchPartial(const ExprStr);
        std::unordered_set<LineNum> getAssignLineByUSUS();
        std::unordered_set<LineNum> getAssignLineByUSMatchFull(const ExprStr);
        std::unordered_set<LineNum> getAssignLineByUSMatchPartial(const ExprStr);
    };
}
