#include "stdafx.h"
#include "CppUnitTest.h"
#include "ProgramKnowledgeBase/PKBStorage.h"
#include "ProgramKnowledgeBase/PKBStorage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting {
    //TEST_CLASS(TestPkb) {
    //        public:
    //        TEST_METHOD(TestPkbStorage) {
    //            PKB::PKBStorage PKBStorage;
    //            shared_ptr<PKB::PKBStorage> pkbStorage = PKBStorage.getPKBStorage();

    //            //Check if statement lines are assigned and stored correctly
    //            PKB::LineNum line = pkbStorage->storeLine(std::make_shared<AssignmentNode>
    //                (std::make_shared<VariableNode>("var1"), std::make_shared<ConstantNode>("1")));
    //            Assert::IsTrue(pkbStorage->stmtSet.find(line) != pkbStorage->stmtSet.end());
    //            Assert::IsTrue(pkbStorage->stmtSet.find("2") == pkbStorage->stmtSet.end());

    //            //Check if CFG is stored correctly
    //            pkbStorage->storeCFGEdge("1", "2");
    //            pkbStorage->storeCFGEdge("1", "3");
    //            std::unordered_set<PKB::LineNum> cfgNextSet;
    //            cfgNextSet.insert("2");
    //            cfgNextSet.insert("3");
    //            Assert::IsTrue(pkbStorage->cfgPrevLineToNextLineMap.find("1") != pkbStorage->cfgPrevLineToNextLineMap.end());
    //            Assert::IsTrue(pkbStorage->cfgPrevLineToNextLineMap.find("2") == pkbStorage->cfgPrevLineToNextLineMap.end());
    //            Assert::IsTrue(pkbStorage->cfgPrevLineToNextLineMap.at("1") == cfgNextSet);

    //            //Check if entity population is correct
    //            pkbStorage->storeVariable("var1");
    //            pkbStorage->storeProcedure("proc1");
    //            pkbStorage->storeConstant("1");
    //            pkbStorage->storeWhile("1");
    //            pkbStorage->storeIf("1");
    //            pkbStorage->storeAssign("1");
    //            pkbStorage->storeRead("1", "var1");
    //            pkbStorage->storePrint("1", "var1");

    //            Assert::IsTrue(pkbStorage->varSet.find("var1") != pkbStorage->varSet.end());
    //            Assert::IsTrue(pkbStorage->varSet.find("var2") == pkbStorage->varSet.end());
    //            Assert::IsTrue(pkbStorage->procSet.find("proc1") != pkbStorage->procSet.end());
    //            Assert::IsTrue(pkbStorage->procSet.find("proc2") == pkbStorage->procSet.end());
    //            Assert::IsTrue(pkbStorage->constSet.find("1") != pkbStorage->constSet.end());
    //            Assert::IsTrue(pkbStorage->constSet.find("2") == pkbStorage->constSet.end());
    //            Assert::IsTrue(pkbStorage->whileSet.find("1") != pkbStorage->whileSet.end());
    //            Assert::IsTrue(pkbStorage->whileSet.find("2") == pkbStorage->whileSet.end());
    //            Assert::IsTrue(pkbStorage->ifSet.find("1") != pkbStorage->ifSet.end());
    //            Assert::IsTrue(pkbStorage->ifSet.find("2") == pkbStorage->ifSet.end());
    //            Assert::IsTrue(pkbStorage->assignSet.find("1") != pkbStorage->assignSet.end());
    //            Assert::IsTrue(pkbStorage->assignSet.find("2") == pkbStorage->assignSet.end());
    //            Assert::IsTrue(pkbStorage->readSet.find("1") != pkbStorage->readSet.end());
    //            Assert::IsTrue(pkbStorage->readSet.find("2") == pkbStorage->readSet.end());
    //            Assert::IsTrue(pkbStorage->printSet.find("1") != pkbStorage->printSet.end());
    //            Assert::IsTrue(pkbStorage->printSet.find("2") == pkbStorage->printSet.end());

    //            //Check if relations population is correct
    //            /*pkbStorage->storeFollows("1", "2");
    //            std::unordered_set<PKB::LineNum> prevSet;
    //            std::unordered_set<PKB::LineNum> nextSet;
    //            prevSet.insert("1");
    //            nextSet.insert("2");
    //            Assert::IsTrue(pkbStorage->followsSet.find(std::make_pair("1", "2")) != pkbStorage->followsSet.end());
    //            Assert::IsTrue(pkbStorage->followsSet.find(std::make_pair("1", "3")) == pkbStorage->followsSet.end());
    //            Assert::IsTrue(pkbStorage->followsPrevToNextMap.find("1") != pkbStorage->followsPrevToNextMap.end());
    //            Assert::IsTrue(pkbStorage->followsPrevToNextMap.find("2") == pkbStorage->followsPrevToNextMap.end());
    //            Assert::IsTrue(pkbStorage->followsNextToPrevMap.find("2") != pkbStorage->followsNextToPrevMap.end());
    //            Assert::IsTrue(pkbStorage->followsNextToPrevMap.find("1") == pkbStorage->followsNextToPrevMap.end());
    //            Assert::IsTrue(pkbStorage->followsPrevToNextMap.at("1") == nextSet);
    //            Assert::IsTrue(pkbStorage->followsNextToPrevMap.at("2") == prevSet);

    //            pkbStorage->storeFollowsT("1", "2");
    //            std::unordered_set<PKB::LineNum> prevTSet;
    //            std::unordered_set<PKB::LineNum> nextTSet;
    //            prevTSet.insert("1");
    //            nextTSet.insert("2");
    //            Assert::IsTrue(pkbStorage->followsTSet.find(std::make_pair("1", "2")) != pkbStorage->followsTSet.end());
    //            Assert::IsTrue(pkbStorage->followsTSet.find(std::make_pair("1", "3")) == pkbStorage->followsTSet.end());
    //            Assert::IsTrue(pkbStorage->followsTPrevToNextMap.find("1") != pkbStorage->followsTPrevToNextMap.end());
    //            Assert::IsTrue(pkbStorage->followsTPrevToNextMap.find("2") == pkbStorage->followsTPrevToNextMap.end());
    //            Assert::IsTrue(pkbStorage->followsTNextToPrevMap.find("2") != pkbStorage->followsTNextToPrevMap.end());
    //            Assert::IsTrue(pkbStorage->followsTNextToPrevMap.find("1") == pkbStorage->followsTNextToPrevMap.end());
    //            Assert::IsTrue(pkbStorage->followsTPrevToNextMap.at("1") == nextTSet);
    //            Assert::IsTrue(pkbStorage->followsTNextToPrevMap.at("2") == prevTSet);

    //            pkbStorage->storeParentT("1", "2");
    //            std::unordered_set<PKB::LineNum> parentTSet;
    //            std::unordered_set<PKB::LineNum> childTSet;
    //            parentTSet.insert("1");
    //            childTSet.insert("2");
    //            Assert::IsTrue(pkbStorage->parentTSet.find(std::make_pair("1", "2")) != pkbStorage->parentTSet.end());
    //            Assert::IsTrue(pkbStorage->parentTSet.find(std::make_pair("1", "3")) == pkbStorage->parentTSet.end());
    //            Assert::IsTrue(pkbStorage->parentTParentToChildMap.find("1") != pkbStorage->parentTParentToChildMap.end());
    //            Assert::IsTrue(pkbStorage->parentTParentToChildMap.find("2") == pkbStorage->parentTParentToChildMap.end());
    //            Assert::IsTrue(pkbStorage->parentTChildToParentMap.find("2") != pkbStorage->parentTChildToParentMap.end());
    //            Assert::IsTrue(pkbStorage->parentTChildToParentMap.find("1") == pkbStorage->parentTChildToParentMap.end());
    //            Assert::IsTrue(pkbStorage->parentTParentToChildMap.at("1") == childTSet);
    //            Assert::IsTrue(pkbStorage->parentTChildToParentMap.at("2") == parentTSet);

    //            pkbStorage->storeParent("1", "2");
    //            std::unordered_set<PKB::LineNum> parentSet;
    //            std::unordered_set<PKB::LineNum> childSet;
    //            parentSet.insert("1");
    //            childSet.insert("2");
    //            Assert::IsTrue(pkbStorage->parentSet.find(std::make_pair("1", "2")) != pkbStorage->parentSet.end());
    //            Assert::IsTrue(pkbStorage->parentSet.find(std::make_pair("1", "3")) == pkbStorage->parentSet.end());
    //            Assert::IsTrue(pkbStorage->parentParentToChildMap.find("1") != pkbStorage->parentParentToChildMap.end());
    //            Assert::IsTrue(pkbStorage->parentParentToChildMap.find("2") == pkbStorage->parentParentToChildMap.end());
    //            Assert::IsTrue(pkbStorage->parentChildToParentMap.find("2") != pkbStorage->parentChildToParentMap.end());
    //            Assert::IsTrue(pkbStorage->parentChildToParentMap.find("1") == pkbStorage->parentChildToParentMap.end());
    //            Assert::IsTrue(pkbStorage->parentParentToChildMap.at("1") == childSet);
    //            Assert::IsTrue(pkbStorage->parentChildToParentMap.at("2") == parentSet);

    //            pkbStorage->storeCallsT("proc1", "proc2");
    //            std::unordered_set<PKB::Procedure> callerTSet;
    //            std::unordered_set<PKB::Procedure> calleeTSet;
    //            callerTSet.insert("proc1");
    //            calleeTSet.insert("proc2");
    //            Assert::IsFalse(pkbStorage->callsTSet.find(std::make_pair("proc1", "proc2")) == pkbStorage->callsTSet.end());
    //            Assert::IsTrue(pkbStorage->callsTSet.find(std::make_pair("proc1", "proc3")) == pkbStorage->callsTSet.end());
    //            Assert::IsFalse(pkbStorage->callsTCallerToCalleeMap.find("proc1") == pkbStorage->callsTCallerToCalleeMap.end());
    //            Assert::IsTrue(pkbStorage->callsTCallerToCalleeMap.find("proc2") == pkbStorage->callsTCallerToCalleeMap.end());
    //            Assert::IsFalse(pkbStorage->callsTCalleeToCallerMap.find("proc2") == pkbStorage->callsTCalleeToCallerMap.end());
    //            Assert::IsTrue(pkbStorage->callsTCalleeToCallerMap.find("proc1") == pkbStorage->callsTCalleeToCallerMap.end());
    //            Assert::IsTrue(pkbStorage->callsTCallerToCalleeMap.at("proc1") == calleeTSet);
    //            Assert::IsTrue(pkbStorage->callsTCalleeToCallerMap.at("proc2") == callerTSet);

    //            pkbStorage->storeCalls("proc1", "proc2");
    //            std::unordered_set<PKB::Procedure> callerSet;
    //            std::unordered_set<PKB::Procedure> calleeSet;
    //            callerSet.insert("proc1");
    //            calleeSet.insert("proc2");
    //            Assert::IsFalse(pkbStorage->callsSet.find(std::make_pair("proc1", "proc2")) == pkbStorage->callsSet.end());
    //            Assert::IsTrue(pkbStorage->callsSet.find(std::make_pair("proc1", "proc3")) == pkbStorage->callsSet.end());
    //            Assert::IsFalse(pkbStorage->callsCallerToCalleeMap.find("proc1") == pkbStorage->callsCallerToCalleeMap.end());
    //            Assert::IsTrue(pkbStorage->callsCallerToCalleeMap.find("proc2") == pkbStorage->callsCallerToCalleeMap.end());
    //            Assert::IsFalse(pkbStorage->callsCalleeToCallerMap.find("proc2") == pkbStorage->callsCalleeToCallerMap.end());
    //            Assert::IsTrue(pkbStorage->callsCalleeToCallerMap.find("proc1") == pkbStorage->callsCalleeToCallerMap.end());
    //            Assert::IsTrue(pkbStorage->callsCallerToCalleeMap.at("proc1") == calleeSet);
    //            Assert::IsTrue(pkbStorage->callsCalleeToCallerMap.at("proc2") == callerSet);

    //            pkbStorage->storeUsesS("1", "var1");
    //            std::unordered_set<PKB::LineNum> usesSLineSet;
    //            std::unordered_set<PKB::Variable> usesSVarSet;
    //            usesSLineSet.insert("1");
    //            usesSVarSet.insert("var1");
    //            Assert::IsTrue(pkbStorage->usesSSet.find(std::make_pair("1", "var1")) != pkbStorage->usesSSet.end());
    //            Assert::IsTrue(pkbStorage->usesSSet.find(std::make_pair("1", "var2")) == pkbStorage->usesSSet.end());
    //            Assert::IsTrue(pkbStorage->usesSLineToVarMap.find("1") != pkbStorage->usesSLineToVarMap.end());
    //            Assert::IsTrue(pkbStorage->usesSLineToVarMap.find("2") == pkbStorage->usesSLineToVarMap.end());
    //            Assert::IsTrue(pkbStorage->usesSVarToLineMap.find("var1") != pkbStorage->usesSVarToLineMap.end());
    //            Assert::IsTrue(pkbStorage->usesSVarToLineMap.find("var2") == pkbStorage->usesSVarToLineMap.end());
    //            Assert::IsTrue(pkbStorage->usesSLineToVarMap.at("1") == usesSVarSet);
    //            Assert::IsTrue(pkbStorage->usesSVarToLineMap.at("var1") == usesSLineSet);

    //            pkbStorage->storeUsesP("proc1", "var1");
    //            std::unordered_set<PKB::Procedure> usesPProcSet;
    //            std::unordered_set<PKB::Variable> usesPVarSet;
    //            usesPProcSet.insert("proc1");
    //            usesPVarSet.insert("var1");
    //            Assert::IsTrue(pkbStorage->usesPSet.find(std::make_pair("proc1", "var1")) != pkbStorage->usesPSet.end());
    //            Assert::IsTrue(pkbStorage->usesPSet.find(std::make_pair("proc1", "var2")) == pkbStorage->usesPSet.end());
    //            Assert::IsTrue(pkbStorage->usesPProcToVarMap.find("proc1") != pkbStorage->usesPProcToVarMap.end());
    //            Assert::IsTrue(pkbStorage->usesPProcToVarMap.find("proc2") == pkbStorage->usesPProcToVarMap.end());
    //            Assert::IsTrue(pkbStorage->usesPVarToProcMap.find("var1") != pkbStorage->usesPVarToProcMap.end());
    //            Assert::IsTrue(pkbStorage->usesPVarToProcMap.find("var2") == pkbStorage->usesPVarToProcMap.end());
    //            Assert::IsTrue(pkbStorage->usesPProcToVarMap.at("proc1") == usesPVarSet);
    //            Assert::IsTrue(pkbStorage->usesPVarToProcMap.at("var1") == usesPProcSet);

    //            pkbStorage->storeModifiesS("1", "var1");
    //            std::unordered_set<PKB::LineNum> modifiesLineSet;
    //            std::unordered_set<PKB::Variable> modifiesVarSet;
    //            modifiesLineSet.insert("1");
    //            modifiesVarSet.insert("var1");
    //            Assert::IsTrue(pkbStorage->modifiesSSet.find(std::make_pair("1", "var1")) != pkbStorage->modifiesSSet.end());
    //            Assert::IsTrue(pkbStorage->modifiesSSet.find(std::make_pair("1", "var2")) == pkbStorage->modifiesSSet.end());
    //            Assert::IsTrue(pkbStorage->modifiesSLineToVarMap.find("1") != pkbStorage->modifiesSLineToVarMap.end());
    //            Assert::IsTrue(pkbStorage->modifiesSLineToVarMap.find("2") == pkbStorage->modifiesSLineToVarMap.end());
    //            Assert::IsTrue(pkbStorage->modifiesSVarToLineMap.find("var1") != pkbStorage->modifiesSVarToLineMap.end());
    //            Assert::IsTrue(pkbStorage->modifiesSVarToLineMap.find("var2") == pkbStorage->modifiesSVarToLineMap.end());
    //            Assert::IsTrue(pkbStorage->modifiesSLineToVarMap.at("1") == modifiesVarSet);
    //            Assert::IsTrue(pkbStorage->modifiesSVarToLineMap.at("var1") == modifiesLineSet);

    //            pkbStorage->storeModifiesP("proc1", "var1");
    //            std::unordered_set<PKB::LineNum> modifiesPProcSet;
    //            std::unordered_set<PKB::Variable> modifiesPVarSet;
    //            modifiesPProcSet.insert("proc1");
    //            modifiesPVarSet.insert("var1");
    //            Assert::IsTrue(pkbStorage->modifiesPSet.find(std::make_pair("proc1", "var1")) != pkbStorage->modifiesPSet.end());
    //            Assert::IsTrue(pkbStorage->modifiesPSet.find(std::make_pair("proc1", "var2")) == pkbStorage->modifiesPSet.end());
    //            Assert::IsTrue(pkbStorage->modifiesPProcToVarMap.find("proc1") != pkbStorage->modifiesPProcToVarMap.end());
    //            Assert::IsTrue(pkbStorage->modifiesPProcToVarMap.find("proc2") == pkbStorage->modifiesPProcToVarMap.end());
    //            Assert::IsTrue(pkbStorage->modifiesPVarToProcMap.find("var1") != pkbStorage->modifiesPVarToProcMap.end());
    //            Assert::IsTrue(pkbStorage->modifiesPVarToProcMap.find("var2") == pkbStorage->modifiesPVarToProcMap.end());
    //            Assert::IsTrue(pkbStorage->modifiesPProcToVarMap.at("proc1") == modifiesPVarSet);
    //            Assert::IsTrue(pkbStorage->modifiesPVarToProcMap.at("var1") == modifiesPProcSet);*/

    //            pkbStorage->storeAssignPattern("var1", "1", "1");
    //            std::unordered_set<std::pair<PKB::LineNum, PKB::Variable>, PKB::pairHash> lineVarSet;
    //            std::unordered_set<std::pair<PKB::LineNum, PKB::ExprStr>, PKB::pairHash> lineExprSet;
    //            lineVarSet.insert(std::make_pair("1", "var1"));
    //            lineExprSet.insert(std::make_pair("1", "1"));
    //            Assert::IsTrue(pkbStorage->assignLineVarSet.find(std::make_pair("1", "var1")) != pkbStorage->assignLineVarSet.end());
    //            Assert::IsTrue(pkbStorage->assignLineVarSet.find(std::make_pair("1", "var2")) == pkbStorage->assignLineVarSet.end());
    //            Assert::IsTrue(pkbStorage->assignVarToLineExprMap.find("var1") != pkbStorage->assignVarToLineExprMap.end());
    //            Assert::IsTrue(pkbStorage->assignVarToLineExprMap.find("var2") == pkbStorage->assignVarToLineExprMap.end());
    //            Assert::IsTrue(pkbStorage->assignExprToLineVarMap.find("1") != pkbStorage->assignExprToLineVarMap.end());
    //            Assert::IsTrue(pkbStorage->assignExprToLineVarMap.find("2") == pkbStorage->assignExprToLineVarMap.end());
    //            Assert::IsTrue(pkbStorage->assignExprToLineVarMap.at("1") == lineVarSet);
    //            Assert::IsTrue(pkbStorage->assignVarToLineExprMap.at("var1") == lineExprSet);

    //            pkbStorage->storeProcFirstLine("proc1", "3");
    //            pkbStorage->storeProcFirstLine("proc2", "10");
    //            Assert::IsTrue(pkbStorage->procFirstLineMap.find("proc1") != pkbStorage->procFirstLineMap.end());
    //            Assert::IsTrue(pkbStorage->procFirstLineMap.find("proc2") != pkbStorage->procFirstLineMap.end());
    //            Assert::IsTrue(pkbStorage->procFirstLineMap.find("proc3") == pkbStorage->procFirstLineMap.end());
    //            Assert::IsTrue(pkbStorage->procFirstLineMap.at("proc1") == "3");
    //            Assert::IsTrue(pkbStorage->procFirstLineMap.at("proc2") == "10");
    //            Assert::IsTrue(pkbStorage->procFirstLineMap.at("proc1") != "1");
    //            Assert::IsTrue(pkbStorage->procFirstLineMap.at("proc2") != "1");

    //            pkbStorage->storeProcLastLine("proc1", "3");
    //            pkbStorage->storeProcLastLine("proc1", "5");
    //            pkbStorage->storeProcLastLine("proc1", "7");
    //            std::unordered_set<PKB::LineNum> lastLineSet;
    //            lastLineSet.insert("3");
    //            lastLineSet.insert("5");
    //            lastLineSet.insert("7");
    //            Assert::IsTrue(pkbStorage->procLastLineMap.find("proc1") != pkbStorage->procLastLineMap.end());
    //            Assert::IsTrue(pkbStorage->procLastLineMap.find("proc2") == pkbStorage->procLastLineMap.end());
    //            Assert::IsTrue(pkbStorage->procLastLineMap.at("proc1") == lastLineSet);
    //        }

    //        TEST_METHOD(TestPKBStorage) {
    //            PKB::PKBStorage PKBStorage;
    //            std::shared_ptr<PKB::PKBStorage> pkbStorage = PKBStorage.getPKBStorage();

    //            //CFG
    //            pkbStorage->storeCFGEdge("1", "2");
    //            pkbStorage->storeCFGEdge("1", "3");
    //            std::unordered_set<PKB::NextLine> cfgNextLineSet;
    //            cfgNextLineSet.insert("2");
    //            cfgNextLineSet.insert("3");
    //            Assert::IsTrue(PKBStorage.getCFG().at("1") == cfgNextLineSet);

    //            ////modifies
    //            //pkbStorage->storeModifiesS("1", "var1");
    //            //pkbStorage->storeModifiesS("1", "var2");
    //            //std::unordered_set<PKB::Variable> modifiesVarSet;
    //            //std::unordered_set<PKB::LineNum> modifiesLineSet;
    //            //std::unordered_set<std::pair<PKB::LineNum, PKB::Variable>, PKB::pairHash> allModifiesSet;

    //            ////getModifies
    //            //Assert::IsTrue(PKBStorage.getModifiesS("1", "var1"));
    //            //Assert::IsTrue(PKBStorage.getModifiesS("1", "var2"));
    //            //Assert::IsFalse(PKBStorage.getModifiesS("2", "var1"));
    //            //Assert::IsFalse(PKBStorage.getModifiesS("1", "var3"));

    //            ////getModifiesUS
    //            //Assert::IsTrue(PKBStorage.getModifiesSUS("1"));
    //            //Assert::IsFalse(PKBStorage.getModifiesSUS("2"));

    //            ////getModifiesVarByStmt
    //            //Assert::IsFalse(PKBStorage.getModifiesSVarByStmt("1") == modifiesVarSet);
    //            //modifiesVarSet.insert("var1");
    //            //modifiesVarSet.insert("var2");
    //            //Assert::IsTrue(PKBStorage.getModifiesSVarByStmt("1") == modifiesVarSet);
    //            //Assert::IsFalse(PKBStorage.getModifiesSVarByStmt("2") == modifiesVarSet);

    //            ////getModifiesStmtByVar
    //            //Assert::IsFalse(PKBStorage.getModifiesSStmtByVar("var1") == modifiesLineSet);
    //            //modifiesLineSet.insert("1");
    //            //Assert::IsTrue(PKBStorage.getModifiesSStmtByVar("var1") == modifiesLineSet);
    //            //Assert::IsTrue(PKBStorage.getModifiesSStmtByVar("var2") == modifiesLineSet);
    //            //Assert::IsFalse(PKBStorage.getModifiesSStmtByVar("var3") == modifiesLineSet);

    //            ////getAllModifies
    //            //Assert::IsFalse(PKBStorage.getAllModifiesS() == allModifiesSet);
    //            //allModifiesSet.insert(std::make_pair("1", "var1"));
    //            //allModifiesSet.insert(std::make_pair("1", "var2"));
    //            //Assert::IsTrue(PKBStorage.getAllModifiesS() == allModifiesSet);

    //            ////getModifiesStmtByUS
    //            //Assert::IsTrue(PKBStorage.getModifiesSStmtByUS() == modifiesLineSet);

    //            ////uses
    //            //pkbStorage->storeUsesS("1", "var1");
    //            //pkbStorage->storeUsesS("1", "var2");
    //            //std::unordered_set<PKB::Variable> usesVarSet;
    //            //std::unordered_set<PKB::LineNum> usesLineSet;
    //            //std::unordered_set<std::pair<PKB::LineNum, PKB::Variable>, PKB::pairHash> allUsesSet;

    //            ////getUses
    //            //Assert::IsTrue(PKBStorage.getUsesS("1", "var1"));
    //            //Assert::IsTrue(PKBStorage.getUsesS("1", "var2"));
    //            //Assert::IsFalse(PKBStorage.getUsesS("2", "var1"));
    //            //Assert::IsFalse(PKBStorage.getUsesS("1", "var3"));

    //            ////getUsesUS
    //            //Assert::IsTrue(PKBStorage.getUsesSUS("1"));
    //            //Assert::IsFalse(PKBStorage.getUsesSUS("2"));

    //            ////getUsesVarByStmt
    //            //Assert::IsFalse(PKBStorage.getUsesSVarByStmt("1") == usesVarSet);
    //            //usesVarSet.insert("var1");
    //            //usesVarSet.insert("var2");
    //            //Assert::IsTrue(PKBStorage.getUsesSVarByStmt("1") == usesVarSet);
    //            //Assert::IsFalse(PKBStorage.getUsesSVarByStmt("2") == usesVarSet);

    //            ////getUsesStmtByVar
    //            //Assert::IsFalse(PKBStorage.getUsesSStmtByVar("var1") == usesLineSet);
    //            //usesLineSet.insert("1");
    //            //Assert::IsTrue(PKBStorage.getUsesSStmtByVar("var1") == usesLineSet);
    //            //Assert::IsTrue(PKBStorage.getUsesSStmtByVar("var2") == usesLineSet);
    //            //Assert::IsFalse(PKBStorage.getUsesSStmtByVar("var3") == usesLineSet);

    //            ////getAllUses
    //            //Assert::IsFalse(PKBStorage.getAllUsesS() == allUsesSet);
    //            //allUsesSet.insert(std::make_pair("1", "var1"));
    //            //allUsesSet.insert(std::make_pair("1", "var2"));
    //            //Assert::IsTrue(PKBStorage.getAllUsesS() == allUsesSet);

    //            ////getUsesStmtByUS
    //            //Assert::IsTrue(PKBStorage.getUsesSStmtByUS() == usesLineSet);

    //            //std::unordered_set<PKB::PrevLine> prevSet;
    //            //std::unordered_set<PKB::PrevLine> prevTSet;
    //            //std::unordered_set<PKB::PrevLine> allPrevSet;
    //            //std::unordered_set<PKB::NextLine> nextSet;
    //            //std::unordered_set<PKB::NextLine> nextTSet;
    //            //std::unordered_set<PKB::NextLine> allNextSet;
    //            //std::unordered_set<std::pair<PKB::PrevLine, PKB::NextLine>, PKB::pairHash> allFollowsSet;
    //            //std::unordered_set<std::pair<PKB::PrevLine, PKB::NextLine>, PKB::pairHash> allFollowsTSet;

    //            //Assert::IsFalse(PKBStorage.getFollowsByUSUS());
    //            //Assert::IsTrue(PKBStorage.getFollowsPrevByUS() == allPrevSet);
    //            //Assert::IsTrue(PKBStorage.getFollowsNextByUS() == allNextSet);
    //            //Assert::IsTrue(PKBStorage.getAllFollows() == allFollowsSet);

    //            //pkbStorage->storeFollows("1", "2");
    //            //pkbStorage->storeFollows("2", "3");
    //            //pkbStorage->storeFollows("3", "5");
    //            //pkbStorage->storeFollowsT("1", "2");
    //            //pkbStorage->storeFollowsT("1", "3");
    //            //pkbStorage->storeFollowsT("1", "5");
    //            //pkbStorage->storeFollowsT("2", "3");
    //            //pkbStorage->storeFollowsT("2", "5");
    //            //pkbStorage->storeFollowsT("3", "5");

    //            ////getFollows
    //            //Assert::IsTrue(PKBStorage.getFollows("1", "2"));
    //            //Assert::IsFalse(PKBStorage.getFollows("1", "3"));
    //            //Assert::IsFalse(PKBStorage.getFollows("4", "5"));
    //            //Assert::IsFalse(PKBStorage.getFollows("4", "6"));
    //            //
    //            ////getFollowsByPrevUS
    //            //Assert::IsTrue(PKBStorage.getFollowsByPrevUS("1"));
    //            //Assert::IsFalse(PKBStorage.getFollowsByPrevUS("4"));
    //            //Assert::IsFalse(PKBStorage.getFollowsByPrevUS("5"));

    //            ////getFollowsByUSNext
    //            //Assert::IsTrue(PKBStorage.getFollowsByUSNext("2"));
    //            //Assert::IsFalse(PKBStorage.getFollowsByUSNext("4"));
    //            //Assert::IsFalse(PKBStorage.getFollowsByUSNext("1"));

    //            ////getFollowsByUSUS
    //            //Assert::IsTrue(PKBStorage.getFollowsByUSUS());

    //            ////getFollowsNextByPrev
    //            //Assert::IsTrue(PKBStorage.getFollowsNextByPrev("4") == nextSet);
    //            //Assert::IsTrue(PKBStorage.getFollowsNextByPrev("5") == nextSet);
    //            //nextSet.insert("2");
    //            //Assert::IsTrue(PKBStorage.getFollowsNextByPrev("1") == nextSet);
    //            //Assert::IsFalse(PKBStorage.getFollowsNextByPrev("2") == nextSet);

    //            ////getFollowsPrevByNext
    //            //Assert::IsTrue(PKBStorage.getFollowsPrevByNext("4") == prevSet);
    //            //Assert::IsTrue(PKBStorage.getFollowsPrevByNext("1") == prevSet);
    //            //prevSet.insert("1");
    //            //Assert::IsTrue(PKBStorage.getFollowsPrevByNext("2") == prevSet);
    //            //Assert::IsFalse(PKBStorage.getFollowsPrevByNext("3") == prevSet);

    //            ////getFollowsPrevByUS
    //            //allPrevSet.insert("1");
    //            //allPrevSet.insert("2");
    //            //allPrevSet.insert("3");
    //            //Assert::IsTrue(PKBStorage.getFollowsPrevByUS() == allPrevSet);

    //            ////getFollowsNextByUS
    //            //allNextSet.insert("2");
    //            //allNextSet.insert("3");
    //            //allNextSet.insert("5");
    //            //Assert::IsTrue(PKBStorage.getFollowsNextByUS() == allNextSet);

    //            ////getAllFollows
    //            //allFollowsSet.insert(std::make_pair("1", "2"));
    //            //allFollowsSet.insert(std::make_pair("2", "3"));
    //            //allFollowsSet.insert(std::make_pair("3", "5"));
    //            //Assert::IsTrue(PKBStorage.getAllFollows() == allFollowsSet);

    //            ////getFollowsT
    //            //Assert::IsTrue(PKBStorage.getFollowsT("1", "2"));
    //            //Assert::IsTrue(PKBStorage.getFollowsT("1", "3"));
    //            //Assert::IsTrue(PKBStorage.getFollowsT("1", "5"));
    //            //Assert::IsFalse(PKBStorage.getFollowsT("4", "5"));
    //            //Assert::IsFalse(PKBStorage.getFollowsT("3", "4"));
    //            //Assert::IsFalse(PKBStorage.getFollowsT("4", "6"));

    //            ////getFollowsTNextByPrev
    //            //nextTSet.insert("2");
    //            //nextTSet.insert("3");
    //            //nextTSet.insert("5");
    //            //Assert::IsTrue(PKBStorage.getFollowsTNextByPrev("1") == nextTSet);
    //            //Assert::IsFalse(PKBStorage.getFollowsTNextByPrev("4") == nextTSet);
    //            //Assert::IsFalse(PKBStorage.getFollowsTNextByPrev("2") == nextTSet);

    //            ////getFollowsTPrevByNext
    //            //prevTSet.insert("3");
    //            //prevTSet.insert("2");
    //            //prevTSet.insert("1");
    //            //Assert::IsTrue(PKBStorage.getFollowsTPrevByNext("5") == prevTSet);
    //            //Assert::IsFalse(PKBStorage.getFollowsTPrevByNext("4") == prevTSet);
    //            //Assert::IsFalse(PKBStorage.getFollowsTPrevByNext("3") == prevTSet);

    //            ////getAllFollowsT
    //            //allFollowsTSet.insert(std::make_pair("1", "2"));
    //            //allFollowsTSet.insert(std::make_pair("1", "3"));
    //            //allFollowsTSet.insert(std::make_pair("1", "5"));
    //            //allFollowsTSet.insert(std::make_pair("2", "3"));
    //            //allFollowsTSet.insert(std::make_pair("2", "5"));
    //            //allFollowsTSet.insert(std::make_pair("3", "5"));
    //            //Assert::IsTrue(PKBStorage.getAllFollowsT() == allFollowsTSet);


    //            //std::unordered_set<PKB::ParentLine> parentSet;
    //            //std::unordered_set<PKB::ParentLine> parentTSet;
    //            //std::unordered_set<PKB::ParentLine> allparentSet;
    //            //std::unordered_set<PKB::ChildLine> childSet;
    //            //std::unordered_set<PKB::ChildLine> childTSet;
    //            //std::unordered_set<PKB::ChildLine> allChildSet;
    //            //std::unordered_set<std::pair<PKB::ParentLine, PKB::ChildLine>, PKB::pairHash> allParentSet;
    //            //std::unordered_set<std::pair<PKB::ParentLine, PKB::ChildLine>, PKB::pairHash> allParentTSet;

    //            //Assert::IsFalse(PKBStorage.getParentByUSUS());
    //            //Assert::IsTrue(PKBStorage.getParentParentByUS() == allparentSet);
    //            //Assert::IsTrue(PKBStorage.getParentChildByUS() == allChildSet);
    //            //Assert::IsTrue(PKBStorage.getAllParent() == allParentSet);

    //            //pkbStorage->storeParent("1", "2");
    //            //pkbStorage->storeParent("2", "3");
    //            //pkbStorage->storeParent("3", "5");
    //            //pkbStorage->storeParent("3", "7");
    //            //pkbStorage->storeParentT("1", "2");
    //            //pkbStorage->storeParentT("1", "3");
    //            //pkbStorage->storeParentT("1", "5");
    //            //pkbStorage->storeParentT("1", "7");
    //            //pkbStorage->storeParentT("2", "3");
    //            //pkbStorage->storeParentT("2", "5");
    //            //pkbStorage->storeParentT("2", "7");
    //            //pkbStorage->storeParentT("3", "5");
    //            //pkbStorage->storeParentT("3", "7");

    //            ////getParent
    //            //Assert::IsTrue(PKBStorage.getParent("1", "2"));
    //            //Assert::IsFalse(PKBStorage.getParent("1", "3"));
    //            //Assert::IsFalse(PKBStorage.getParent("4", "5"));
    //            //Assert::IsFalse(PKBStorage.getParent("4", "6"));

    //            ////getParentByParentUS
    //            //Assert::IsTrue(PKBStorage.getParentByParentUS("1"));
    //            //Assert::IsFalse(PKBStorage.getParentByParentUS("4"));
    //            //Assert::IsFalse(PKBStorage.getParentByParentUS("5"));

    //            ////getParentByUSChild
    //            //Assert::IsTrue(PKBStorage.getParentByUSChild("2"));
    //            //Assert::IsFalse(PKBStorage.getParentByUSChild("4"));
    //            //Assert::IsFalse(PKBStorage.getParentByUSChild("1"));

    //            ////getParentByUSUS
    //            //Assert::IsTrue(PKBStorage.getParentByUSUS());

    //            ////getParentChildByParent
    //            //Assert::IsTrue(PKBStorage.getParentChildByParent("4") == childSet);
    //            //Assert::IsTrue(PKBStorage.getParentChildByParent("5") == childSet);
    //            //childSet.insert("5");
    //            //childSet.insert("7");
    //            //Assert::IsTrue(PKBStorage.getParentChildByParent("3") == childSet);
    //            //Assert::IsFalse(PKBStorage.getParentChildByParent("2") == childSet);

    //            ////getParentParentByChild
    //            //Assert::IsTrue(PKBStorage.getParentParentByChild("4") == parentSet);
    //            //Assert::IsTrue(PKBStorage.getParentParentByChild("1") == parentSet);
    //            //parentSet.insert("1");
    //            //Assert::IsTrue(PKBStorage.getParentParentByChild("2") == parentSet);
    //            //Assert::IsFalse(PKBStorage.getParentParentByChild("3") == parentSet);

    //            ////getParentParentByUS
    //            //allparentSet.insert("1");
    //            //allparentSet.insert("2");
    //            //allparentSet.insert("3");
    //            //Assert::IsTrue(PKBStorage.getParentParentByUS() == allparentSet);

    //            ////getParentChildByUS
    //            //allChildSet.insert("2");
    //            //allChildSet.insert("3");
    //            //allChildSet.insert("5");
    //            //allChildSet.insert("7");
    //            //Assert::IsTrue(PKBStorage.getParentChildByUS() == allChildSet);

    //            ////getAllParent
    //            //allParentSet.insert(std::make_pair("1", "2"));
    //            //allParentSet.insert(std::make_pair("2", "3"));
    //            //allParentSet.insert(std::make_pair("3", "5"));
    //            //allParentSet.insert(std::make_pair("3", "7"));
    //            //Assert::IsTrue(PKBStorage.getAllParent() == allParentSet);

    //            ////getParentT
    //            //Assert::IsTrue(PKBStorage.getParentT("1", "2"));
    //            //Assert::IsTrue(PKBStorage.getParentT("1", "3"));
    //            //Assert::IsTrue(PKBStorage.getParentT("1", "5"));
    //            //Assert::IsFalse(PKBStorage.getParentT("4", "5"));
    //            //Assert::IsFalse(PKBStorage.getParentT("3", "4"));
    //            //Assert::IsFalse(PKBStorage.getParentT("4", "6"));

    //            ////getParentTChildByParent
    //            //childTSet.insert("2");
    //            //childTSet.insert("3");
    //            //childTSet.insert("5");
    //            //childTSet.insert("7");
    //            //Assert::IsTrue(PKBStorage.getParentTChildByParent("1") == childTSet);
    //            //Assert::IsFalse(PKBStorage.getParentTChildByParent("4") == childTSet);
    //            //Assert::IsFalse(PKBStorage.getParentTChildByParent("2") == childTSet);

    //            ////getParentTParentByChild
    //            //parentTSet.insert("3");
    //            //parentTSet.insert("2");
    //            //parentTSet.insert("1");
    //            //Assert::IsTrue(PKBStorage.getParentTParentByChild("5") == parentTSet);
    //            //Assert::IsFalse(PKBStorage.getParentTParentByChild("4") == parentTSet);
    //            //Assert::IsFalse(PKBStorage.getParentTParentByChild("3") == parentTSet);

    //            ////getAllParentT
    //            //allParentTSet.insert(std::make_pair("1", "2"));
    //            //allParentTSet.insert(std::make_pair("1", "3"));
    //            //allParentTSet.insert(std::make_pair("1", "5"));
    //            //allParentTSet.insert(std::make_pair("1", "7"));
    //            //allParentTSet.insert(std::make_pair("2", "3"));
    //            //allParentTSet.insert(std::make_pair("2", "5"));
    //            //allParentTSet.insert(std::make_pair("2", "7"));
    //            //allParentTSet.insert(std::make_pair("3", "5"));
    //            //allParentTSet.insert(std::make_pair("3", "7"));
    //            //Assert::IsTrue(PKBStorage.getAllParentT() == allParentTSet);

    //            ////Calls and CallsT
    //            //std::unordered_set<PKB::CallerProc> callerSet;
    //            //std::unordered_set<PKB::CallerProc> callerTSet;
    //            //std::unordered_set<PKB::CallerProc> allCallerSet;
    //            //std::unordered_set<PKB::CalleeProc> calleeSet;
    //            //std::unordered_set<PKB::CalleeProc> calleeTSet;
    //            //std::unordered_set<PKB::CalleeProc> allCalleeSet;
    //            //std::unordered_set<std::pair<PKB::CallerProc, PKB::CalleeProc>, PKB::pairHash> allCallsSet;
    //            //std::unordered_set<std::pair<PKB::CallerProc, PKB::CalleeProc>, PKB::pairHash> allCallsTSet;

    //            //Assert::IsFalse(PKBStorage.getCallsByUSUS());
    //            //Assert::IsTrue(PKBStorage.getCallsCallerByUS() == allCallerSet);
    //            //Assert::IsTrue(PKBStorage.getCallsCalleeByUS() == allCalleeSet);
    //            //Assert::IsTrue(PKBStorage.getAllCalls() == allCallsSet);

    //            //pkbStorage->storeCalls("proc1", "proc2");
    //            //pkbStorage->storeCalls("proc2", "proc3");
    //            //pkbStorage->storeCalls("proc3", "proc5");
    //            //pkbStorage->storeCallsT("proc1", "proc2");
    //            //pkbStorage->storeCallsT("proc2", "proc3");
    //            //pkbStorage->storeCallsT("proc3", "proc5");
    //            //pkbStorage->storeCallsT("proc1", "proc3");
    //            //pkbStorage->storeCallsT("proc1", "proc5");
    //            //pkbStorage->storeCallsT("proc2", "proc5");

    //            ////getCalls
    //            //Assert::IsTrue(PKBStorage.getCalls("proc1", "proc2"));
    //            //Assert::IsTrue(PKBStorage.getCalls("proc2", "proc3"));
    //            //Assert::IsFalse(PKBStorage.getCalls("proc4", "proc5"));
    //            //Assert::IsFalse(PKBStorage.getCalls("proc2", "proc6"));

    //            ////getCallsByCallerUS
    //            //Assert::IsTrue(PKBStorage.getCallsByCallerUS("proc1"));
    //            //Assert::IsTrue(PKBStorage.getCallsByCallerUS("proc3"));
    //            //Assert::IsFalse(PKBStorage.getCallsByCallerUS("proc4"));
    //            //Assert::IsFalse(PKBStorage.getCallsByCallerUS("proc5"));

    //            ////getCallsByUSCallee
    //            //Assert::IsTrue(PKBStorage.getCallsByUSCallee("proc2"));
    //            //Assert::IsTrue(PKBStorage.getCallsByUSCallee("proc3"));
    //            //Assert::IsFalse(PKBStorage.getCallsByUSCallee("proc1"));
    //            //Assert::IsFalse(PKBStorage.getCallsByUSCallee("proc4"));

    //            ////getCallsByUSUS
    //            //Assert::IsTrue(PKBStorage.getCallsByUSUS());

    //            ////getCallsCalleeByCaller
    //            //Assert::IsTrue(PKBStorage.getCallsCalleeByCaller("proc4") == calleeSet);
    //            //Assert::IsTrue(PKBStorage.getCallsCalleeByCaller("proc5") == calleeSet);
    //            //calleeSet.insert("proc2");
    //            //Assert::IsTrue(PKBStorage.getCallsCalleeByCaller("proc1") == calleeSet);
    //            //Assert::IsFalse(PKBStorage.getCallsCalleeByCaller("proc2") == calleeSet);

    //            ////getCallsCallerByCallee
    //            //Assert::IsTrue(PKBStorage.getCallsCallerByCallee("proc4") == callerSet);
    //            //Assert::IsTrue(PKBStorage.getCallsCallerByCallee("proc1") == callerSet);
    //            //callerSet.insert("proc1");
    //            //Assert::IsTrue(PKBStorage.getCallsCallerByCallee("proc2") == callerSet);
    //            //Assert::IsFalse(PKBStorage.getCallsCallerByCallee("proc3") == callerSet);

    //            ////getCallsCallerByUS
    //            //allCallerSet.insert("proc1");
    //            //allCallerSet.insert("proc2");
    //            //allCallerSet.insert("proc3");
    //            //Assert::IsTrue(PKBStorage.getCallsCallerByUS() == allCallerSet);

    //            ////getCallsCalleeByUS
    //            //allCalleeSet.insert("proc2");
    //            //allCalleeSet.insert("proc3");
    //            //allCalleeSet.insert("proc5");
    //            //Assert::IsTrue(PKBStorage.getCallsCalleeByUS() == allCalleeSet);

    //            ////getAllCalls
    //            //allCallsSet.insert(std::make_pair("proc1", "proc2"));
    //            //allCallsSet.insert(std::make_pair("proc2", "proc3"));
    //            //allCallsSet.insert(std::make_pair("proc3", "proc5"));
    //            //Assert::IsTrue(PKBStorage.getAllCalls() == allCallsSet);

    //            ////getCallsT
    //            //Assert::IsTrue(PKBStorage.getCallsT("proc1", "proc2"));
    //            //Assert::IsTrue(PKBStorage.getCallsT("proc1", "proc3"));
    //            //Assert::IsTrue(PKBStorage.getCallsT("proc1", "proc5"));
    //            //Assert::IsFalse(PKBStorage.getCallsT("proc4", "proc5"));
    //            //Assert::IsFalse(PKBStorage.getCallsT("proc3", "proc4"));
    //            //Assert::IsFalse(PKBStorage.getCallsT("proc4", "proc6"));

    //            ////getCallsTCalleeByCaller
    //            //calleeTSet.insert("proc2");
    //            //calleeTSet.insert("proc3");
    //            //calleeTSet.insert("proc5");
    //            //Assert::IsTrue(PKBStorage.getCallsTCalleeByCaller("proc1") == calleeTSet);
    //            //Assert::IsFalse(PKBStorage.getCallsTCalleeByCaller("proc4") == calleeTSet);
    //            //Assert::IsFalse(PKBStorage.getCallsTCalleeByCaller("proc2") == calleeTSet);

    //            ////getCallsTCallerByCallee
    //            //callerTSet.insert("proc3");
    //            //callerTSet.insert("proc2");
    //            //callerTSet.insert("proc1");
    //            //Assert::IsTrue(PKBStorage.getCallsTCallerByCallee("proc5") == callerTSet);
    //            //Assert::IsFalse(PKBStorage.getCallsTCallerByCallee("proc4") == callerTSet);
    //            //Assert::IsFalse(PKBStorage.getCallsTCallerByCallee("proc3") == callerTSet);

    //            ////getAllCallsT
    //            //allCallsTSet.insert(std::make_pair("proc1", "proc2"));
    //            //allCallsTSet.insert(std::make_pair("proc1", "proc3"));
    //            //allCallsTSet.insert(std::make_pair("proc1", "proc5"));
    //            //allCallsTSet.insert(std::make_pair("proc2", "proc3"));
    //            //allCallsTSet.insert(std::make_pair("proc2", "proc5"));
    //            //allCallsTSet.insert(std::make_pair("proc3", "proc5"));
    //            //Assert::IsTrue(PKBStorage.getAllCallsT() == allCallsTSet);


    //            std::unordered_set<PKB::LineNum> assignLineSet1;
    //            std::unordered_set<PKB::LineNum> assignLineSet2;
    //            std::unordered_set<PKB::LineNum> assignLineSet3;
    //            std::unordered_set<PKB::LineNum> assignLineSet4;
    //            std::unordered_set<std::pair<PKB::LineNum, PKB::Variable>, PKB::pairHash> assignPairSet1;
    //            std::unordered_set<std::pair<PKB::LineNum, PKB::Variable>, PKB::pairHash> assignPairSet2;

    //            pkbStorage->storeAssign("1");
    //            pkbStorage->storeAssign("2");
    //            pkbStorage->storeAssign("3");
    //            pkbStorage->storeAssign("4");
    //            pkbStorage->storeAssign("5");

    //            pkbStorage->storeAssignPattern("var1", "1", "BinOpNode(-, BinOpNode(+, VariableNode(x), BinOpNode(*, VariableNode(y), VariableNode(z))), ConstantNode(1))");
    //            pkbStorage->storeAssignPattern("var2", "2", "BinOpNode(-, BinOpNode(+, VariableNode(x), BinOpNode(*, VariableNode(y), VariableNode(z))), ConstantNode(1))");
    //            pkbStorage->storeAssignPattern("var1", "3", "BinOpNode(+, VariableNode(x), BinOpNode(*, VariableNode(y), VariableNode(z)))");
    //            pkbStorage->storeAssignPattern("var3", "4", "BinOpNode(*, VariableNode(y), VariableNode(z))");
    //            pkbStorage->storeAssignPattern("var3", "5", "VariableNode(y)");


    //            //getAssignLineByVarUS
    //            Assert::IsTrue(PKBStorage.getAssignLineByVarUS("var4") == assignLineSet1);
    //            assignLineSet1.insert("1");
    //            assignLineSet1.insert("3");
    //            Assert::IsTrue(PKBStorage.getAssignLineByVarUS("var1") == assignLineSet1);
    //            Assert::IsFalse(PKBStorage.getAssignLineByVarUS("var2") == assignLineSet1);


    //            //getAssignLineByVarMatchPartial
    //            assignLineSet2.insert("1");
    //            assignLineSet2.insert("3");
    //            Assert::IsTrue(PKBStorage.getAssignLineByVarMatchPartial("var1", "_z_") == assignLineSet2);
    //            Assert::IsFalse(PKBStorage.getAssignLineByVarMatchPartial("var1", "_1_") == assignLineSet2);

    //            //getAssignLineVarByUS
    //            assignPairSet1.insert(std::make_pair("1", "var1"));
    //            assignPairSet1.insert(std::make_pair("2", "var2"));
    //            assignPairSet1.insert(std::make_pair("3", "var1"));
    //            assignPairSet1.insert(std::make_pair("4", "var3"));
    //            assignPairSet1.insert(std::make_pair("5", "var3"));
    //            Assert::IsTrue(PKBStorage.getAssignLineVarByUS() == assignPairSet1);

    //            //getAssignLineVarByMatchPartial
    //            assignPairSet2.insert(std::make_pair("1", "var1"));
    //            assignPairSet2.insert(std::make_pair("2", "var2"));
    //            assignPairSet2.insert(std::make_pair("3", "var1"));
    //            Assert::IsTrue(PKBStorage.getAssignLineVarByMatchPartial("_x_") == assignPairSet2);

    //            //getAssignLineByUSUS
    //            assignLineSet3.insert("1");
    //            assignLineSet3.insert("2");
    //            assignLineSet3.insert("3");
    //            assignLineSet3.insert("4");
    //            assignLineSet3.insert("5");
    //            Assert::IsTrue(PKBStorage.getAssignLineByUSUS() == assignLineSet3);

    //            //getAssignLineByUSMatchPartial
    //            assignLineSet4.insert("1");
    //            assignLineSet4.insert("2");
    //            assignLineSet4.insert("3");
    //            assignLineSet4.insert("4");
    //            assignLineSet4.insert("5");
    //            Assert::IsTrue(PKBStorage.getAssignLineByUSMatchPartial("_  y  _") == assignLineSet4);
    //        }
    //};
}
