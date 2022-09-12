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

    //relations map
    std::unordered_map<LineNum, LineNum> followsMap;
    std::unordered_map<LineNum, LineNum> parentMap;
    std::unordered_map<LineNum, std::unordered_set<Variable>> usesSMap;
    std::unordered_map<LineNum, std::unordered_set<Variable>> modifiesMap;

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
};

}
