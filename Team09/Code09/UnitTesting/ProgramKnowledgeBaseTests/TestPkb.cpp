#include "stdafx.h"
#include <iostream>
#include "CppUnitTest.h"
#include "ProgramKnowledgeBase/PKBStorage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting {
    TEST_CLASS(TestPkb) {
        public:
        TEST_METHOD(TestPkbStorage) {
            PKB::PKBStorage* pkb = new PKB::PKBStorage();

            // entities: variable
            std::unordered_set<std::string> varSet;
            Assert::IsFalse(pkb->entityContains(Declaration::Variable, "x"));
            pkb->storeEntity(Declaration::Variable, "x");
            Assert::IsTrue(pkb->entityContains(Declaration::Variable, "x"));
            Assert::IsFalse(pkb->getEntitySet(Declaration::Variable) == varSet);
            varSet.insert("x");
            Assert::IsTrue(pkb->getEntitySet(Declaration::Variable) == varSet);


            // relations: Follows
            std::unordered_set<std::string> secondFromFirstSet;
            std::unordered_set<std::string> firstFromSecondSet;
            std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> allFollowsSet;
            Assert::IsTrue(pkb->relationIsEmpty(Relation::Follows));
            pkb->storeRelations(Relation::Follows, "1", "2");
            Assert::IsFalse(pkb->relationIsEmpty(Relation::Follows));
            Assert::IsTrue(pkb->relationContainsSet(Relation::Follows, "1", "2"));
            Assert::IsTrue(pkb->relationContainsFirst(Relation::Follows, "1"));
            Assert::IsFalse(pkb->relationContainsFirst(Relation::Follows, "2"));
            Assert::IsTrue(pkb->relationContainsSecond(Relation::Follows, "2"));
            Assert::IsFalse(pkb->relationContainsSecond(Relation::Follows, "1"));

            // getRelationSecondFromFirst
            Assert::IsFalse(pkb->getRelationSecondFromFirst(Relation::Follows, "1") == secondFromFirstSet);
            secondFromFirstSet.insert("2");
            Assert::IsTrue(pkb->getRelationSecondFromFirst(Relation::Follows, "1") == secondFromFirstSet);

            // getRelationFirstFromSecond
            Assert::IsFalse(pkb->getRelationFirstFromSecond(Relation::Follows, "2") == firstFromSecondSet);
            firstFromSecondSet.insert("1");
            Assert::IsTrue(pkb->getRelationFirstFromSecond(Relation::Follows, "2") == firstFromSecondSet);

            pkb->storeRelations(Relation::Follows, "3", "4");

            // getRelationAllSecond
            Assert::IsFalse(pkb->getRelationAllSecond(Relation::Follows) == secondFromFirstSet);
            secondFromFirstSet.insert("4");
            Assert::IsTrue(pkb->getRelationAllSecond(Relation::Follows) == secondFromFirstSet);

            // getRelationAllFirst
            Assert::IsFalse(pkb->getRelationAllFirst(Relation::Follows) == firstFromSecondSet);
            firstFromSecondSet.insert("3");
            Assert::IsTrue(pkb->getRelationAllFirst(Relation::Follows) == firstFromSecondSet);

            // getRelationSet
            Assert::IsFalse(pkb->getRelationSet(Relation::Follows) == allFollowsSet);
            allFollowsSet.insert(std::make_pair("1", "2"));
            allFollowsSet.insert(std::make_pair("3", "4"));
            Assert::IsTrue(pkb->getRelationSet(Relation::Follows) == allFollowsSet);


            // patterns: assign
            std::unordered_set<PKB::LineNum> lineNo;
            std::unordered_set<std::pair<PKB::LineNum, PKB::Variable>, PKB::pairHash> lineVarSet;
            Assert::IsTrue(pkb->getPatternLineByVar(Pattern::Assign, "x") == lineNo);
            Assert::IsTrue(pkb->getPatternLineByUS(Pattern::Assign) == lineNo);
            // x = 1 + 2 * 3 - 4 / 5 % 6
            pkb->storePatterns(Pattern::Assign, "x", "1", "AssignmentNode(VariableNode(x), BinOpNode(-, BinOpNode(+, ConstantNode(1), BinOpNode(*, ConstantNode(2), ConstantNode(3))), BinOpNode(%, BinOpNode(/, ConstantNode(4), ConstantNode(5)), ConstantNode(6))))");
            lineNo.insert("1");
            Assert::IsTrue(pkb->getPatternLineByVar(Pattern::Assign, "x") == lineNo);
            //Assert::IsTrue(pkb->getPatternLineByVarMatchFull(Pattern::Assign, "x", "BinOpNode(-, BinOpNode(+, ConstantNode(1), BinOpNode(*, ConstantNode(2), ConstantNode(3))), BinOpNode(%, BinOpNode(/, ConstantNode(4), ConstantNode(5)), ConstantNode(6)))") == lineNo);
            //Assert::IsTrue(pkb->getPatternLineByVarMatchPartial(Pattern::Assign, "x", "BinOpNode(*, ConstantNode(2), ConstantNode(3))") == lineNo);

            //Assert::IsFalse(pkb->getPatternLineVarSet(Pattern::Assign) == lineVarSet);
            //Assert::IsFalse(pkb->getPatternLineVarByMatchFull(Pattern::Assign, "BinOpNode(-, BinOpNode(+, ConstantNode(1), BinOpNode(*, ConstantNode(2), ConstantNode(3))), BinOpNode(%, BinOpNode(/, ConstantNode(4), ConstantNode(5)), ConstantNode(6)))") == lineVarSet);
            //Assert::IsFalse(pkb->getPatternLineVarByMatchPartial(Pattern::Assign, "BinOpNode(*, ConstantNode(2), ConstantNode(3))") == lineVarSet);
            lineVarSet.insert(std::make_pair("1", "x"));
            //Assert::IsTrue(pkb->getPatternLineVarSet(Pattern::Assign) == lineVarSet);
            //Assert::IsTrue(pkb->getPatternLineVarByMatchFull(Pattern::Assign, "BinOpNode(-, BinOpNode(+, ConstantNode(1), BinOpNode(*, ConstantNode(2), ConstantNode(3))), BinOpNode(%, BinOpNode(/, ConstantNode(4), ConstantNode(5)), ConstantNode(6)))") == lineVarSet);
            //Assert::IsTrue(pkb->getPatternLineVarByMatchPartial(Pattern::Assign, "BinOpNode(*, ConstantNode(2), ConstantNode(3))") == lineVarSet);

            Assert::IsTrue(pkb->getPatternLineByUS(Pattern::Assign) == lineNo);
            //Assert::IsTrue(pkb->getPatternLineByUSMatchFull(Pattern::Assign, "BinOpNode(-, BinOpNode(+, ConstantNode(1), BinOpNode(*, ConstantNode(2), ConstantNode(3))), BinOpNode(%, BinOpNode(/, ConstantNode(4), ConstantNode(5)), ConstantNode(6)))") == lineNo);
            //Assert::IsTrue(pkb->getPatternLineByUSMatchPartial(Pattern::Assign, "BinOpNode(*, ConstantNode(2), ConstantNode(3))") == lineNo);

            // cache
            Assert::IsFalse(pkb->isCacheFullyComputed(Relation::Affects));
        }
    };
}
