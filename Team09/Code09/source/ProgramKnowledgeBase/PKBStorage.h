#pragma once

#include <unordered_set>
#include <unordered_map>
#include "PKBUtils.h"
#include "../TNode/TNode.h"

namespace PKB {
    class PKBStorage {
    private:
        int lineNum = 1;
        std::unordered_map<LineNum, std::shared_ptr<TNode>> lineToNodeMap;
        std::unordered_map<std::shared_ptr<TNode>, LineNum> nodeToLineMap;
        std::unordered_map<LineNum, Procedure> lineToProcMap;

        LineNum getCurrLineNumber();
        void incrementCurrLineNumber();

    public:
        //entity set
        std::unordered_set<Variable> varSet;
        std::unordered_set<Procedure> procSet;
        std::unordered_set<Constant> constSet;
        std::unordered_set<LineNum> whileSet;
        std::unordered_set<LineNum> ifSet;
        std::unordered_set<LineNum> assignSet;
        std::unordered_set<LineNum> readSet;
        std::unordered_set<LineNum> printSet;
        std::unordered_set<LineNum> stmtSet;
        std::unordered_set<LineNum> callSet;

        //Modifies
        std::unordered_set<std::pair<LineNum, Variable>, pairHash> modifiesSSet;
        std::unordered_map<LineNum, std::unordered_set<Variable>> modifiesSLineToVarMap;
        std::unordered_map<Variable, std::unordered_set<LineNum>> modifiesSVarToLineMap;
        std::unordered_set<std::pair<Procedure, Variable>, pairHash> modifiesPSet;
        std::unordered_map<Procedure, std::unordered_set<Variable>> modifiesPProcToVarMap;
        std::unordered_map<Variable, std::unordered_set<Procedure>> modifiesPVarToProcMap;

        //Uses
        std::unordered_set<std::pair<LineNum, Variable>, pairHash> usesSSet;
        std::unordered_map<LineNum, std::unordered_set<Variable>> usesSLineToVarMap;
        std::unordered_map<Variable, std::unordered_set<LineNum>> usesSVarToLineMap;
        std::unordered_set<std::pair<Procedure, Variable>, pairHash> usesPSet;
        std::unordered_map<Procedure, std::unordered_set<Variable>> usesPProcToVarMap;
        std::unordered_map<Variable, std::unordered_set<Procedure>> usesPVarToProcMap;

        //Follows
        std::unordered_set<std::pair<PrevLine, NextLine>, pairHash> followsSet;
        std::unordered_map<PrevLine, std::unordered_set<NextLine>> followsPrevToNextMap;
        std::unordered_map<NextLine, std::unordered_set<PrevLine>> followsNextToPrevMap;
        std::unordered_set<std::pair<PrevLine, NextLine>, pairHash> followsTSet;
        std::unordered_map<PrevLine, std::unordered_set<NextLine>> followsTPrevToNextMap;
        std::unordered_map<NextLine, std::unordered_set<PrevLine>> followsTNextToPrevMap;

        //Parent
        std::unordered_set<std::pair<ParentLine, ChildLine>, pairHash> parentSet;
        std::unordered_map<ParentLine, std::unordered_set<ChildLine>> parentParentToChildMap;
        std::unordered_map<ChildLine, std::unordered_set<ParentLine>> parentChildToParentMap;
        std::unordered_set<std::pair<ParentLine, ChildLine>, pairHash> parentTSet;
        std::unordered_map<ParentLine, std::unordered_set<ChildLine>> parentTParentToChildMap;
        std::unordered_map<ChildLine, std::unordered_set<ParentLine>> parentTChildToParentMap;

        //Calls
        std::unordered_set<std::pair<LineNum, Procedure>, pairHash> callLineProcSet;
        std::unordered_map<LineNum, std::unordered_set<Procedure>> callLineToProcMap;
        std::unordered_map<Procedure, std::unordered_set<LineNum>> callProcToLineMap;
        std::unordered_set<std::pair<CallerProc, CalleeProc>, pairHash> callsSet;
        std::unordered_map<CallerProc, std::unordered_set<CalleeProc>> callsCallerToCalleeMap;
        std::unordered_map<CalleeProc, std::unordered_set<CallerProc>> callsCalleeToCallerMap;
        std::unordered_set<std::pair<CallerProc, CalleeProc>, pairHash> callsTSet;
        std::unordered_map<CallerProc, std::unordered_set<CalleeProc>> callsTCallerToCalleeMap;
        std::unordered_map<CalleeProc, std::unordered_set<CallerProc>> callsTCalleeToCallerMap;

        //pattern map
        std::unordered_set<std::pair<LineNum, Variable>, pairHash> assignLineVarSet;
        std::unordered_map<ExprStr, std::unordered_set<std::pair<LineNum, Variable>, pairHash>> assignExprToLineVarMap;
        std::unordered_map<Variable, std::unordered_set<std::pair<LineNum, ExprStr>, pairHash>> assignVarToLineExprMap;

        PKBStorage();
        ~PKBStorage();

        //line number API
        LineNum storeLine(const Stmt node);
        LineNum getLineFromNode(const Stmt node);
        void storeLineToProcedure(LineNum lineNum, Procedure proc);
        Procedure getProcedureFromLine(LineNum lineNum);
        std::shared_ptr<TNode> getNodeFromLine(const LineNum line);
        void storeStmt(const LineNum lineNum);

        //store entities API
        void storeVariable(const Variable var);
        void storeProcedure(const Procedure proc);
        void storeConstant(const Constant constant);
        void storeWhile(const LineNum lineNum);
        void storeIf(const LineNum lineNum);
        void storeAssign(const LineNum lineNum);
        void storeRead(const LineNum lineNum, Variable var);
        void storePrint(const LineNum lineNum, Variable var);
        void storeCall(const LineNum lineNum, Procedure proc);

        //store relations API
        void storeFollows(const PrevLine, const NextLine);
        void storeFollowsT(const PrevLine, const NextLine);
        void storeParent(const ParentLine, const ChildLine);
        void storeParentT(const ParentLine, const ChildLine);
        void storeCalls(const CallerProc, const CalleeProc);
        void storeCallsT(const CallerProc, const CalleeProc);
        void storeUsesS(const LineNum, const Variable);
        void storeUsesP(const Procedure, const Variable);
        void storeModifiesS(const LineNum, const Variable);
        void storeModifiesP(const Procedure, const Variable);
        void storeAssignPattern(const Variable, const LineNum, const ExprStr);
    };
}
