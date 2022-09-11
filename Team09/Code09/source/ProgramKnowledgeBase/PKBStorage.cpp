#include <unordered_set>
#include <unordered_map>
#include "PKBStorage.h"

namespace PKB {

    LineNum PKB::PKBStorage::getCurrLineNumber() {
        return std::to_string(lineNum);
    }

    void PKB::PKBStorage::incrementCurrLineNumber() {
        lineNum += 1;
    }

    PKB::PKBStorage::PKBStorage() {}

    PKB::PKBStorage::~PKBStorage() {}

    LineNum PKBStorage::storeLine(const Stmt node) {
        const LineNum currLineNum = getCurrLineNumber();
        incrementCurrLineNumber();
        std::visit(
            [this, currLineNum](const auto& s) {
                lineToNodeMap[currLineNum] = s;
                nodeToLineMap[s] = currLineNum;
            },
            node);

        return currLineNum;
    }

}