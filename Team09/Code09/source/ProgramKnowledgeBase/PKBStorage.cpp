#include "PKBStorage.h"

namespace PKB {
    LineNum PKBStorage::getCurrLineNumber() {
        return std::to_string(lineNum);
    }

    void PKBStorage::incrementCurrLineNumber() {
        lineNum += 1;
    }

    RelationsSetBiMap<std::string, std::string>* PKBStorage::getRelationFromEnum(Relation::Types type) {
        switch (type) {
        case Relation::ModifiesS:
            return &modifiesSRelations;
        case Relation::ModifiesP:
            return &modifiesPRelations;
        case Relation::UsesS:
            return &usesSRelations;
        case Relation::UsesP:
            return &usesPRelations;
        case Relation::Follows:
            return &followsRelations;
        case Relation::FollowsT:
            return &followsTRelations;
        case Relation::Parent:
            return &parentRelations;
        case Relation::ParentT:
            return &parentTRelations;
        case Relation::Calls:
            return &callsRelations;
        case Relation::CallsT:
            return &callsTRelations;
        default:
            return &RelationsSetBiMap<std::string, std::string>();
        }
    }

    std::unordered_set<std::string>* PKBStorage::getEntityFromEnum(Declaration::DesignEntity entity) {
        switch (entity) {
        case Declaration::Variable:
            return &varSet;
        case Declaration::Procedure:
            return &procSet;
        case Declaration::Constant:
            return &constSet;
        case Declaration::While:
            return &whileSet;
        case Declaration::If:
            return &ifSet;
        case Declaration::Assignment:
            return &assignSet;
        case Declaration::Read:
            return &readSet;
        case Declaration::Print:
            return &printSet;
        case Declaration::Statement:
            return &stmtSet;
        case Declaration::Call:
            return &callSet;
        default:
            return &std::unordered_set<std::string>();
        }
    }

    PKBStorage::PKBStorage() {}

    PKBStorage::~PKBStorage() {}

    void PKBStorage::storeLineCallsProc(LineNum lineNum, Procedure proc) {
        std::pair<LineNum, Procedure> pair = std::make_pair(lineNum, proc);
        lineCallsProcSet.insert(pair);
    }

