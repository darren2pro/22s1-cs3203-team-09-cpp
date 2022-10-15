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
        }
    };
}
