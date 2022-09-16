#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "pkb_adt.h"
#include "../TNode/TNode.h"

namespace PKB {

class PKBStorage {
private:
    int lineNum = 1;
    std::unordered_map<LineNum, std::shared_ptr<TNode>> lineToNodeMap;
    std::unordered_map<std::shared_ptr<TNode>, LineNum> nodeToLineMap;

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

    //Modifies
    std::unordered_set<std::pair<LineNum, Variable>> modifiesSet;
    std::unordered_map<LineNum, std::unordered_set<Variable>> modifiesLineToVarMap;
    std::unordered_map<Variable, std::unordered_set<LineNum>> modifiesVarToLineMap;

    //Uses
    std::unordered_set<std::pair<LineNum, Variable>> usesSet;
    std::unordered_map<LineNum, std::unordered_set<Variable>> usesLineToVarMap;
    std::unordered_map<Variable, std::unordered_set<LineNum>> usesVarToLineMap;

    //Follows
    std::unordered_set<std::pair<PrevLine, NextLine>> followsSet;
    std::unordered_map<PrevLine, NextLine> followsPrevToNextMap;
    std::unordered_map<NextLine, PrevLine> followsNextToPrevMap;
    std::unordered_set<std::pair<PrevLine, NextLine>> followsTSet;
    std::unordered_map<PrevLine, std::unordered_set<NextLine>> followsTPrevToNextMap;
    std::unordered_map<NextLine, std::unordered_set<PrevLine>> followsTNextToPrevMap;

    //Parent
    std::unordered_set<std::pair<ParentLine, ChildLine>> parentSet;
    std::unordered_map<ParentLine, ChildLine> parentParentToChildMap;
    std::unordered_map<ChildLine, ParentLine> parentChildToParentMap;
    std::unordered_set<std::pair<ParentLine, ChildLine>> parentTSet;
    std::unordered_map<ParentLine, std::unordered_set<ChildLine>> parentTParentToChildMap;
    std::unordered_map<ChildLine, std::unordered_set<ParentLine>> parentTChildToParentMap;

    //pattern map
    std::unordered_map<ExprStr, std::unordered_set<std::pair<LineNum, Variable>>> assignMap;


    PKBStorage();
    ~PKBStorage();

    //line number API
    LineNum storeLine(const Stmt node);
    LineNum getLineFromNode(const Stmt node);
    std::shared_ptr<TNode> getNodeFromLine(const LineNum line);

    //store entities API
    void storeVariable(const Variable var);
    void storeProcedure(const Procedure proc);
    void storeConstant(const Constant constant);
    void storeWhile(const LineNum lineNum);
    void storeIf(const LineNum lineNum);
    void storeAssign(const LineNum lineNum);
    void storeRead(const LineNum lineNum, Variable var);
    void storePrint(const LineNum lineNum, Variable var);

    //store relations API
    void storeFollows(const PrevLine, const NextLine);
    void storeParent(const ParentLine, const ChildLine);
    void storeUsesS(const LineNum, const Variable);
    void storeModifiesS(const LineNum, const Variable);

    //helper to store variable into usesMap and modifiesMap
    static void addToSetInMap(std::unordered_map<std::string, std::unordered_set<std::string>>& map,
        const std::string key, const std::string val);
};

}