    LineNum PKBStorage::storeLine(const Stmt node) {
        const LineNum currLineNum = getCurrLineNumber();
        storeEntity(Declaration::Statement, currLineNum);
        incrementCurrLineNumber();
        std::visit(
                [this, currLineNum](const auto& s) {
                    //lineToNodeMap[currLineNum] = s;
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

    //std::shared_ptr<TNode> PKBStorage::getNodeFromLine(const LineNum line) {
    //    if (lineToNodeMap.find(line) != lineToNodeMap.end()) {
    //        return std::shared_ptr<TNode>(lineToNodeMap.at(line));
    //    }
    //}

    void PKBStorage::storeProcFirstLine(const Procedure proc, const LineNum firstLine) {
        procFirstLineMap[proc] = firstLine; //can only have 1 first line for each proc
    }

    void PKBStorage::storeProcLastLine(const Procedure proc, const LineNum lastLine) {
        PKB::addToSetInMap(procLastLineMap, proc, lastLine);
    }

    void PKBStorage::storeCFGEdge(const PrevLine lineBefore, const NextLine lineAfter) {
        PKB::addToSetInMap(cfgPrevLineToNextLineMap, lineBefore, lineAfter);
    }

    void PKBStorage::storeCFGEdgeProc(const PrevLine lineBefore, const NextLine lineAfter) {
        PKB::addToSetInMap(cfgProcPrevLineToNextLineMap, lineBefore, lineAfter);
    }

    void PKBStorage::storeEntity(Declaration::DesignEntity entity, const std::string value) {
        auto entitySet = getEntityFromEnum(entity);
        entitySet->insert(value);
    }

    //do sth to second (for with clauses)
    void PKBStorage::storeEntity(Declaration::DesignEntity entity, const std::string first, const std::string second) {
        auto entitySet = getEntityFromEnum(entity);
        entitySet->insert(first);
    }

    void PKBStorage::storeRelations(Relation::Types type, std::string first, std::string second) {
        auto relation = getRelationFromEnum(type);
        relation->add(first, second);
    }

    void PKBStorage::storeAssignPattern(const Variable var, const LineNum line, const ExprStr expr) {
        assignLineVarSet.insert(std::pair<LineNum, Variable>(line, var));
        PKB::addToSetInMap(assignExprToLineVarMap, expr, std::pair<LineNum, Variable>(line, var));
        PKB::addToSetInMap(assignVarToLineExprMap, var, std::pair<LineNum, ExprStr>(line, expr));
    }

    std::unordered_map<PrevLine, std::unordered_set<NextLine>> PKBStorage::getCFG() {
        return cfgPrevLineToNextLineMap;
    }

    std::unordered_set<std::string> PKBStorage::getEntitySet(Declaration::DesignEntity entity) {
        return *getEntityFromEnum(entity);
    }

    bool PKBStorage::relationContainsSet(Relation::Types type, const std::string first, const std::string second) {
        auto relation = getRelationFromEnum(type);
        return relation->containsSet(first, second);
    }

<<<<<<< HEAD
    bool PKBStorage::relationContainsFirst(Relation::Types type, const std::string first) {
        auto relation = getRelationFromEnum(type);
        return relation->containsFirst(first);
    }

    bool PKBStorage::relationContainsSecond(Relation::Types type, const std::string second) {
        auto relation = getRelationFromEnum(type);
        return relation->containsSecond(second);
    }

    bool PKBStorage::relationIsEmpty(Relation::Types type) {
        auto relation = getRelationFromEnum(type);
        return relation->isEmpty();
    }

    std::unordered_set<std::string> PKBStorage::getRelationSecondFromFirst(Relation::Types type, const std::string first) {
        auto relation = getRelationFromEnum(type);
        return relation->getSecondFromFirst(first);
    }

    std::unordered_set<std::string> PKBStorage::getRelationFirstFromSecond(Relation::Types type, const std::string second) {
        auto relation = getRelationFromEnum(type);
        return relation->getFirstFromSecond(second);
    }

    std::unordered_set<std::string> PKBStorage::getRelationAllSecond(Relation::Types type) {
        auto relation = getRelationFromEnum(type);
        return relation->getAllSecond();
    }

    std::unordered_set<std::string> PKBStorage::getRelationAllFirst(Relation::Types type) {
        auto relation = getRelationFromEnum(type);
        return relation->getAllFirst();
    }

    std::unordered_set<std::pair<std::string, std::string>, pairHash> PKBStorage::getRelationSet(Relation::Types type) {
        auto relation = getRelationFromEnum(type);
        return relation->getSet();
    }

    std::unordered_set<LineNum> PKBStorage::getAssignLineByVarUS(const Variable var) {
        std::unordered_set<LineNum> set;
        if (assignVarToLineExprMap.find(var) != assignVarToLineExprMap.end()) {
            for (const auto& elem : assignVarToLineExprMap.at(var)) {
                set.insert(elem.first);
            }
        }
        return set;
    }

    std::unordered_set<LineNum> PKBStorage::getAssignLineByVarMatchFull(const Variable var, const  ExprStr expr) {
        std::unordered_set<LineNum> set;
        std::string pattern = expr;
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '_'), pattern.end());
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '\"'), pattern.end());

        Expr exprNode = SimpleInterface::parseExpression(pattern);
        ExprStr patternStr = std::visit([](const auto& node) { return node->toString(); }, exprNode);

