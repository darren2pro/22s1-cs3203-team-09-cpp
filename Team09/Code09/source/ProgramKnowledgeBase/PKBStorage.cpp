#include "PKBStorage.h"

namespace PKB {
    LineNum PKBStorage::getCurrLineNumber() {
        return std::to_string(lineNum);
    }

    void PKBStorage::incrementCurrLineNumber() {
        lineNum += 1;
    }

    PKBStorage::PKBStorage() {}

    PKBStorage::~PKBStorage() {}

    LineNum PKBStorage::storeLine(const Stmt node) {
        const LineNum currLineNum = getCurrLineNumber();
        storeStmt(currLineNum);
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

    void PKBStorage::storeLineToProcedure(LineNum lineNum, Procedure proc) {
        lineToProcMap[lineNum] = proc;
    }

    Procedure PKBStorage::getProcedureFromLine(LineNum lineNum) {
        return lineToProcMap.at(lineNum);
    }

    std::shared_ptr<TNode> PKBStorage::getNodeFromLine(const LineNum line) {
        if (lineToNodeMap.find(line) != lineToNodeMap.end()) {
            return std::shared_ptr<TNode>(lineToNodeMap.at(line));
        }
    }

    void PKBStorage::storeStmt(const LineNum lineNum) {
        stmtSet.insert(lineNum);
    }

    void PKBStorage::storeCFGEdge(const PrevLine lineBefore, const NextLine lineAfter) {
        PKB::addToSetInMap(cfgPrevLineToNextLineMap, lineBefore, lineAfter);
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
    
    //todo (figure out what to  do with var)
    void PKBStorage::storeRead(const LineNum lineNum, Variable var) {
        readSet.insert(lineNum);
    }

    //todo (figure out what to  do with var)
    void PKBStorage::storePrint(const LineNum lineNum, Variable var) {
        printSet.insert(lineNum);
    }

    //todo (figure out what to  do with proc)
    void PKBStorage::storeCall(const LineNum lineNum, Procedure proc) {
        callSet.insert(lineNum);
        callRelations.add(lineNum, proc);
        //std::pair<LineNum, Procedure> pair = std::make_pair(lineNum, proc);
        //callLineProcSet.insert(pair);
        //PKB::addToSetInMap(callLineToProcMap, lineNum, proc);
        //PKB::addToSetInMap(callProcToLineMap, proc, lineNum);
    }

    //relations
    void PKBStorage::storeModifiesS(const LineNum lineNum, const Variable var) {
        modifiesSRelations.add(lineNum, var);
        //std::pair<LineNum, Variable> pair = std::make_pair(lineNum, var);
        //modifiesSSet.insert(pair);
        //PKB::addToSetInMap(modifiesSLineToVarMap, lineNum, var);
        //PKB::addToSetInMap(modifiesSVarToLineMap, var, lineNum);
    }

    void PKBStorage::storeModifiesP(const Procedure proc, const Variable var) {
        modifiesPRelations.add(proc, var);
        //std::pair<Procedure, Variable> pair = std::make_pair(proc, var);
        //modifiesPSet.insert(pair);
        //PKB::addToSetInMap(modifiesPProcToVarMap, proc, var);
        //PKB::addToSetInMap(modifiesPVarToProcMap, var, proc);
    }

    void PKBStorage::storeUsesS(const LineNum lineNum, const Variable var) {
        usesSRelations.add(lineNum, var);
        //std::pair<LineNum, Variable> pair = std::make_pair(lineNum, var);
        //usesSSet.insert(pair);
        //PKB::addToSetInMap(usesSLineToVarMap, lineNum, var);
        //PKB::addToSetInMap(usesSVarToLineMap, var, lineNum);
    }

    void PKBStorage::storeUsesP(const Procedure proc, const Variable var) {
        usesPRelations.add(proc, var);
        //std::pair<Procedure, Variable> pair = std::make_pair(proc, var);
        //usesPSet.insert(pair);
        //PKB::addToSetInMap(usesPProcToVarMap, proc, var);
        //PKB::addToSetInMap(usesPVarToProcMap, var, proc);
    }

    void PKBStorage::storeFollows(const PrevLine prev, const NextLine next) {
        followsRelations.add(prev, next);
        //std::pair<PrevLine, NextLine> pair = std::make_pair(prev, next);
        //followsSet.insert(pair);
        //PKB::addToSetInMap(followsPrevToNextMap, prev, next);
        //PKB::addToSetInMap(followsNextToPrevMap, next, prev);
    }

    void PKBStorage::storeFollowsT(const PrevLine prev, const NextLine next) {
        followsTRelations.add(prev, next);
        //std::pair<PrevLine, NextLine> pair = std::make_pair(prev, next);
        //followsTSet.insert(pair);
        //PKB::addToSetInMap(followsTPrevToNextMap, prev, next);
        //PKB::addToSetInMap(followsTNextToPrevMap, next, prev);
    }

    void PKBStorage::storeParent(const ParentLine parent, const ChildLine child) {
        parentRelations.add(parent, child);
        //std::pair<ParentLine, ChildLine> pair = std::make_pair(parent, child);
        //parentSet.insert(pair);
        //PKB::addToSetInMap(parentParentToChildMap, parent, child);
        //PKB::addToSetInMap(parentChildToParentMap, child, parent);
    }

    void PKBStorage::storeParentT(const ParentLine parent, const ChildLine child) {
        parentTRelations.add(parent, child);
        //std::pair<ParentLine, ChildLine> pair = std::make_pair(parent, child);
        //parentTSet.insert(pair);
        //PKB::addToSetInMap(parentTParentToChildMap, parent, child);
        //PKB::addToSetInMap(parentTChildToParentMap, child, parent);
    }

    void PKBStorage::storeCalls(const CallerProc caller, const CalleeProc callee) {
        callsRelations.add(caller, callee);
        //std::pair<CallerProc, CalleeProc> pair = std::make_pair(caller, callee);
        //callsSet.insert(pair);
        //PKB::addToSetInMap(callsCallerToCalleeMap, caller, callee);
        //PKB::addToSetInMap(callsCalleeToCallerMap, callee, caller);
    }

    void PKBStorage::storeCallsT(const CallerProc caller, const CalleeProc callee) {
        callsTRelations.add(caller, callee);
        //std::pair<CallerProc, CalleeProc> pair = std::make_pair(caller, callee);
        //callsTSet.insert(pair);
        //PKB::addToSetInMap(callsTCallerToCalleeMap, caller, callee);
        //PKB::addToSetInMap(callsTCalleeToCallerMap, callee, caller);
    }

    void PKBStorage::storeAssignPattern(const Variable var, const LineNum line, const ExprStr expr) {
        assignLineVarSet.insert(std::pair<LineNum, Variable>(line, var));
        PKB::addToSetInMap(assignExprToLineVarMap, expr, std::pair<LineNum, Variable>(line, var));
        PKB::addToSetInMap(assignVarToLineExprMap, var, std::pair<LineNum, ExprStr>(line, expr));
    }
}


