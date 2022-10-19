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
        case Relation::Next:
            return &nextRelations;
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

    PatternsSetBiMap* PKBStorage::getPatternFromEnum(Pattern::Types type) {
        switch (type) {
        case Pattern::Assign:
            return &assignPattern;
        case Pattern::If:
            return &ifPattern;
        case Pattern::While:
            return &whilePattern;
        default:
            return &PatternsSetBiMap();
        }
    }

    PKBStorage::PKBStorage() {}

    PKBStorage::~PKBStorage() {}

    void PKBStorage::storeLineCallsProc(LineNum lineNum, Procedure proc) {
        std::pair<LineNum, Procedure> pair = std::make_pair(lineNum, proc);
        lineCallsProcSet.insert(pair);
    }

    std::unordered_set<std::pair<LineNum, Procedure>, pairHash> PKBStorage::getLineCallsProc() {
        return lineCallsProcSet;
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

    void PKBStorage::storePatterns(Pattern::Types type, const Variable var, const LineNum line, const ExprStr expr) {
        auto pattern = getPatternFromEnum(type);
        pattern->add(var, line, expr);
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

    //NextT
    std::unordered_set<PKB::NextLine> PKBStorage::getNextLineT(const PKB::PrevLine prevLine) {
        std::shared_ptr<std::unordered_set<LineNum>> visited =std::make_shared<std::unordered_set<LineNum>>();
        auto toVisit = getRelationSecondFromFirst(Relation::Next, prevLine);
        if (!toVisit.empty()) {
            for (const auto& neighbour : toVisit) {
                getNextLineTH(neighbour, visited);
            }
            if (visited->size() > 0) {
                return std::unordered_set<NextLine>(*visited.get());
            }
        }
        return *visited;
    }

    void PKBStorage::getNextLineTH(const PKB::LineNum currLine, std::shared_ptr<std::unordered_set<LineNum>> visited) {
        if (visited->find(currLine) != visited->end()) {
            return;
        }
        visited->insert(currLine);
        auto toVisit = getRelationSecondFromFirst(Relation::Next, currLine);
        if (!toVisit.empty()) {
            for (const auto& neighbour : toVisit) {
                getNextLineTH(neighbour, visited);
            }
        }
    }

    std::unordered_set<PKB::PrevLine> PKBStorage::getPreviousLineT(const PKB::PrevLine nextLine) {
        std::shared_ptr<std::unordered_set<LineNum>> visited = std::make_shared<std::unordered_set<LineNum>>();
        auto toVisit = getRelationFirstFromSecond(Relation::Next, nextLine);
        if (!toVisit.empty()) {
            for (const auto& neighbour : toVisit) {
                getPreviousLineTH(neighbour, visited);
            }
            if (visited->size() > 0) {
                return std::unordered_set<PrevLine>(*visited.get());
            }
        }
        return *visited;
    }

    void PKBStorage::getPreviousLineTH(const PKB::LineNum currLine, std::shared_ptr<std::unordered_set<LineNum>> visited) {
        if (visited->find(currLine) != visited->end()) {
            return;
        }
        visited->insert(currLine);
        auto toVisit = getRelationFirstFromSecond(Relation::Next, currLine);
        if (!toVisit.empty()) {
            for (const auto& neighbour : toVisit) {
                getPreviousLineTH(neighbour, visited);
            }
        }
    }

    // Pattern functions

    std::unordered_set<LineNum> PKBStorage::getPatternLineByVar(Pattern::Types type, const Variable var) {
        auto pattern = getPatternFromEnum(type);
        return pattern->geLineByVar(var);
    }

    std::unordered_set<LineNum> PKBStorage::getPatternLineByVarMatchFull(Pattern::Types type, const Variable var, const ExprStr expr) {
        auto pattern = getPatternFromEnum(type);
        return pattern->getLineByVarMatchFull(var, expr);
    }

    std::unordered_set<LineNum> PKBStorage::getPatternLineByVarMatchPartial(Pattern::Types type, const Variable var, const ExprStr expr) {
        auto pattern = getPatternFromEnum(type);
        return pattern->getLineByVarMatchPartial(var, expr);
    }

    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PKBStorage::getPatternLineVarSet(Pattern::Types type) {
        auto pattern = getPatternFromEnum(type);
        return pattern->getLineVarSet();
    }

    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PKBStorage::getPatternLineVarByMatchFull(Pattern::Types type, const ExprStr expr) {
        auto pattern = getPatternFromEnum(type);
        return pattern->getLineVarByMatchFull(expr);
    }

    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PKBStorage::getPatternLineVarByMatchPartial(Pattern::Types type, const ExprStr expr) {
        auto pattern = getPatternFromEnum(type);
        return pattern->getLineVarByMatchPartial(expr);
    }

    std::unordered_set<LineNum> PKBStorage::getPatternLineByUS(Pattern::Types type) {
        auto pattern = getPatternFromEnum(type);
        return pattern->getLineByUS();
    }

    std::unordered_set<LineNum> PKBStorage::getPatternLineByUSMatchFull(Pattern::Types type, const ExprStr expr) {
        auto pattern = getPatternFromEnum(type);
        return pattern->getLineByUSMatchFull(expr);
    }

    std::unordered_set<LineNum> PKBStorage::getPatternLineByUSMatchPartial(Pattern::Types type, const ExprStr expr) {
        auto pattern = getPatternFromEnum(type);
        return pattern->getLineByUSMatchPartial(expr);
    }
}