        if (assignExprToLineVarMap.find(patternStr) != assignExprToLineVarMap.end()) {
            for (const auto& elem : assignExprToLineVarMap.at(patternStr)) {
                if (elem.second == var) {
                    set.insert(elem.first);
                }
            }
        }
        return set;
    }

    std::unordered_set<LineNum> PKBStorage::getAssignLineByVarMatchPartial(const Variable var, const  ExprStr expr) {
        //get pattern string
        std::unordered_set<LineNum> set;
        std::string pattern = expr;
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '_'), pattern.end());
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '\"'), pattern.end());
        Expr exprNode = SimpleInterface::parseExpression(pattern);
        ExprStr patternStr = std::visit([](const auto& node) { return node->toString(); }, exprNode);

        if (assignVarToLineExprMap.find(var) != assignVarToLineExprMap.end()) {
            for (const auto& elem : assignVarToLineExprMap.at(var)) {
                LineNum line = elem.first;
                ExprStr exprStr = elem.second;
                if (exprStr.find(patternStr) != std::string::npos) {
                    set.insert(line);
                }
            }
        }
        return set;
    }

    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PKBStorage::getAssignLineVarByUS() {
        return assignLineVarSet;
=======
    //todo (figure out what to  do with proc)
    void PKBStorage::storeCall(const LineNum lineNum, Procedure proc) {
        callSet.insert(lineNum);
        std::pair<LineNum, Procedure> pair = std::make_pair(lineNum, proc);
        callLineProcSet.insert(pair);
        PKB::addToSetInMap(callLineToProcMap, lineNum, proc);
        PKB::addToSetInMap(callProcToLineMap, proc, lineNum);
    }

    //relations
    void PKBStorage::storeModifiesS(const LineNum lineNum, const Variable var) {
        std::pair<LineNum, Variable> pair = std::make_pair(lineNum, var);
        modifiesSSet.insert(pair);
        PKB::addToSetInMap(modifiesSLineToVarMap, lineNum, var);
        PKB::addToSetInMap(modifiesSVarToLineMap, var, lineNum);
    }

    void PKBStorage::storeModifiesP(const Procedure proc, const Variable var) {
        std::pair<Procedure, Variable> pair = std::make_pair(proc, var);
        modifiesPSet.insert(pair);
        PKB::addToSetInMap(modifiesPProcToVarMap, proc, var);
        PKB::addToSetInMap(modifiesPVarToProcMap, var, proc);
    }

    void PKBStorage::storeUsesS(const LineNum lineNum, const Variable var) {
        std::pair<LineNum, Variable> pair = std::make_pair(lineNum, var);
        usesSSet.insert(pair);
        PKB::addToSetInMap(usesSLineToVarMap, lineNum, var);
        PKB::addToSetInMap(usesSVarToLineMap, var, lineNum);
    }

    void PKBStorage::storeUsesP(const Procedure proc, const Variable var) {
        std::pair<Procedure, Variable> pair = std::make_pair(proc, var);
        usesPSet.insert(pair);
        PKB::addToSetInMap(usesPProcToVarMap, proc, var);
        PKB::addToSetInMap(usesPVarToProcMap, var, proc);
    }

    void PKBStorage::storeFollows(const PrevLine prev, const NextLine next) {
        std::pair<PrevLine, NextLine> pair = std::make_pair(prev, next);
        followsSet.insert(pair);
        PKB::addToSetInMap(followsPrevToNextMap, prev, next);
        PKB::addToSetInMap(followsNextToPrevMap, next, prev);
    }

    void PKBStorage::storeFollowsT(const PrevLine prev, const NextLine next) {
        std::pair<PrevLine, NextLine> pair = std::make_pair(prev, next);
        followsTSet.insert(pair);
        PKB::addToSetInMap(followsTPrevToNextMap, prev, next);
        PKB::addToSetInMap(followsTNextToPrevMap, next, prev);
    }

    void PKBStorage::storeParent(const ParentLine parent, const ChildLine child) {
        std::pair<ParentLine, ChildLine> pair = std::make_pair(parent, child);
        parentSet.insert(pair);
        PKB::addToSetInMap(parentParentToChildMap, parent, child);
        PKB::addToSetInMap(parentChildToParentMap, child, parent);
    }

    void PKBStorage::storeParentT(const ParentLine parent, const ChildLine child) {
        std::pair<ParentLine, ChildLine> pair = std::make_pair(parent, child);
        parentTSet.insert(pair);
        PKB::addToSetInMap(parentTParentToChildMap, parent, child);
        PKB::addToSetInMap(parentTChildToParentMap, child, parent);
    }

    void PKBStorage::storeCalls(const CallerProc caller, const CalleeProc callee) {
        std::pair<CallerProc, CalleeProc> pair = std::make_pair(caller, callee);
        callsSet.insert(pair);
        PKB::addToSetInMap(callsCallerToCalleeMap, caller, callee);
        PKB::addToSetInMap(callsCalleeToCallerMap, callee, caller);
    }

    void PKBStorage::storeCallsT(const CallerProc caller, const CalleeProc callee) {
        std::pair<CallerProc, CalleeProc> pair = std::make_pair(caller, callee);
        callsTSet.insert(pair);
        PKB::addToSetInMap(callsTCallerToCalleeMap, caller, callee);
        PKB::addToSetInMap(callsTCalleeToCallerMap, callee, caller);
>>>>>>> 8f881df334ef0302d1b449e44a95822b4b1ed8f6
    }

    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PKBStorage::getAssignLineVarByMatchFull(const ExprStr expr) {
        std::string pattern = expr;
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '_'), pattern.end());
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '\"'), pattern.end());

        Expr exprNode = SimpleInterface::parseExpression(pattern);
        ExprStr patternStr = std::visit([](const auto& node) { return node->toString(); }, exprNode);

        if (assignExprToLineVarMap.find(patternStr) != assignExprToLineVarMap.end()) {
            return assignExprToLineVarMap.at(patternStr);
        }
        else {
            return std::unordered_set<std::pair<LineNum, Variable>, pairHash>();
        }
    }

    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PKBStorage::getAssignLineVarByMatchPartial(const ExprStr expr) {
        //get pattern string
        std::unordered_set<std::pair<LineNum, Variable>, pairHash> set;
        std::string pattern = expr;
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '_'), pattern.end());
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '\"'), pattern.end());
        Expr exprNode = SimpleInterface::parseExpression(pattern);
        ExprStr patternStr = std::visit([](const auto& node) { return node->toString(); }, exprNode);

        for (const auto& elem : assignExprToLineVarMap) {
            ExprStr exprStr = elem.first;
            if (exprStr.find(patternStr) != std::string::npos) {
                for (const auto& ele : elem.second) {
                    set.insert(ele);
                }
            }
        }
        return set;
    }

    std::unordered_set<LineNum> PKBStorage::getAssignLineByUSUS() {
        return assignSet;
    }

    std::unordered_set<LineNum> PKBStorage::getAssignLineByUSMatchFull(const ExprStr expr) {
        std::unordered_set<LineNum> set;
        std::string pattern = expr;
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '_'), pattern.end());
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '\"'), pattern.end());

        Expr exprNode = SimpleInterface::parseExpression(pattern);
        ExprStr patternStr = std::visit([](const auto& node) { return node->toString(); }, exprNode);

        if (assignExprToLineVarMap.find(patternStr) != assignExprToLineVarMap.end()) {
            for (const auto& elem : assignExprToLineVarMap.at(patternStr)) {
                set.insert(elem.first);
            }
        }
        return set;
    }

    std::unordered_set<LineNum> PKBStorage::getAssignLineByUSMatchPartial(const ExprStr expr) {
        //get pattern string
        std::unordered_set<LineNum> set;
        std::string pattern = expr;
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '_'), pattern.end());
        pattern.erase(std::remove(pattern.begin(), pattern.end(), '\"'), pattern.end());

        Expr exprNode = SimpleInterface::parseExpression(pattern);
        ExprStr patternStr = std::visit([](const auto& node) { return node->toString(); }, exprNode);

        for (const auto& elem : assignExprToLineVarMap) {
            ExprStr exprStr = elem.first;
            if (exprStr.find(patternStr) != std::string::npos) {
                for (const auto& ele : elem.second) {
                    set.insert(ele.first);
                }
            }
        }
        return set;
    }
}
