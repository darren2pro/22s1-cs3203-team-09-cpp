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
        std::unordered_map<LineNum, std::shared_ptr<TNode>> lineToNodeMap;
        std::unordered_map<std::shared_ptr<TNode>, LineNum> nodeToLineMap;
        std::unordered_map<LineNum, Procedure> lineToProcMap;

        LineNum getCurrLineNumber();
        void incrementCurrLineNumber();

    public:
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

        //Modifies
        RelationsSetBiMap<LineNum, Variable> modifiesSRelations;
        RelationsSetBiMap<Procedure, Variable> modifiesPRelations;

        //Uses
        RelationsSetBiMap<LineNum, Variable> usesSRelations;
        RelationsSetBiMap<Procedure, Variable> usesPRelations;

        //Follows
        RelationsSetBiMap<PrevLine, NextLine> followsRelations;
        RelationsSetBiMap<PrevLine, NextLine> followsTRelations;

        //Parent
        RelationsSetBiMap<ParentLine, ChildLine> parentRelations;
        RelationsSetBiMap<ParentLine, ChildLine> parentTRelations;

        //Calls
        RelationsSetBiMap<LineNum, Procedure> callRelations;
        RelationsSetBiMap<CallerProc, CalleeProc> callsRelations;
        RelationsSetBiMap<CallerProc, CalleeProc> callsTRelations;

        //pattern map
        std::unordered_set<std::pair<LineNum, Variable>, pairHash> assignLineVarSet;
        std::unordered_map<ExprStr, std::unordered_set<std::pair<LineNum, Variable>, pairHash>> assignExprToLineVarMap;
        std::unordered_map<Variable, std::unordered_set<std::pair<LineNum, ExprStr>, pairHash>> assignVarToLineExprMap;

        //CFG
        std::unordered_map<Procedure, LineNum> procFirstLineMap;
        std::unordered_map<Procedure, std::unordered_set<LineNum>> procLastLineMap;

        PKBStorage();
        ~PKBStorage();

        //line number API
        LineNum storeLine(const Stmt node);
        LineNum getLineFromNode(const Stmt node);
        void storeLineToProcedure(LineNum lineNum, Procedure proc);
        Procedure getProcedureFromLine(LineNum lineNum);
        std::shared_ptr<TNode> getNodeFromLine(const LineNum line);
        void storeStmt(const LineNum lineNum);
        void storeCFGEdge(const PrevLine lineBefore, const NextLine lineAfter);
        void storeCFGEdgeProc(const PrevLine lineBefore, const NextLine lineAfter);

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
        void storeRelations(RelationsSetBiMap<std::string, std::string>& relations, const std::string first, const std::string second);
        void storeAssignPattern(const Variable, const LineNum, const ExprStr);
        void storeProcFirstLine(const Procedure, const LineNum);
        void storeProcLastLine(const Procedure, const LineNum);

        std::unordered_map<PrevLine, std::unordered_set<NextLine>> getCFG();

        //get entity set
        std::unordered_set<std::string> getEntitySet(Declaration::DesignEntity);
        std::unordered_set<Variable> getVariableSet();
        std::unordered_set<Procedure> getProcedureSet();
        std::unordered_set<Constant> getConstantSet();
        std::unordered_set<LineNum> getWhileSet();
        std::unordered_set<LineNum> getIfSet();
        std::unordered_set<LineNum> getAssignSet();
        std::unordered_set<LineNum> getReadSet();
        std::unordered_set<LineNum> getPrintSet();
        std::unordered_set<LineNum> getStmtSet();
        std::unordered_set<LineNum> getCallSet();

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
