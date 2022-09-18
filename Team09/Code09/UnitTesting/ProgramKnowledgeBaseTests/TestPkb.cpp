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
                PKBManager pkbManager;
                shared_ptr<PKBStorage> pkbStorage = pkbManager.getPKBStorage();

                //Check if statement lines are assigned and stored correctly
                PKBStorage::LineNum line = pkbStorage->storeLine(std::make_shared<AssignmentNode>
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
                std::unordered_set<PKBStorage::LineNum> prevSet;
                std::unordered_set<PKBStorage::LineNum> nextSet;
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

                pkbStorage->storeParent("1", "2");
                std::unordered_set<PKBStorage::LineNum> parentSet;
                std::unordered_set<PKBStorage::LineNum> childSet;
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
                std::unordered_set<PKBStorage::LineNum> usesLineSet;
                std::unordered_set<PKBStorage::Variable> usesVarSet;
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
                std::unordered_set<PKBStorage::LineNum> modifiesLineSet;
                std::unordered_set<PKBStorage::Variable> modifiesVarSet;
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
                std::unordered_set<std::pair<PKBStorage::LineNum, PKBStorage::Variable>, PairHasher::pairHash> lineVarSet;
                std::unordered_set<std::pair<PKBStorage::LineNum, PKBStorage::ExprStr>, PairHasher::pairHash> lineExprSet;
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
                //getModifies

                //getModifiesUS

                //getModifiesVarByStmt

                //getModifesStmtByVar

                //getAllModifies

                //getModifiesStmtByUS

                //getUses

                //getUsesUS

                //getUsesVarByStmt

                //getUsesStmtByVar

                //getAllUses

                //getUsesStmtByUS

                //getFollows

                //getFollowsByPrevUS

                //getFollowsByUSNext

                //getFollowsByUSUS

                //getFollowsNextByPrev

                //getFollowsPrevByNext

                //getFollowsPrevByUS

                //getFollowsNextByUS

                //getAllFollows

                //getFollowsT

                //getFollowsNextByPrev

                //getFollowsPrevByNext

                //getAllFollowsT

                //getParent

                //getParentByPrevUS

                //getParentByUSNext

                //getParentByUSUS

                //getParentNextByPrev

                //getParentPrevByNext

                //getParentPrevByUS

                //getParentNextByUS

                //getAllParent

                //getParentT

                //getParentTNextByPrev

                //getParentTPrevByNext

                //getAllParentT

                //getAssignLineByVarUS

                //getAssignLineByVarMatchPartial

                //getAssignLineVarByUS

                //getAssignLineVarByMatchPartial

                //getAssignLineByUSUS

                //getAssignLineByUSMatchPartial
            }
    };
}
