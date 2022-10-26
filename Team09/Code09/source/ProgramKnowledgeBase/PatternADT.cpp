#include "PatternADT.h"

namespace PKB {
    void PatternADT::add(const Variable var, const LineNum line, const ExprStr expr) {
        lineVarSet.insert(std::pair<LineNum, Variable>(line, var));
        PKB::addToSetInMap(exprToLineVarMap, expr, std::pair<LineNum, Variable>(line, var));
        PKB::addToSetInMap(varToLineExprMap, var, std::pair<LineNum, ExprStr>(line, expr));
    }
    std::unordered_set<LineNum> PatternADT::geLineByVar(const Variable var) {
        std::unordered_set<LineNum> set;
        if (varToLineExprMap.find(var) != varToLineExprMap.end()) {
            for (const auto& elem : varToLineExprMap.at(var)) {
                set.insert(elem.first);
            }
        }
        return set;
    }
    std::unordered_set<LineNum> PatternADT::getLineByVarMatchFull(const Variable var, const ExprStr expr) {
        std::unordered_set<LineNum> set;

        Expr exprNode = SimpleInterface::parseExpression(expr);
        ExprStr patternStr = std::visit([](const auto& node) { return node->toString(); }, exprNode);

        if (exprToLineVarMap.find(patternStr) != exprToLineVarMap.end()) {
            for (const auto& elem : exprToLineVarMap.at(patternStr)) {
                if (elem.second == var) {
                    set.insert(elem.first);
                }
            }
        }
        return set;
    }
    std::unordered_set<LineNum> PatternADT::getLineByVarMatchPartial(const Variable var, const ExprStr expr) {
        std::unordered_set<LineNum> set;

        Expr exprNode = SimpleInterface::parseExpression(expr);
        ExprStr patternStr = std::visit([](const auto& node) { return node->toString(); }, exprNode);

        if (varToLineExprMap.find(var) != varToLineExprMap.end()) {
            for (const auto& elem : varToLineExprMap.at(var)) {
                LineNum line = elem.first;
                ExprStr exprStr = elem.second;
                if (exprStr.find(patternStr) != std::string::npos) {
                    set.insert(line);
                }
            }
        }
        return set;
    }
    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PatternADT::getLineVarSet() {
        return lineVarSet;
    }
    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PatternADT::getLineVarByMatchFull(const ExprStr expr) {
        Expr exprNode = SimpleInterface::parseExpression(expr);
        ExprStr patternStr = std::visit([](const auto& node) { return node->toString(); }, exprNode);

        if (exprToLineVarMap.find(patternStr) != exprToLineVarMap.end()) {
            return exprToLineVarMap.at(patternStr);
        }
        else {
            return std::unordered_set<std::pair<LineNum, Variable>, pairHash>();
        }
    }
    std::unordered_set<std::pair<LineNum, Variable>, pairHash> PatternADT::getLineVarByMatchPartial(const ExprStr expr) {
        std::unordered_set<std::pair<LineNum, Variable>, pairHash> set;

        Expr exprNode = SimpleInterface::parseExpression(expr);
        ExprStr patternStr = std::visit([](const auto& node) { return node->toString(); }, exprNode);

        for (const auto& elem : exprToLineVarMap) {
            ExprStr exprStr = elem.first;
            if (exprStr.find(patternStr) != std::string::npos) {
                for (const auto& ele : elem.second) {
                    set.insert(ele);
                }
            }
        }
        return set;
    }
    std::unordered_set<LineNum> PatternADT::getLineByUS() {
        std::unordered_set<LineNum> set;
        for (const auto& elem : lineVarSet) {
            set.insert(elem.first);
        }
        return set;
    }
    std::unordered_set<LineNum> PatternADT::getLineByUSMatchFull(const ExprStr expr) {
        std::unordered_set<LineNum> set;

        Expr exprNode = SimpleInterface::parseExpression(expr);
        ExprStr patternStr = std::visit([](const auto& node) { return node->toString(); }, exprNode);

        if (exprToLineVarMap.find(patternStr) != exprToLineVarMap.end()) {
            for (const auto& elem : exprToLineVarMap.at(patternStr)) {
                set.insert(elem.first);
            }
        }
        return set;
    }
    std::unordered_set<LineNum> PatternADT::getLineByUSMatchPartial(const ExprStr expr) {
        std::unordered_set<LineNum> set;

        Expr exprNode = SimpleInterface::parseExpression(expr);
        ExprStr patternStr = std::visit([](const auto& node) { return node->toString(); }, exprNode);

        for (const auto& elem : exprToLineVarMap) {
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