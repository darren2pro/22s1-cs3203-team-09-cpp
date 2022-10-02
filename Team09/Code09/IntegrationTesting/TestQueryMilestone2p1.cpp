#include "stdafx.h"
#include "CppUnitTest.h"
#include <SPAManager/SPAManager.h>
#include <QueryProcessingSubsystem/Validator/SemanticException.h>
#include <SourceProcessor/exceptions/SimpleInvalidSyntaxException.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace IntegrationTesting {
    TEST_CLASS(TestQueryMilestone2p1) {
		//! Remove or comment out this block to test your code
        BEGIN_TEST_CLASS_ATTRIBUTE(TestQueryMilestone2p1)
            TEST_CLASS_ATTRIBUTE(L"Ignore", L"true")
        END_TEST_CLASS_ATTRIBUTE()

        TEST_METHOD(TestNextNextTPart1) {
                string program = "procedure First {\n"
                                 "        read x;\n" // line 1
                                 "        read z;\n" // line 2
                                 "        call Second; }\n" // line 3
                                 "\n"
                                 "procedure Second {\n"
                                 "                x = 0;\n" // line 4
                                 "                i = 5;\n" // line 5
                                 "                while (i!=0) {\n" // line 6
                                 "                        x = x + 2*y;\n" // line 7
                                 "                        call Third;\n" // line 8
                                 "                        i = i - 1; }\n" // line 9
                                 "                if (x==1) then {\n" // line 10
                                 "                        x = x+1; }\n" // line 11
                                 "        else {\n"
                                 "                        z = 1; }\n" // line 12
                                 "                z = z + x + i;\n" // line 13
                                 "                y = z + 2;\n" // line 14
                                 "                x = x * y + z; }\n" // line 15
                                 "\n"
                                 "procedure Third {\n"
                                 "        z = 5;\n" // line 16
                                 "        v = z;\n" // line 17
                                 "        print v; }"; // line 18
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1
                string query1 = "assign a; stmt s; read r;\n"
                                "Select r such that Next(a, 1)";
                unordered_set<string> queryResults1 = spaManager.query(query1);
                // Empty expected results
                Assert::AreEqual(0, (int) queryResults1.size(), L"Query 1 fails");

                //! Query 2
                string query2 = "assign a; stmt s; read r;\n"
                                "Select r such that Next(2, 3)";
                unordered_set<string> queryResults2 = spaManager.query(query2);
                // Expected results: 1, 2
                Assert::AreEqual(2, (int) queryResults2.size(), L"Query 2 fails");
                Assert::IsTrue(queryResults2.find("1") != queryResults2.end(), L"Query 2 fails");
                Assert::IsTrue(queryResults2.find("2") != queryResults2.end(), L"Query 2 fails");

                //! Query 3 - statements in different procedures
                string query3 = "assign a; stmt s; read r;\n"
                                "Select r such that Next(3, 4)";
                unordered_set<string> queryResults3 = spaManager.query(query3);
                // Empty expected results
                Assert::AreEqual(0, (int) queryResults3.size(), L"Query 3 fails");

                //! Query 4
                string query4 = "assign a; stmt s; read r;\n"
                                "Select r such that Next(6, 7)";
                unordered_set<string> queryResults4 = spaManager.query(query4);
                // Expected results: 1, 2
                Assert::AreEqual(2, (int) queryResults4.size(), L"Query 4 fails");
                Assert::IsTrue(queryResults4.find("1") != queryResults4.end(), L"Query 4 fails");
                Assert::IsTrue(queryResults4.find("2") != queryResults4.end(), L"Query 4 fails");

                //! Query 5
                string query5 = "assign a; stmt s; read r;\n"
                                "Select r such that Next(6, 10)";
                unordered_set<string> queryResults5 = spaManager.query(query5);
                // Expected results: 1, 2
                Assert::AreEqual(2, (int) queryResults5.size(), L"Query 5 fails");
                Assert::IsTrue(queryResults5.find("1") != queryResults5.end(), L"Query 5 fails");
                Assert::IsTrue(queryResults5.find("2") != queryResults5.end(), L"Query 5 fails");

                //! Query 6
                string query6 = "assign a; stmt s; read r;\n"
                                "Select r such that Next(10, 11)";
                unordered_set<string> queryResults6 = spaManager.query(query6);
                // Expected results: 1, 2
                Assert::AreEqual(2, (int) queryResults6.size(), L"Query 6 fails");
                Assert::IsTrue(queryResults6.find("1") != queryResults6.end(), L"Query 6 fails");
                Assert::IsTrue(queryResults6.find("2") != queryResults6.end(), L"Query 6 fails");

                //! Query 7
                string query7 = "assign a; stmt s; read r;\n"
                                "Select r such that Next(10, 12)";
                unordered_set<string> queryResults7 = spaManager.query(query7);
                // Expected results: 1, 2
                Assert::AreEqual(2, (int) queryResults7.size(), L"Query 7 fails");
                Assert::IsTrue(queryResults7.find("1") != queryResults7.end(), L"Query 7 fails");
                Assert::IsTrue(queryResults7.find("2") != queryResults7.end(), L"Query 7 fails");

                //! Query 8
                string query8 = "assign a; stmt s; read r;\n"
                                "Select r such that Next(8, 16)";
                unordered_set<string> queryResults8 = spaManager.query(query8);
                // Empty expected results
                Assert::AreEqual(0, (int) queryResults8.size(), L"Query 8 fails");

                //! Query 9
                string query9 = "assign a; stmt s; read r;\n"
                                "Select s such that Next*(6, s)";
                unordered_set<string> queryResults9 = spaManager.query(query9);
                // Expected results: 7, 8, 9, 10, 11, 12, 13, 14, 15
                Assert::AreEqual(9, (int) queryResults9.size(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("7") != queryResults9.end(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("8") != queryResults9.end(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("9") != queryResults9.end(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("10") != queryResults9.end(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("11") != queryResults9.end(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("12") != queryResults9.end(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("13") != queryResults9.end(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("14") != queryResults9.end(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("15") != queryResults9.end(), L"Query 9 fails");

                //! Query 10
                string query10 = "assign a; stmt s; read r;\n"
                                "Select s such that Next*(s, 16)";
                unordered_set<string> queryResults10 = spaManager.query(query10);
                // Empty expected results
                Assert::AreEqual(0, (int) queryResults10.size(), L"Query 10 fails");

                //! Query 11
                string query11 = "assign a; stmt s; read r;\n"
                                "Select s such that Next*(s, 13)";
                unordered_set<string> queryResults11 = spaManager.query(query11);
                // Expected results: 12, 10, 9, 8, 7, 6, 5, 4
                Assert::AreEqual(8, (int) queryResults11.size(), L"Query 11 fails");
                Assert::IsTrue(queryResults11.find("12") != queryResults11.end(), L"Query 11 fails");
                Assert::IsTrue(queryResults11.find("10") != queryResults11.end(), L"Query 11 fails");
                Assert::IsTrue(queryResults11.find("9") != queryResults11.end(), L"Query 11 fails");
                Assert::IsTrue(queryResults11.find("8") != queryResults11.end(), L"Query 11 fails");
                Assert::IsTrue(queryResults11.find("7") != queryResults11.end(), L"Query 11 fails");
                Assert::IsTrue(queryResults11.find("6") != queryResults11.end(), L"Query 11 fails");
                Assert::IsTrue(queryResults11.find("5") != queryResults11.end(), L"Query 11 fails");
                Assert::IsTrue(queryResults11.find("4") != queryResults11.end(), L"Query 11 fails");

                //! Query 12
                string query12 = "assign a; stmt s; read r;\n"
                                "Select s such that Next*(18, s)";
                unordered_set<string> queryResults12 = spaManager.query(query12);
                // Empty expected results
                Assert::AreEqual(0, (int) queryResults12.size(), L"Query 12 fails");

                //! Query 13
                string query13 = "assign a; stmt s; read r;\n"
                                "Select s such that Next*(11, s)";
                unordered_set<string> queryResults13 = spaManager.query(query13);
                // Empty expected results
                Assert::AreEqual(0, (int) queryResults13.size(), L"Query 13 fails");
        }

        TEST_METHOD(TestFollowsParent) {
            // todo implement this
        }
    };
}
