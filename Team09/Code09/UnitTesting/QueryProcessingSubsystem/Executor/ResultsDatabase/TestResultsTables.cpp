#include "../UnitTesting/stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include <vector>
#include "QueryProcessingSubsystem/Executor/ResultsDatabase/ResultsTables.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestQueryLexer) {

public:

		TEST_METHOD(TestResultsTablesCreateSampleList) {
			const std::string var = "a";
			const std::unordered_set<std::string> list = {"1"};

			ResultsTables rt;
			rt.create(var, list);

			for (int i = 0; i < list.size(); i++) {
				Assert::AreEqual(*begin(list), rt.resultsTable[0].at(3));
			}
		}
    };
}
