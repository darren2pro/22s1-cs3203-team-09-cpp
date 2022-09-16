#include "PKBStorage.h"

PKBStorage::LineNum PKBStorage::getCurrLineNumber() {
    return std::to_string(lineNum);
}

void PKBStorage::incrementCurrLineNumber() {
    lineNum += 1;
}

PKBStorage::PKBStorage() {}

PKBStorage::~PKBStorage() {}

PKBStorage::LineNum PKBStorage::storeLine(const Stmt node) {
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

PKBStorage::LineNum PKBStorage::getLineFromNode(const Stmt node) {
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

//entities
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

//relations
void PKBStorage::storeModifiesS(const LineNum lineNum, const Variable var) {
    std::pair<LineNum, Variable> pair = std::make_pair(lineNum, var);
    modifiesSet.insert(pair);
    addToSetInMap(modifiesLineToVarMap, lineNum, var);
    addToSetInMap(modifiesVarToLineMap, var, lineNum);
}

void PKBStorage::storeUsesS(const LineNum lineNum, const Variable var) {
    std::pair<LineNum, Variable> pair = std::make_pair(lineNum, var);
    usesSet.insert(pair);
    addToSetInMap(usesLineToVarMap, lineNum, var);
    addToSetInMap(usesVarToLineMap, var, lineNum);
}

void PKBStorage::storeFollows(const PrevLine prev, const NextLine next) {
    std::pair<PrevLine, NextLine> pair = std::make_pair(prev, next);
    followsSet.insert(pair);
    addToSetInMap(followsPrevToNextMap, prev, next);
    addToSetInMap(followsNextToPrevMap, next, prev);
}


void PKBStorage::storeParent(const ParentLine parent, const ChildLine child) {
    std::pair<ParentLine, ChildLine> pair = std::make_pair(parent, child);
    parentSet.insert(pair);
    addToSetInMap(parentParentToChildMap, parent, child);
    addToSetInMap(parentChildToParentMap, child, parent);
}

void PKBStorage::storeAssignPattern(const Variable var, const LineNum line, const ExprStr expr) {
    assignLineVarSet.insert(std::pair<LineNum, Variable>(line, var));
    addToSetInMap(assignExprToLineVarMap, expr, std::pair<LineNum, Variable>(line, var));
    addToSetInMap(assignVarToLineExprMap, var, std::pair<LineNum, ExprStr>(line, expr));
}
