#include "stdafx.h"
#include "CppUnitTest.h"
#include "ProgramKnowledgeBase/PKBManager.h"
#include "ProgramKnowledgeBase/PKBStorage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting {
    TEST_CLASS(TestPkb) {
            public:
            TEST_METHOD(TestPkbStorage) {
                PKB::PKBManager pkbManager;
                shared_ptr<PKB::PKBStorage> pkbStorage = pkbManager.getPKBStorage();

                //Check if statement lines are assigned and stored correctly
                PKB::LineNum line = pkbStorage->storeLine(std::make_shared<AssignmentNode>
                    (std::make_shared<VariableNode>("var1"), std::make_shared<ConstantNode>("1")));
                Assert::IsTrue(pkbStorage->stmtSet.find(line) != pkbStorage->stmtSet.end());
                Assert::IsTrue(pkbStorage->stmtSet.find("2") == pkbStorage->stmtSet.end());

                //Check if entity population is correct
                pkbStorage->storeVariable("var1");
                pkbStorage->storeProcedure("proc1");
                pkbStorage->storeConstant("1");
                pkbStorage->storeWhile("1");
                pkbStorage->storeIf("1");
                pkbStorage->storeAssign("1");
                pkbStorage->storeRead("1", "var1");
                pkbStorage->storePrint("1", "var1");

                Assert::IsTrue(pkbStorage->varSet.find("var1") != pkbStorage->varSet.end());
                Assert::IsTrue(pkbStorage->varSet.find("var2") == pkbStorage->varSet.end());
                Assert::IsTrue(pkbStorage->procSet.find("proc1") != pkbStorage->procSet.end());
                Assert::IsTrue(pkbStorage->procSet.find("proc2") == pkbStorage->procSet.end());
                Assert::IsTrue(pkbStorage->constSet.find("1") != pkbStorage->constSet.end());
                Assert::IsTrue(pkbStorage->constSet.find("2") == pkbStorage->constSet.end());
                Assert::IsTrue(pkbStorage->whileSet.find("1") != pkbStorage->whileSet.end());
                Assert::IsTrue(pkbStorage->whileSet.find("2") == pkbStorage->whileSet.end());
                Assert::IsTrue(pkbStorage->ifSet.find("1") != pkbStorage->ifSet.end());
                Assert::IsTrue(pkbStorage->ifSet.find("2") == pkbStorage->ifSet.end());
                Assert::IsTrue(pkbStorage->assignSet.find("1") != pkbStorage->assignSet.end());
                Assert::IsTrue(pkbStorage->assignSet.find("2") == pkbStorage->assignSet.end());
                Assert::IsTrue(pkbStorage->readSet.find("1") != pkbStorage->readSet.end());
                Assert::IsTrue(pkbStorage->readSet.find("2") == pkbStorage->readSet.end());
                Assert::IsTrue(pkbStorage->printSet.find("1") != pkbStorage->printSet.end());
                Assert::IsTrue(pkbStorage->printSet.find("2") == pkbStorage->printSet.end());

                //Check if relations population is correct
                pkbStorage->storeFollows("1", "2");
                std::unordered_set<PKB::LineNum> prevSet;
                std::unordered_set<PKB::LineNum> nextSet;
                prevSet.insert("1");
                nextSet.insert("2");
                Assert::IsTrue(pkbStorage->followsSet.find(std::make_pair("1", "2")) != pkbStorage->followsSet.end());
                Assert::IsTrue(pkbStorage->followsSet.find(std::make_pair("1", "3")) == pkbStorage->followsSet.end());
                Assert::IsTrue(pkbStorage->followsPrevToNextMap.find("1") != pkbStorage->followsPrevToNextMap.end());
                Assert::IsTrue(pkbStorage->followsPrevToNextMap.find("2") == pkbStorage->followsPrevToNextMap.end());
                Assert::IsTrue(pkbStorage->followsNextToPrevMap.find("2") != pkbStorage->followsNextToPrevMap.end());
                Assert::IsTrue(pkbStorage->followsNextToPrevMap.find("1") == pkbStorage->followsNextToPrevMap.end());
                Assert::IsTrue(pkbStorage->followsPrevToNextMap.at("1") == nextSet);
                Assert::IsTrue(pkbStorage->followsNextToPrevMap.at("2") == prevSet);

                pkbStorage->storeFollowsT("1", "2");
                std::unordered_set<PKB::LineNum> prevTSet;
                std::unordered_set<PKB::LineNum> nextTSet;
                prevTSet.insert("1");
                nextTSet.insert("2");
                Assert::IsTrue(pkbStorage->followsTSet.find(std::make_pair("1", "2")) != pkbStorage->followsTSet.end());
                Assert::IsTrue(pkbStorage->followsTSet.find(std::make_pair("1", "3")) == pkbStorage->followsTSet.end());
                Assert::IsTrue(pkbStorage->followsTPrevToNextMap.find("1") != pkbStorage->followsTPrevToNextMap.end());
                Assert::IsTrue(pkbStorage->followsTPrevToNextMap.find("2") == pkbStorage->followsTPrevToNextMap.end());
                Assert::IsTrue(pkbStorage->followsTNextToPrevMap.find("2") != pkbStorage->followsTNextToPrevMap.end());
                Assert::IsTrue(pkbStorage->followsTNextToPrevMap.find("1") == pkbStorage->followsTNextToPrevMap.end());
                Assert::IsTrue(pkbStorage->followsTPrevToNextMap.at("1") == nextTSet);
                Assert::IsTrue(pkbStorage->followsTNextToPrevMap.at("2") == prevTSet);

                pkbStorage->storeParentT("1", "2");
                std::unordered_set<PKB::LineNum> parentTSet;
                std::unordered_set<PKB::LineNum> childTSet;
                parentTSet.insert("1");
                childTSet.insert("2");
                Assert::IsTrue(pkbStorage->parentTSet.find(std::make_pair("1", "2")) != pkbStorage->parentTSet.end());
                Assert::IsTrue(pkbStorage->parentTSet.find(std::make_pair("1", "3")) == pkbStorage->parentTSet.end());
                Assert::IsTrue(pkbStorage->parentTParentToChildMap.find("1") != pkbStorage->parentTParentToChildMap.end());
                Assert::IsTrue(pkbStorage->parentTParentToChildMap.find("2") == pkbStorage->parentTParentToChildMap.end());
                Assert::IsTrue(pkbStorage->parentTChildToParentMap.find("2") != pkbStorage->parentTChildToParentMap.end());
                Assert::IsTrue(pkbStorage->parentTChildToParentMap.find("1") == pkbStorage->parentTChildToParentMap.end());
                Assert::IsTrue(pkbStorage->parentTParentToChildMap.at("1") == childTSet);
                Assert::IsTrue(pkbStorage->parentTChildToParentMap.at("2") == parentTSet);

                pkbStorage->storeParent("1", "2");
                std::unordered_set<PKB::LineNum> parentSet;
                std::unordered_set<PKB::LineNum> childSet;
                parentSet.insert("1");
                childSet.insert("2");
                Assert::IsTrue(pkbStorage->parentSet.find(std::make_pair("1", "2")) != pkbStorage->parentSet.end());
                Assert::IsTrue(pkbStorage->parentSet.find(std::make_pair("1", "3")) == pkbStorage->parentSet.end());
                Assert::IsTrue(pkbStorage->parentParentToChildMap.find("1") != pkbStorage->parentParentToChildMap.end());
                Assert::IsTrue(pkbStorage->parentParentToChildMap.find("2") == pkbStorage->parentParentToChildMap.end());
                Assert::IsTrue(pkbStorage->parentChildToParentMap.find("2") != pkbStorage->parentChildToParentMap.end());
                Assert::IsTrue(pkbStorage->parentChildToParentMap.find("1") == pkbStorage->parentChildToParentMap.end());
                Assert::IsTrue(pkbStorage->parentParentToChildMap.at("1") == childSet);
                Assert::IsTrue(pkbStorage->parentChildToParentMap.at("2") == parentSet);

                pkbStorage->storeUsesS("1", "var1");
                std::unordered_set<PKB::LineNum> usesLineSet;
                std::unordered_set<PKB::Variable> usesVarSet;
                usesLineSet.insert("1");
                usesVarSet.insert("var1");
                Assert::IsTrue(pkbStorage->usesSet.find(std::make_pair("1", "var1")) != pkbStorage->usesSet.end());
                Assert::IsTrue(pkbStorage->usesSet.find(std::make_pair("1", "var2")) == pkbStorage->usesSet.end());
                Assert::IsTrue(pkbStorage->usesLineToVarMap.find("1") != pkbStorage->usesLineToVarMap.end());
                Assert::IsTrue(pkbStorage->usesLineToVarMap.find("2") == pkbStorage->usesLineToVarMap.end());
                Assert::IsTrue(pkbStorage->usesVarToLineMap.find("var1") != pkbStorage->usesVarToLineMap.end());
                Assert::IsTrue(pkbStorage->usesVarToLineMap.find("var2") == pkbStorage->usesVarToLineMap.end());
                Assert::IsTrue(pkbStorage->usesLineToVarMap.at("1") == usesVarSet);
                Assert::IsTrue(pkbStorage->usesVarToLineMap.at("var1") == usesLineSet);

                pkbStorage->storeModifiesS("1", "var1");
                std::unordered_set<PKB::LineNum> modifiesLineSet;
                std::unordered_set<PKB::Variable> modifiesVarSet;
                modifiesLineSet.insert("1");
                modifiesVarSet.insert("var1");
                Assert::IsTrue(pkbStorage->modifiesSet.find(std::make_pair("1", "var1")) != pkbStorage->modifiesSet.end());
                Assert::IsTrue(pkbStorage->modifiesSet.find(std::make_pair("1", "var2")) == pkbStorage->modifiesSet.end());
                Assert::IsTrue(pkbStorage->modifiesLineToVarMap.find("1") != pkbStorage->modifiesLineToVarMap.end());
                Assert::IsTrue(pkbStorage->modifiesLineToVarMap.find("2") == pkbStorage->modifiesLineToVarMap.end());
                Assert::IsTrue(pkbStorage->modifiesVarToLineMap.find("var1") != pkbStorage->modifiesVarToLineMap.end());
                Assert::IsTrue(pkbStorage->modifiesVarToLineMap.find("var2") == pkbStorage->modifiesVarToLineMap.end());
                Assert::IsTrue(pkbStorage->modifiesLineToVarMap.at("1") == modifiesVarSet);
                Assert::IsTrue(pkbStorage->modifiesVarToLineMap.at("var1") == modifiesLineSet);

                pkbStorage->storeAssignPattern("var1", "1", "1");
                std::unordered_set<std::pair<PKB::LineNum, PKB::Variable>, PKB::pairHash> lineVarSet;
                std::unordered_set<std::pair<PKB::LineNum, PKB::ExprStr>, PKB::pairHash> lineExprSet;
                lineVarSet.insert(std::make_pair("1", "var1"));
                lineExprSet.insert(std::make_pair("1", "1"));
                Assert::IsTrue(pkbStorage->assignLineVarSet.find(std::make_pair("1", "var1")) != pkbStorage->assignLineVarSet.end());
                Assert::IsTrue(pkbStorage->assignLineVarSet.find(std::make_pair("1", "var2")) == pkbStorage->assignLineVarSet.end());
                Assert::IsTrue(pkbStorage->assignVarToLineExprMap.find("var1") != pkbStorage->assignVarToLineExprMap.end());
                Assert::IsTrue(pkbStorage->assignVarToLineExprMap.find("var2") == pkbStorage->assignVarToLineExprMap.end());
                Assert::IsTrue(pkbStorage->assignExprToLineVarMap.find("1") != pkbStorage->assignExprToLineVarMap.end());
                Assert::IsTrue(pkbStorage->assignExprToLineVarMap.find("2") == pkbStorage->assignExprToLineVarMap.end());
                Assert::IsTrue(pkbStorage->assignExprToLineVarMap.at("1") == lineVarSet);
                Assert::IsTrue(pkbStorage->assignVarToLineExprMap.at("var1") == lineExprSet);
            }

            TEST_METHOD(TestPkbManager) {
                PKB::PKBManager pkbManager;
                std::shared_ptr<PKB::PKBStorage> pkbStorage = pkbManager.getPKBStorage();

                //modifies
                pkbStorage->storeModifiesS("1", "var1");
                pkbStorage->storeModifiesS("1", "var2");
                std::unordered_set<PKB::Variable> modifiesVarSet;
                std::unordered_set<PKB::LineNum> modifiesLineSet;
                std::unordered_set<std::pair<PKB::LineNum, PKB::Variable>, PKB::pairHash> allModifiesSet;

                //getModifies
                Assert::IsTrue(pkbManager.getModifies("1", "var1"));
                Assert::IsTrue(pkbManager.getModifies("1", "var2"));
                Assert::IsFalse(pkbManager.getModifies("2", "var1"));
                Assert::IsFalse(pkbManager.getModifies("1", "var3"));

                //getModifiesUS
                Assert::IsTrue(pkbManager.getModifiesUS("1"));
                Assert::IsFalse(pkbManager.getModifiesUS("2"));

                //getModifiesVarByStmt
                Assert::IsFalse(pkbManager.getModifiesVarByStmt("1") == modifiesVarSet);
                modifiesVarSet.insert("var1");
                modifiesVarSet.insert("var2");
                Assert::IsTrue(pkbManager.getModifiesVarByStmt("1") == modifiesVarSet);
                Assert::IsFalse(pkbManager.getModifiesVarByStmt("2") == modifiesVarSet);

                //getModifiesStmtByVar
                Assert::IsFalse(pkbManager.getModifiesStmtByVar("var1") == modifiesLineSet);
                modifiesLineSet.insert("1");
                Assert::IsTrue(pkbManager.getModifiesStmtByVar("var1") == modifiesLineSet);
                Assert::IsTrue(pkbManager.getModifiesStmtByVar("var2") == modifiesLineSet);
                Assert::IsFalse(pkbManager.getModifiesStmtByVar("var3") == modifiesLineSet);

                //getAllModifies
                Assert::IsFalse(pkbManager.getAllModifies() == allModifiesSet);
                allModifiesSet.insert(std::make_pair("1", "var1"));
                allModifiesSet.insert(std::make_pair("1", "var2"));
                Assert::IsTrue(pkbManager.getAllModifies() == allModifiesSet);

                //getModifiesStmtByUS
                Assert::IsTrue(pkbManager.getModifiesStmtByUS() == modifiesLineSet);

                //uses
                pkbStorage->storeUsesS("1", "var1");
                pkbStorage->storeUsesS("1", "var2");
                std::unordered_set<PKB::Variable> usesVarSet;
                std::unordered_set<PKB::LineNum> usesLineSet;
                std::unordered_set<std::pair<PKB::LineNum, PKB::Variable>, PKB::pairHash> allUsesSet;

                //getUses
                Assert::IsTrue(pkbManager.getUses("1", "var1"));
                Assert::IsTrue(pkbManager.getUses("1", "var2"));
                Assert::IsFalse(pkbManager.getUses("2", "var1"));
                Assert::IsFalse(pkbManager.getUses("1", "var3"));

                //getUsesUS
                Assert::IsTrue(pkbManager.getUsesUS("1"));
                Assert::IsFalse(pkbManager.getUsesUS("2"));

                //getUsesVarByStmt
                Assert::IsFalse(pkbManager.getUsesVarByStmt("1") == usesVarSet);
                usesVarSet.insert("var1");
                usesVarSet.insert("var2");
                Assert::IsTrue(pkbManager.getUsesVarByStmt("1") == usesVarSet);
                Assert::IsFalse(pkbManager.getUsesVarByStmt("2") == usesVarSet);

                //getUsesStmtByVar
                Assert::IsFalse(pkbManager.getUsesStmtByVar("var1") == usesLineSet);
                usesLineSet.insert("1");
                Assert::IsTrue(pkbManager.getUsesStmtByVar("var1") == usesLineSet);
                Assert::IsTrue(pkbManager.getUsesStmtByVar("var2") == usesLineSet);
                Assert::IsFalse(pkbManager.getUsesStmtByVar("var3") == usesLineSet);

                //getAllUses
                Assert::IsFalse(pkbManager.getAllUses() == allUsesSet);
                allUsesSet.insert(std::make_pair("1", "var1"));
                allUsesSet.insert(std::make_pair("1", "var2"));
                Assert::IsTrue(pkbManager.getAllUses() == allUsesSet);

                //getUsesStmtByUS
                Assert::IsTrue(pkbManager.getUsesStmtByUS() == usesLineSet);

                std::unordered_set<PKB::PrevLine> prevSet;
                std::unordered_set<PKB::PrevLine> prevTSet;
                std::unordered_set<PKB::PrevLine> allPrevSet;
                std::unordered_set<PKB::NextLine> nextSet;
                std::unordered_set<PKB::NextLine> nextTSet;
                std::unordered_set<PKB::NextLine> allNextSet;
                std::unordered_set<std::pair<PKB::PrevLine, PKB::NextLine>, PKB::pairHash> allFollowsSet;
                std::unordered_set<std::pair<PKB::PrevLine, PKB::NextLine>, PKB::pairHash> allFollowsTSet;

                Assert::IsFalse(pkbManager.getFollowsByUSUS());
                Assert::IsTrue(pkbManager.getFollowsPrevByUS() == allPrevSet);
                Assert::IsTrue(pkbManager.getFollowsNextByUS() == allNextSet);
                Assert::IsTrue(pkbManager.getAllFollows() == allFollowsSet);

                pkbStorage->storeFollows("1", "2");
                pkbStorage->storeFollows("2", "3");
                pkbStorage->storeFollows("3", "5");
                pkbStorage->storeFollowsT("1", "2");
                pkbStorage->storeFollowsT("1", "3");
                pkbStorage->storeFollowsT("1", "5");
                pkbStorage->storeFollowsT("2", "3");
                pkbStorage->storeFollowsT("2", "5");
                pkbStorage->storeFollowsT("3", "5");

                //getFollows
                Assert::IsTrue(pkbManager.getFollows("1", "2"));
                Assert::IsFalse(pkbManager.getFollows("1", "3"));
                Assert::IsFalse(pkbManager.getFollows("4", "5"));
                Assert::IsFalse(pkbManager.getFollows("4", "6"));
                
                //getFollowsByPrevUS
                Assert::IsTrue(pkbManager.getFollowsByPrevUS("1"));
                Assert::IsFalse(pkbManager.getFollowsByPrevUS("4"));
                Assert::IsFalse(pkbManager.getFollowsByPrevUS("5"));

                //getFollowsByUSNext
                Assert::IsTrue(pkbManager.getFollowsByUSNext("2"));
                Assert::IsFalse(pkbManager.getFollowsByUSNext("4"));
                Assert::IsFalse(pkbManager.getFollowsByUSNext("1"));

                //getFollowsByUSUS
                Assert::IsTrue(pkbManager.getFollowsByUSUS());

                //getFollowsNextByPrev
                Assert::IsTrue(pkbManager.getFollowsNextByPrev("4") == nextSet);
                Assert::IsTrue(pkbManager.getFollowsNextByPrev("5") == nextSet);
                nextSet.insert("2");
                Assert::IsTrue(pkbManager.getFollowsNextByPrev("1") == nextSet);
                Assert::IsFalse(pkbManager.getFollowsNextByPrev("2") == nextSet);

                //getFollowsPrevByNext
                Assert::IsTrue(pkbManager.getFollowsPrevByNext("4") == prevSet);
                Assert::IsTrue(pkbManager.getFollowsPrevByNext("1") == prevSet);
                prevSet.insert("1");
                Assert::IsTrue(pkbManager.getFollowsPrevByNext("2") == prevSet);
                Assert::IsFalse(pkbManager.getFollowsPrevByNext("3") == prevSet);

                //getFollowsPrevByUS
                allPrevSet.insert("1");
                allPrevSet.insert("2");
                allPrevSet.insert("3");
                Assert::IsTrue(pkbManager.getFollowsPrevByUS() == allPrevSet);

                //getFollowsNextByUS
                allNextSet.insert("2");
                allNextSet.insert("3");
                allNextSet.insert("5");
                Assert::IsTrue(pkbManager.getFollowsNextByUS() == allNextSet);

                //getAllFollows
                allFollowsSet.insert(std::make_pair("1", "2"));
                allFollowsSet.insert(std::make_pair("2", "3"));
                allFollowsSet.insert(std::make_pair("3", "5"));
                Assert::IsTrue(pkbManager.getAllFollows() == allFollowsSet);

                //getFollowsT
                Assert::IsTrue(pkbManager.getFollowsT("1", "2"));
                Assert::IsTrue(pkbManager.getFollowsT("1", "3"));
                Assert::IsTrue(pkbManager.getFollowsT("1", "5"));
                Assert::IsFalse(pkbManager.getFollowsT("4", "5"));
                Assert::IsFalse(pkbManager.getFollowsT("3", "4"));
                Assert::IsFalse(pkbManager.getFollowsT("4", "6"));

                //getFollowsTNextByPrev
                nextTSet.insert("2");
                nextTSet.insert("3");
                nextTSet.insert("5");
                Assert::IsTrue(pkbManager.getFollowsTNextByPrev("1") == nextTSet);
                Assert::IsFalse(pkbManager.getFollowsTNextByPrev("4") == nextTSet);
                Assert::IsFalse(pkbManager.getFollowsTNextByPrev("2") == nextTSet);

                //getFollowsTPrevByNext
                prevTSet.insert("3");
                prevTSet.insert("2");
                prevTSet.insert("1");
                Assert::IsTrue(pkbManager.getFollowsTPrevByNext("5") == prevTSet);
                Assert::IsFalse(pkbManager.getFollowsTPrevByNext("4") == prevTSet);
                Assert::IsFalse(pkbManager.getFollowsTPrevByNext("3") == prevTSet);

                //getAllFollowsT
                allFollowsTSet.insert(std::make_pair("1", "2"));
                allFollowsTSet.insert(std::make_pair("1", "3"));
                allFollowsTSet.insert(std::make_pair("1", "5"));
                allFollowsTSet.insert(std::make_pair("2", "3"));
                allFollowsTSet.insert(std::make_pair("2", "5"));
                allFollowsTSet.insert(std::make_pair("3", "5"));
                Assert::IsTrue(pkbManager.getAllFollowsT() == allFollowsTSet);


                std::unordered_set<PKB::ParentLine> parentSet;
                std::unordered_set<PKB::ParentLine> parentTSet;
                std::unordered_set<PKB::ParentLine> allparentSet;
                std::unordered_set<PKB::ChildLine> childSet;
                std::unordered_set<PKB::ChildLine> childTSet;
                std::unordered_set<PKB::ChildLine> allChildSet;
                std::unordered_set<std::pair<PKB::ParentLine, PKB::ChildLine>, PKB::pairHash> allParentSet;
                std::unordered_set<std::pair<PKB::ParentLine, PKB::ChildLine>, PKB::pairHash> allParentTSet;

                Assert::IsFalse(pkbManager.getParentByUSUS());
                Assert::IsTrue(pkbManager.getParentParentByUS() == allparentSet);
                Assert::IsTrue(pkbManager.getParentChildByUS() == allChildSet);
                Assert::IsTrue(pkbManager.getAllParent() == allParentSet);

                pkbStorage->storeParent("1", "2");
                pkbStorage->storeParent("2", "3");
                pkbStorage->storeParent("3", "5");
                pkbStorage->storeParent("3", "7");
                pkbStorage->storeParentT("1", "2");
                pkbStorage->storeParentT("1", "3");
                pkbStorage->storeParentT("1", "5");
                pkbStorage->storeParentT("1", "7");
                pkbStorage->storeParentT("2", "3");
                pkbStorage->storeParentT("2", "5");
                pkbStorage->storeParentT("2", "7");
                pkbStorage->storeParentT("3", "5");
                pkbStorage->storeParentT("3", "7");

                //getParent
                Assert::IsTrue(pkbManager.getParent("1", "2"));
                Assert::IsFalse(pkbManager.getParent("1", "3"));
                Assert::IsFalse(pkbManager.getParent("4", "5"));
                Assert::IsFalse(pkbManager.getParent("4", "6"));

                //getParentByParentUS
                Assert::IsTrue(pkbManager.getParentByParentUS("1"));
                Assert::IsFalse(pkbManager.getParentByParentUS("4"));
                Assert::IsFalse(pkbManager.getParentByParentUS("5"));

                //getParentByUSChild
                Assert::IsTrue(pkbManager.getParentByUSChild("2"));
                Assert::IsFalse(pkbManager.getParentByUSChild("4"));
                Assert::IsFalse(pkbManager.getParentByUSChild("1"));

                //getParentByUSUS
                Assert::IsTrue(pkbManager.getParentByUSUS());

                //getParentChildByParent
                Assert::IsTrue(pkbManager.getParentChildByParent("4") == childSet);
                Assert::IsTrue(pkbManager.getParentChildByParent("5") == childSet);
                childSet.insert("5");
                childSet.insert("7");
                Assert::IsTrue(pkbManager.getParentChildByParent("3") == childSet);
                Assert::IsFalse(pkbManager.getParentChildByParent("2") == childSet);

                //getParentParentByChild
                Assert::IsTrue(pkbManager.getParentParentByChild("4") == parentSet);
                Assert::IsTrue(pkbManager.getParentParentByChild("1") == parentSet);
                parentSet.insert("1");
                Assert::IsTrue(pkbManager.getParentParentByChild("2") == parentSet);
                Assert::IsFalse(pkbManager.getParentParentByChild("3") == parentSet);

                //getParentParentByUS
                allparentSet.insert("1");
                allparentSet.insert("2");
                allparentSet.insert("3");
                Assert::IsTrue(pkbManager.getParentParentByUS() == allparentSet);

                //getParentChildByUS
                allChildSet.insert("2");
                allChildSet.insert("3");
                allChildSet.insert("5");
                allChildSet.insert("7");
                Assert::IsTrue(pkbManager.getParentChildByUS() == allChildSet);

                //getAllParent
                allParentSet.insert(std::make_pair("1", "2"));
                allParentSet.insert(std::make_pair("2", "3"));
                allParentSet.insert(std::make_pair("3", "5"));
                allParentSet.insert(std::make_pair("3", "7"));
                Assert::IsTrue(pkbManager.getAllParent() == allParentSet);

                //getParentT
                Assert::IsTrue(pkbManager.getParentT("1", "2"));
                Assert::IsTrue(pkbManager.getParentT("1", "3"));
                Assert::IsTrue(pkbManager.getParentT("1", "5"));
                Assert::IsFalse(pkbManager.getParentT("4", "5"));
                Assert::IsFalse(pkbManager.getParentT("3", "4"));
                Assert::IsFalse(pkbManager.getParentT("4", "6"));

                //getParentTChildByParent
                childTSet.insert("2");
                childTSet.insert("3");
                childTSet.insert("5");
                childTSet.insert("7");
                Assert::IsTrue(pkbManager.getParentTChildByParent("1") == childTSet);
                Assert::IsFalse(pkbManager.getParentTChildByParent("4") == childTSet);
                Assert::IsFalse(pkbManager.getParentTChildByParent("2") == childTSet);

                //getParentTParentByChild
                parentTSet.insert("3");
                parentTSet.insert("2");
                parentTSet.insert("1");
                Assert::IsTrue(pkbManager.getParentTParentByChild("5") == parentTSet);
                Assert::IsFalse(pkbManager.getParentTParentByChild("4") == parentTSet);
                Assert::IsFalse(pkbManager.getParentTParentByChild("3") == parentTSet);

                //getAllParentT
                allParentTSet.insert(std::make_pair("1", "2"));
                allParentTSet.insert(std::make_pair("1", "3"));
                allParentTSet.insert(std::make_pair("1", "5"));
                allParentTSet.insert(std::make_pair("1", "7"));
                allParentTSet.insert(std::make_pair("2", "3"));
                allParentTSet.insert(std::make_pair("2", "5"));
                allParentTSet.insert(std::make_pair("2", "7"));
                allParentTSet.insert(std::make_pair("3", "5"));
                allParentTSet.insert(std::make_pair("3", "7"));
                Assert::IsTrue(pkbManager.getAllParentT() == allParentTSet);


                std::unordered_set<PKB::LineNum> assignLineSet1;
                std::unordered_set<PKB::LineNum> assignLineSet2;
                std::unordered_set<PKB::LineNum> assignLineSet3;
                std::unordered_set<PKB::LineNum> assignLineSet4;
                std::unordered_set<std::pair<PKB::LineNum, PKB::Variable>, PKB::pairHash> assignPairSet1;
                std::unordered_set<std::pair<PKB::LineNum, PKB::Variable>, PKB::pairHash> assignPairSet2;

                pkbStorage->storeAssign("1");
                pkbStorage->storeAssign("2");
                pkbStorage->storeAssign("3");
                pkbStorage->storeAssign("4");
                pkbStorage->storeAssign("5");

                pkbStorage->storeAssignPattern("var1", "1", "BinOpNode(-, BinOpNode(+, VariableNode(x), BinOpNode(*, VariableNode(y), VariableNode(z))), ConstantNode(1))");
                pkbStorage->storeAssignPattern("var2", "2", "BinOpNode(-, BinOpNode(+, VariableNode(x), BinOpNode(*, VariableNode(y), VariableNode(z))), ConstantNode(1))");
                pkbStorage->storeAssignPattern("var1", "3", "BinOpNode(+, VariableNode(x), BinOpNode(*, VariableNode(y), VariableNode(z)))");
                pkbStorage->storeAssignPattern("var3", "4", "BinOpNode(*, VariableNode(y), VariableNode(z))");
                pkbStorage->storeAssignPattern("var3", "5", "VariableNode(y)");


                //getAssignLineByVarUS
                Assert::IsTrue(pkbManager.getAssignLineByVarUS("var4") == assignLineSet1);
                assignLineSet1.insert("1");
                assignLineSet1.insert("3");
                Assert::IsTrue(pkbManager.getAssignLineByVarUS("var1") == assignLineSet1);
                Assert::IsFalse(pkbManager.getAssignLineByVarUS("var2") == assignLineSet1);


                //getAssignLineByVarMatchPartial
                assignLineSet2.insert("1");
                assignLineSet2.insert("3");
                Assert::IsTrue(pkbManager.getAssignLineByVarMatchPartial("var1", "_z_") == assignLineSet2);
                Assert::IsFalse(pkbManager.getAssignLineByVarMatchPartial("var1", "_1_") == assignLineSet2);

                //getAssignLineVarByUS
                assignPairSet1.insert(std::make_pair("1", "var1"));
                assignPairSet1.insert(std::make_pair("2", "var2"));
                assignPairSet1.insert(std::make_pair("3", "var1"));
                assignPairSet1.insert(std::make_pair("4", "var3"));
                assignPairSet1.insert(std::make_pair("5", "var3"));
                Assert::IsTrue(pkbManager.getAssignLineVarByUS() == assignPairSet1);

                //getAssignLineVarByMatchPartial
                assignPairSet2.insert(std::make_pair("1", "var1"));
                assignPairSet2.insert(std::make_pair("2", "var2"));
                assignPairSet2.insert(std::make_pair("3", "var1"));
                Assert::IsTrue(pkbManager.getAssignLineVarByMatchPartial("_x_") == assignPairSet2);

                //getAssignLineByUSUS
                assignLineSet3.insert("1");
                assignLineSet3.insert("2");
                assignLineSet3.insert("3");
                assignLineSet3.insert("4");
                assignLineSet3.insert("5");
                Assert::IsTrue(pkbManager.getAssignLineByUSUS() == assignLineSet3);

                //getAssignLineByUSMatchPartial
                assignLineSet4.insert("1");
                assignLineSet4.insert("2");
                assignLineSet4.insert("3");
                assignLineSet4.insert("4");
                assignLineSet4.insert("5");
                Assert::IsTrue(pkbManager.getAssignLineByUSMatchPartial("_  y  _") == assignLineSet4);
            }
    };
}
