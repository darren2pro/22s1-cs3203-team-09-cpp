#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "PKBUtils.h"
#include "PatternsSetBiMap.h"
#include "RelationsSetBiMap.h"
#include "../TNode/TNode.h"
#include "../QueryProcessingSubsystem/Declaration.h"
#include "../QueryProcessingSubsystem/Relation.h"
#include "../QueryProcessingSubsystem/Pattern.h"


namespace PKB {
    class PKBStorage {
    private:
        int lineNum = 1;
        std::unordered_map<std::shared_ptr<TNode>, LineNum> nodeToLineMap;
        std::unordered_map<LineNum, Procedure> lineToProcMap;

        LineNum getCurrLineNumber();
        void incrementCurrLineNumber();

        RelationsSetBiMap<std::string, std::string>* getRelationFromEnum(Relation::Types);
        std::unordered_set<std::string>* getEntityFromEnum(Declaration::DesignEntity);
        PatternsSetBiMap* getPatternFromEnum(Pattern::Types);

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
        PatternsSetBiMap assignPattern;
        PatternsSetBiMap ifPattern;
        PatternsSetBiMap whilePattern;

    public:

        PKBStorage();
        ~PKBStorage();

        void storeLineCallsProc(LineNum lineNum, Procedure proc);
        std::unordered_set < std::pair<LineNum, Procedure>, pairHash> getLineCallsProc();

        //line number API
        LineNum storeLine(const Stmt node);
        LineNum getLineFromNode(const Stmt node);
        void storeLineToProcedure(const LineNum lineNum, const Procedure proc);
        Procedure getProcedureFromLine(const LineNum lineNum);

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
        void storePatterns(Pattern::Types type, const Variable, const LineNum, const ExprStr);

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
        std::unordered_set<LineNum> getPatternLineByVar(Pattern::Types, const Variable);
        std::unordered_set<LineNum> getPatternLineByVarMatchFull(Pattern::Types, const Variable, const ExprStr);
        std::unordered_set<LineNum> getPatternLineByVarMatchPartial(Pattern::Types, const Variable, const ExprStr);
        std::unordered_set<std::pair<LineNum, Variable>, pairHash> getPatternLineVarSet(Pattern::Types);
        std::unordered_set<std::pair<LineNum, Variable>, pairHash> getPatternLineVarByMatchFull(Pattern::Types, const ExprStr);
        std::unordered_set<std::pair<LineNum, Variable>, pairHash> getPatternLineVarByMatchPartial(Pattern::Types, const ExprStr);
        std::unordered_set<LineNum> getPatternLineByUS(Pattern::Types);
        std::unordered_set<LineNum> getPatternLineByUSMatchFull(Pattern::Types, const ExprStr);
        std::unordered_set<LineNum> getPatternLineByUSMatchPartial(Pattern::Types, const ExprStr);
    };
}
