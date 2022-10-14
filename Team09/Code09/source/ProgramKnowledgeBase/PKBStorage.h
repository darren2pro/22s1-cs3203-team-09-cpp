#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "PKBUtils.h"
#include "RelationsSetBiMap.h"
#include "../SourceProcessor/SimpleInterface.h"
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
