#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include <vector>
#include "QueryProcessingSubsystem/Expression.h"
#include "QueryProcessingSubsystem/Declaration.h"

#include <iostream>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestExpression) {

public:
    TEST_METHOD(UnderscoreType) {
        Expression e = Expression("_");

        Assert::IsTrue(e.TYPE == Expression::Types::Underscore);
        Assert::IsTrue(e.value == "_");

        Assert::IsTrue(e.isUnderscore());
        Assert::IsFalse(e.isRelaxed());
        Assert::IsFalse(e.isStrict());
    }

    TEST_METHOD(StrictType) {
        Expression e = Expression("\"x+1\"");

        Assert::IsTrue(e.TYPE == Expression::Types::Strict);
        Assert::IsTrue(e.value == "x+1");

        Assert::IsFalse(e.isUnderscore());
        Assert::IsFalse(e.isRelaxed());
        Assert::IsTrue(e.isStrict());
    }

    TEST_METHOD(RelaxedType) {
        Expression e = Expression("_\"x+1\"_");

        Assert::IsTrue(e.TYPE == Expression::Types::Relaxed);
        Assert::IsTrue(e.value == "x+1");

        Assert::IsFalse(e.isUnderscore());
        Assert::IsTrue(e.isRelaxed());
        Assert::IsFalse(e.isStrict());
    }

    };
}