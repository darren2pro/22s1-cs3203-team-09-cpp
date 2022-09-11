#pragma once

#include <string>
#include <unordered_map>
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
    PKBStorage();
    ~PKBStorage();
    LineNum storeLine(const Stmt node);
};

}
