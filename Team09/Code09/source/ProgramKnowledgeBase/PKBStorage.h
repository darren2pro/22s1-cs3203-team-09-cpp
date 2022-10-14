#pragma once

<<<<<<< HEAD
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
=======
>>>>>>> 8f881df334ef0302d1b449e44a95822b4b1ed8f6
#include <unordered_set>
#include <unordered_map>
#include "PKBUtils.h"
<<<<<<< HEAD
#include "RelationsSetBiMap.h"
#include "../SourceProcessor/SimpleInterface.h"
=======
>>>>>>> 8f881df334ef0302d1b449e44a95822b4b1ed8f6
#include "../TNode/TNode.h"
#include "../QueryProcessingSubsystem/Declaration.h"
#include "../QueryProcessingSubsystem/Relation.h"


namespace PKB {
    class PKBStorage {
    private:
        int lineNum = 1;
        //std::unordered_map<LineNum, std::shared_ptr<TNode>> lineToNodeMap; (useless)
        std::unordered_map<std::shared_ptr<TNode>, LineNum> nodeToLineMap;
        std::unordered_map<LineNum, Procedure> lineToProcMap;

        LineNum getCurrLineNumber();
        void incrementCurrLineNumber();

        RelationsSetBiMap<std::string, std::string>* getRelationFromEnum(Relation::Types);
        std::unordered_set<std::string>* getEntityFromEnum(Declaration::DesignEntity);

    public:
        std::unordered_set<std::pair<LineNum, Procedure>, pairHash> lineCallsProcSet;

        //CFG
        std::unordered_map<Procedure, LineNum> procFirstLineMap;
        std::unordered_map<Procedure, std::unordered_set<LineNum>> procLastLineMap;
        std::unordered_map<PrevLine, std::unordered_set<NextLine>> cfgPrevLineToNextLineMap;
        std::unordered_map<PrevLine, std::unordered_set<NextLine>> cfgProcPrevLineToNextLineMap;

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

<<<<<<< HEAD
        //relations
        RelationsSetBiMap<LineNum, Variable> modifiesSRelations;
        RelationsSetBiMap<Procedure, Variable> modifiesPRelations;
        RelationsSetBiMap<LineNum, Variable> usesSRelations;
        RelationsSetBiMap<Procedure, Variable> usesPRelations;
        RelationsSetBiMap<PrevLine, NextLine> followsRelations;
        RelationsSetBiMap<PrevLine, NextLine> followsTRelations;
        RelationsSetBiMap<ParentLine, ChildLine> parentRelations;
        RelationsSetBiMap<ParentLine, ChildLine> parentTRelations;
        RelationsSetBiMap<CallerProc, CalleeProc> callsRelations;
        RelationsSetBiMap<CallerProc, CalleeProc> callsTRelations;
=======
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
>>>>>>> 8f881df334ef0302d1b449e44a95822b4b1ed8f6

        //pattern map
        std::unordered_set<std::pair<LineNum, Variable>, pairHash> assignLineVarSet;
        std::unordered_map<ExprStr, std::unordered_set<std::pair<LineNum, Variable>, pairHash>> assignExprToLineVarMap;
        std::unordered_map<Variable, std::unordered_set<std::pair<LineNum, ExprStr>, pairHash>> assignVarToLineExprMap;

        PKBStorage();
        ~PKBStorage();

        void storeLineCallsProc(LineNum lineNum, Procedure proc);

        //line number API
        LineNum storeLine(const Stmt node);
        LineNum getLineFromNode(const Stmt node);
        void storeLineToProcedure(LineNum lineNum, Procedure proc);
        Procedure getProcedureFromLine(LineNum lineNum);
        //std::shared_ptr<TNode> getNodeFromLine(const LineNum line); (useless)

        //store CFG
        void storeProcFirstLine(const Procedure, const LineNum);
        void storeProcLastLine(const Procedure, const LineNum);
        void storeCFGEdge(const PrevLine lineBefore, const NextLine lineAfter);
        void storeCFGEdgeProc(const PrevLine lineBefore, const NextLine lineAfter);

        //store entities API
        void storeEntity(Declaration::DesignEntity entity, const std::string value);
        void storeEntity(Declaration::DesignEntity entity, const std::string first, const std::string second);

        //store relations API
        void storeRelations(Relation::Types type, const std::string first, const std::string second);
        
        //store patterns API
        void storeAssignPattern(const Variable, const LineNum, const ExprStr);

        //get CFG
        std::unordered_map<PrevLine, std::unordered_set<NextLine>> getCFG();

        //get entities API
        std::unordered_set<std::string> getEntitySet(Declaration::DesignEntity);

        //get relations API
        bool relationContainsSet(Relation::Types, const std::string, const std::string);
        bool relationContainsFirst(Relation::Types, const std::string);
        bool relationContainsSecond(Relation::Types, const std::string);
        bool relationIsEmpty(Relation::Types);
        std::unordered_set<std::string> getRelationSecondFromFirst(Relation::Types, const std::string);
        std::unordered_set<std::string> getRelationFirstFromSecond(Relation::Types, const std::string);
        std::unordered_set<std::string> getRelationAllSecond(Relation::Types);
        std::unordered_set<std::string> getRelationAllFirst(Relation::Types);
        std::unordered_set<std::pair<std::string, std::string>, pairHash> getRelationSet(Relation::Types);

        //get patterns API
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
