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

    LineNum PKBStorage::getLineFromNode(const Stmt node) {
        return std::visit(
            [this](const auto& s) {
                if (nodeToLineMap.find(s) != nodeToLineMap.end()) {
                    return nodeToLineMap.at(s);
                }
            },
            node);
    }

    std::shared_ptr<TNode> PKBStorage::getNodeFromLine(const LineNum line) {
        if (lineToNodeMap.find(line) != lineToNodeMap.end()) {
            return std::shared_ptr<TNode>(lineToNodeMap.at(line));
        }
    }

    void PKBStorage::storeVariable(const Variable var) {
        varSet.insert(var);
    }

    void PKBStorage::storeProcedure(const Procedure proc) {
        procSet.insert(proc);
    }

    void PKBStorage::storeConstant(const Constant constant) {
        constSet.insert(constant);
    }

    void PKBStorage::storeWhile(const LineNum lineNum) {
        whileSet.insert(lineNum);
    }

    void PKBStorage::storeIf(const LineNum lineNum) {
        ifSet.insert(lineNum);
    }

    void PKBStorage::storeAssign(const LineNum lineNum) {
        assignSet.insert(lineNum);
    }

    void PKBStorage::storeRead(const LineNum lineNum, Variable var) {
        readSet.insert(lineNum);
    }

    void PKBStorage::storePrint(const LineNum lineNum, Variable var) {
        printSet.insert(lineNum);
    }

}