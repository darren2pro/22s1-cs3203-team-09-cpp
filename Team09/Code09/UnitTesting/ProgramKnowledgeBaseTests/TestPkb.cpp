#include "stdafx.h"
#include "CppUnitTest.h"
#include "ProgramKnowledgeBase/PKBStorage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting {
    TEST_CLASS(TestPkb) {
        public:
        TEST_METHOD(TestPkbStorage) {
            PKB::PKBStorage* pkb = new PKB::PKBStorage();

            Assert::IsTrue(pkb->relationIsEmpty(Relation::Follows));

            pkb->storeRelations(Relation::Follows, "1", "2");

            Assert::IsFalse(pkb->relationIsEmpty(Relation::Follows));

            pkb->storeEntity(Declaration::Variable, "x");

            //NextT
            pkb->storeRelations(Relation::Next, "1", "2");
            pkb->storeRelations(Relation::Next, "2", "3");
            pkb->storeRelations(Relation::Next, "1", "3");

            std::unordered_set<PKB::NextLine> nextLineSet;
            std::unordered_set<PKB::PrevLine> prevLineSet;
            nextLineSet.insert("2");
            nextLineSet.insert("3");
            prevLineSet.insert("1");
            prevLineSet.insert("2");

            Assert::IsTrue(pkb->getNextLineT("1") == nextLineSet);
            Assert::IsFalse(pkb->getNextLineT("2") == nextLineSet);
            Assert::IsTrue(pkb->getPreviousLineT("3") == prevLineSet);
            Assert::IsFalse(pkb->getPreviousLineT("2") == nextLineSet);
        }
    };
}
