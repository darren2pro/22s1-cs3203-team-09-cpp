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
                std::unordered_set<PKBStorage::LineNum> lineSet;
                std::unordered_set<PKBStorage::Variable> varSet;
                lineSet.insert("1");
                varSet.insert("var1");
            }
    };
}
