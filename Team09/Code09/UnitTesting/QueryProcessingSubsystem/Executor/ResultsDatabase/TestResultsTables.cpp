#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include <vector>
#include "QueryProcessingSubsystem/Executor/ResultsDatabase/ResultsTables.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestResultsTables) {

	public:
		TEST_METHOD(OneElementList) {
			std::string var = "a";
			std::unordered_set<std::string> list = {"1"};

			ResultsTables rt;
			rt.create(var, list);

			for (int i = 0; i < list.size(); i++) {
				Assert::AreEqual(*begin(list), rt.resultsTable[0][0]);
			}
		}

		TEST_METHOD(TwoElementList) {
			const std::string var = "a";
			std::unordered_set<std::string> list = {"1", "2"};

			ResultsTables rt;
			rt.create(var, list);

			for (int i = 0; i < list.size(); i++) {
				Assert::IsTrue(list.find(rt.resultsTable[i][0]) != list.end());
			}
		}

		TEST_METHOD(OneElementListPair) {
			const std::string var1 = "a";
			const std::string var2 = "b";

			std::unordered_set<std::pair<std::string, std::string>, PairHasher::pairHash> pairList = {};
			pairList.insert(std::make_pair("h", "hello"));
		}
    };
}
