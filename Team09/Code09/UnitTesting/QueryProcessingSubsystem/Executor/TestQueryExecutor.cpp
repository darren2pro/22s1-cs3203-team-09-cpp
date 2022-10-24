
#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include <vector>
#include "QueryProcessingSubsystem/Executor/QueryExecutor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestQueryExecutor) {

	public:
		TEST_METHOD(TestGetMultipleTarget) {
			//ResultsDatabase rdb;
			//rdb.insertList("a", { "1, 2" });
			//rdb.insertList("b", { "3" });
			//rdb.insertList("c", { "5" });
			//auto input = std::vector<std::string>();
			//std::string a = "a";
			//std::string b = "b";
			//std::string c = "c";
			//input.push_back(a);
			//input.push_back(b);
			//input.push_back(c);
			//auto res = rdb.getMultipleTarget(input);

			//
			//Assert::IsTrue(std::find(res.begin(), res.end(), std::string("1, 3, 5")) != res.end());
			//Assert::IsTrue(std::find(res.begin(), res.end(), std::string("2, 3, 5")) != res.end());
		}
	};
}
