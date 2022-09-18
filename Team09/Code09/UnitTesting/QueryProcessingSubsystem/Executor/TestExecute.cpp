#include "stdafx.h"
#include "CppUnitTest.h"
#include <SourceProcessor/Parser.h>
#include <QueryProcessingSubsystem/Query.h>
#include <QueryProcessingSubsystem/Validator/QueryBuilder.h>
#include <QueryProcessingSubsystem/Validator/SyntaxException.h>
#include <QueryProcessingSubsystem/Validator/SemanticException.h>
#include <QueryProcessingSubsystem/Executor/QueryExecutor.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting {
    TEST_CLASS(TestExecute) {
            public:
            TEST_METHOD(TestExecuteSampleQuery) {
                string program = "procedure q {\n"
                                 "  if (x==1) then {\n"
                                 "    z = x + 1; }\n"
                                 "  else {\n"
                                 "    x = z + x; } }\n";
                std::istringstream iss(program);
                SimpleParser parser(&iss);
                PKBManager pkb = parser.parse();
                QueryExecutor executor(pkb);

                //// Query 1
                //string query1 = "while w;\n"
                //                "Select w such that Parent(w, 7)";
                //Query* q1 = QueryBuilder().buildQuery(query1);
                //unordered_set<string> results1 = executor.processQuery(q1);
                //Assert::IsTrue(results1.size() == 0, L"Query 1 fails");

                //// Query 2
                //string query2 = "stmt s;\n"
                //                "Select s such that Parent(1, s)";
                //Query* q2 = QueryBuilder().buildQuery(query2);
                //unordered_set<string> results2 = executor.processQuery(q2);
                //Assert::IsTrue(results2.size() == 2, L"Query 2 fails");
                //// Expected results: 2, 3
                //Assert::IsTrue(results2.find("2") != results2.end());
                //Assert::IsTrue(results2.find("3") != results2.end());

                //// Query 3
                //string query3 = "stmt s;\n"
                //                "Select s such that Parent*(1, s)";
                //Query* q3 = QueryBuilder().buildQuery(query3);
                //unordered_set<string> results3 = executor.processQuery(q3);
                //Assert::IsTrue(results3.size() == 2, L"Query 3 fails");
                //// Expected results: 2, 3
                //Assert::IsTrue(results3.find("2") != results3.end());
                //Assert::IsTrue(results3.find("3") != results3.end());

                //// Query 4
                //string query4 = "stmt s;\n"
                //                "Select s such that Parent*(s, 3)";
                //Query* q4 = QueryBuilder().buildQuery(query4);
                //unordered_set<string> results4 = executor.processQuery(q4);
                //Assert::IsTrue(results4.size() == 0, L"Query 4 fails");
                //// Expected results: none

                //// Query 5
                //string query5 = "stmt s1, s2;\n"
                //                "Select s1 such that Follows(s1, s2)";
                //Query* q5 = QueryBuilder().buildQuery(query5);
                //unordered_set<string> results5 = executor.processQuery(q5);
                //Assert::IsTrue(results5.size() == 0, L"Query 5 fails");
                //// Expected results: none

                //// Query 6
                //string query6 = "stmt s1, s2;\n"
                //                "Select s1 such that Follows*(s1, s2)";
                //Query* q6 = QueryBuilder().buildQuery(query6);
                //unordered_set<string> results6 = executor.processQuery(q6);
                //Assert::IsTrue(results6.size() == 0, L"Query 6 fails");
                //// Expected results: none

                // Query 7
                string query7 = "assign aaa;\n"
                                "Select aaa such that Modifies(aaa, \"x\")"; // Modifies(aaa, "x")
                Query* q7 = QueryBuilder().buildQuery(query7);
                unordered_set<string> results7 = executor.processQuery(q7);
				Assert::IsTrue(results7.size() == 1, L"Query 7 fails");
                // Expected results: 3
                Assert::IsTrue(results7.find("3") != results7.end(), L"Query 7 fails");

                // Query 8
                string query8 = "assign aaa;\n"
                                "Select aaa such that Uses(aaa, \"x\")";
                Query* q8 = QueryBuilder().buildQuery(query8);
                unordered_set<string> results8 = executor.processQuery(q8);
                Assert::IsTrue(results8.size() == 2, L"Query 8 fails");
                // Expected results: 2, 3
                Assert::IsTrue(results8.find("2") != results8.end());
                Assert::IsTrue(results8.find("3") != results8.end());

                // Query 9
                string query9 = "stmt ssssHi;\n"
                                "Select ssssHi such that Modifies(\"z\", _)";
                Query* q9 = QueryBuilder().buildQuery(query9);
                unordered_set<string> results9 = executor.processQuery(q9);
                Assert::IsTrue(results9.size() == 2, L"Query 9 fails");
                // Expected results: 1, 3
                Assert::IsTrue(results9.find("1") != results9.end());
                Assert::IsTrue(results9.find("3") != results9.end());

                // Query 10
                string query10 = "if ssssif; variable myVar; assign myAssign;\n"
                                 "Select myVar such that Modifies(ssssif, myVar) pattern myAssign(myVar, _\"x  +    1\"_)";
                Query* q10 = QueryBuilder().buildQuery(query10);
                unordered_set<string> results10 = executor.processQuery(q10);
                Assert::IsTrue(results10.size() == 1, L"Query 10 fails");
                // Expected results: z
                Assert::IsTrue(results10.find("z") != results10.end());
            }

            TEST_METHOD(TestExecuteMoreSampleQuery) {
                string program = "procedure p {\n"
                                 "  if (x<0) then {\n"
                                 "    while (i>0) {\n"
                                 "      x = z * 3 + 2 * y;\n"
                                 "      i = i - 1; }\n"
                                 "    x = x + 1;\n"
                                 "    z = x + z; }\n"
                                 "  else {\n"
                                 "    z = 1; }\n"
                                 "  z = z + x + i; }\n";
                std::istringstream iss(program);
                SimpleParser parser(&iss);
                PKBManager pkb = parser.parse();
                QueryExecutor executor(pkb);

                // Query 1
                string query1 = "assign MyA;\n"
                                "Select a such that Modifies(MyA, \"x\")";
                Query* q1 = QueryBuilder().buildQuery(query1);
                unordered_set<string> results1 = executor.processQuery(q1);
                Assert::IsTrue(results1.size() == 2, L"Query 1 fails");
                // Expected results: 3, 5
                Assert::IsTrue(results1.find("3") != results1.end());
                Assert::IsTrue(results1.find("5") != results1.end());

                // Query 2
                string query2 = "proc myProc; assign myAssign; variable myVar;\n"
                                "Select myVar such that Modifies(myProc, myVar) pattern myAssign(myVar, _\"2 * y\"_)";
                Query* q2 = QueryBuilder().buildQuery(query2);
                unordered_set<string> results2 = executor.processQuery(q2);
                Assert::IsTrue(results2.size() == 1, L"Query 2 fails");
                // Expected results: x
                Assert::IsTrue(results2.find("x") != results2.end());

                // Query 3
                string query3 = "proc myProc; assign myAssign; variable myVar;\n"
                                "Select myProc such that Modifies(myProc, myVar) pattern myAssign(myVar, _\"i - 1\"_)";
                Query* q3 = QueryBuilder().buildQuery(query3);
                unordered_set<string> results3 = executor.processQuery(q3);
                Assert::IsTrue(results3.size() == 1, L"Query 3 fails");
                // Expected results: p
                Assert::IsTrue(results3.find("p") != results3.end());

                // Query 4
                string query4 = "assign myAssign; variable myVar;\n"
                                "Select myVar such that Modifies(\"p\", myVar) pattern myAssign(myVar, _)";
                Query* q4 = QueryBuilder().buildQuery(query4);
                unordered_set<string> results4 = executor.processQuery(q4);
                Assert::IsTrue(results4.size() == 3, L"Query 4 fails");
                // Expected results: x, i, z
                Assert::IsTrue(results4.find("x") != results4.end());
                Assert::IsTrue(results4.find("i") != results4.end());
                Assert::IsTrue(results4.find("z") != results4.end());

                // Query 5
                string query5 = "proc procproc;"
                                "Select procproc such that Modifies(\"p\", _)";
                Query* q5 = QueryBuilder().buildQuery(query5);
                unordered_set<string> results5 = executor.processQuery(q5);
                Assert::IsTrue(results5.size() == 1, L"Query 5 fails");
                // Expected results: p
                Assert::IsTrue(results5.find("p") != results5.end());

                // Query 6
                string query5 = "stmt ss;"
                                "Select ss such that Uses(ss, \"i\")";
                Query* q5 = QueryBuilder().buildQuery(query5);
                unordered_set<string> results5 = executor.processQuery(q5);
                Assert::IsTrue(results5.size() == 3, L"Query 6 fails");
                // Expected results: 2, 4, 8
                Assert::IsTrue(results5.find("2") != results5.end());
                Assert::IsTrue(results5.find("4") != results5.end());
                Assert::IsTrue(results5.find("8") != results5.end());

                // Query 7
                string query7 = "stmt ss; assign aa;"
                                "Select ss such that Uses(ss, \"x\") pattern aa(\"z\", _)";
                Query* q7 = QueryBuilder().buildQuery(query7);
                unordered_set<string> results7 = executor.processQuery(q7);
                Assert::IsTrue(results7.size() == 2, L"Query 7 fails");
                // Expected results: 1, 3, 5, 6, 8
                Assert::IsTrue(results7.find("1") != results7.end());
                Assert::IsTrue(results7.find("3") != results7.end());
                Assert::IsTrue(results7.find("5") != results7.end());
                Assert::IsTrue(results7.find("6") != results7.end());
                Assert::IsTrue(results7.find("8") != results7.end());
            }
    };
}
