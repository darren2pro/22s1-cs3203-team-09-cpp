#include "stdafx.h"
#include "CppUnitTest.h"
#include <ProgramKnowledgeBase/PKBManager.h>
#include <ProgramKnowledgeBase/PKBStorage.cpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting {
    TEST_CLASS(TestPkb) {
            public:
            TEST_METHOD(TestPkbStorage) {
                PKBManager pkbManager;
                shared_ptr<PKBStorage> pkbStorage = pkbManager.getPKBStorage();
                pkbStorage->storeProcedure("procMain");
                pkbStorage->storeProcedure("proc1");
                pkbStorage->storeProcedure("proc2");
                pkbStorage->storeProcedure("proc3");
                pkbStorage->storeVariable("var1");
                pkbStorage->storeVariable("var2");
                pkbStorage->storeVariable("var3");
                VariableNodePtr var1 = make_shared<VariableNode>("var1");
                VariableNodePtr var2 = make_shared<VariableNode>("var2");
                VariableNodePtr var3 = make_shared<VariableNode>("var3");
                AssignmentNodePtr assignNode1 = make_shared<AssignmentNode>(var1, make_shared<ConstantNode>("1"));
                AssignmentNodePtr assignNode2 = make_shared<AssignmentNode>(var2, make_shared<ConstantNode>("2"));
                AssignmentNodePtr assignNode3 = make_shared<AssignmentNode>(var3, make_shared<ConstantNode>("3"));
                PKBStorage::LineNum lineNum1 = pkbStorage->storeLine(assignNode1);
                PKBStorage::LineNum lineNum2 = pkbStorage->storeLine(assignNode2);
                PKBStorage::LineNum lineNum3 = pkbStorage->storeLine(assignNode3);
                pkbStorage->storeModifiesS(lineNum1, "var1");
                pkbStorage->storeModifiesS(lineNum2, "var2");
                pkbStorage->storeModifiesS(lineNum3, "var3");

                // Check that the line numbers are correct
                Assert::AreEqual("1", lineNum1.c_str());
                Assert::AreEqual("2", lineNum2.c_str());
                Assert::AreEqual("3", lineNum3.c_str());

                // Check that the modifiesSet contains the correct stuff
                Assert::IsTrue(pkbStorage->modifiesSet.find(std::make_pair(lineNum1, "var1")) != pkbStorage->modifiesSet.end());
                Assert::IsTrue(pkbStorage->modifiesSet.find(std::make_pair(lineNum2, "var2")) != pkbStorage->modifiesSet.end());
                Assert::IsTrue(pkbStorage->modifiesSet.find(std::make_pair(lineNum3, "var3")) != pkbStorage->modifiesSet.end());

                // Check that the API returns the correct stuff
                Assert::IsTrue(pkbManager.getModifies(lineNum1, "var1"));
                Assert::IsTrue(pkbManager.getModifies(lineNum2, "var2"));
                Assert::IsTrue(pkbManager.getModifies(lineNum3, "var3"));
                Assert::IsFalse(pkbManager.getModifies(lineNum1, "var2"));
                Assert::IsFalse(pkbManager.getModifies(lineNum2, "var3"));
                Assert::IsFalse(pkbManager.getModifies(lineNum3, "var1"));
            }
    };
}
