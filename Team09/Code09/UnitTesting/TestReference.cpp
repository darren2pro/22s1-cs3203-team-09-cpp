#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include <vector>
#include "QueryProcessingSubsystem/Declaration.h"
#include "QueryProcessingSubsystem/Reference.h"

#include <iostream>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestReference) {

public:
    TEST_METHOD(UnderscoreType) {
        Reference r = Reference("_");

        Assert::IsTrue(r.value == "_");
        Assert::IsTrue(r.TYPE == Reference::Types::Underscore);
        Assert::IsTrue(r.declaration == Declaration());

        Assert::IsTrue(r.isUnderscore());
        Assert::IsFalse(r.isStmtNum());
        Assert::IsFalse(r.isString());
        Assert::IsFalse(r.isSynonym());
    }

    TEST_METHOD(SynonymType) {
        Reference r = Reference(Declaration(Declaration::DesignEntity::Assignment, "a"));

        Assert::IsTrue(r.value == "a");
        Assert::IsTrue(r.TYPE == Reference::Types::Synonym);
        Assert::IsTrue(r.declaration == Declaration(Declaration::DesignEntity::Assignment, "a"));

        Assert::IsFalse(r.isUnderscore());
        Assert::IsFalse(r.isStmtNum());
        Assert::IsFalse(r.isString());
        Assert::IsTrue(r.isSynonym());
    }

    TEST_METHOD(StmtNumType) {
        Reference r = Reference("12");

        Assert::IsTrue(r.value == "12");
        Assert::IsTrue(r.TYPE == Reference::Types::StmtNum);
        Assert::IsTrue(r.declaration == Declaration());

        Assert::IsFalse(r.isUnderscore());
        Assert::IsTrue(r.isStmtNum());
        Assert::IsFalse(r.isString());
        Assert::IsFalse(r.isSynonym());
    }    
    
    TEST_METHOD(StringType) {
        Reference r = Reference("\"x\"");

        Assert::IsTrue(r.value == "x");
        Assert::IsTrue(r.TYPE == Reference::Types::String);
        Assert::IsTrue(r.declaration == Declaration());

        Assert::IsFalse(r.isUnderscore());
        Assert::IsFalse(r.isStmtNum());
        Assert::IsTrue(r.isString());
        Assert::IsFalse(r.isSynonym());
    }

    };
}