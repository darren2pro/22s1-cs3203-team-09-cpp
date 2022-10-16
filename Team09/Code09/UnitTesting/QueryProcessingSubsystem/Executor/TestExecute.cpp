#include "stdafx.h"
#include "CppUnitTest.h"
#include <SPAManager/SPAManager.h>
#include <QueryProcessingSubsystem/Validator/SyntaxException.h>
#include <QueryProcessingSubsystem/Validator/SemanticException.h>

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
				SPAManager spaManager;
				spaManager.loadSimpleSourceFromProgram(program);

                // Query 1
                string query1 = "while w;\n"
                                "Select w such that Parent(w, 7)";
                unordered_set<string> results1 = spaManager.query(query1);
                Assert::IsTrue(results1.size() == 0, L"Query 1 fails");

                // Query 2
                string query2 = "stmt s;\n"
                                "Select s such that Parent(1, s)";
                unordered_set<string> results2 = spaManager.query(query2);
                Assert::IsTrue(results2.size() == 2, L"Query 2 fails");
                // Expected results: 2, 3
                Assert::IsTrue(results2.find("2") != results2.end());
                Assert::IsTrue(results2.find("3") != results2.end());

                // Query 3
                string query3 = "stmt s;\n"
                                "Select s such that Parent*(1, s)";
                unordered_set<string> results3 = spaManager.query(query3);
                Assert::IsTrue(results3.size() == 2, L"Query 3 fails");
                // Expected results: 2, 3
                Assert::IsTrue(results3.find("2") != results3.end());
                Assert::IsTrue(results3.find("3") != results3.end());

                // Query 4
                string query4 = "stmt s;\n"
                                "Select s such that Parent*(s, 3)";
                unordered_set<string> results4 = spaManager.query(query4);
                Assert::IsTrue(results4.size() == 1, L"Query 4 fails");
                // Expected results: none

                // Query 5
                string query5 = "stmt s1, s2;\n"
                                "Select s1 such that Follows(s1, s2)";
                unordered_set<string> results5 = spaManager.query(query5);
                Assert::IsTrue(results5.size() == 0, L"Query 5 fails");
                // Expected results: none

                // Query 6
                string query6 = "stmt s1, s2;\n"
                                "Select s1 such that Follows*(s1, s2)";
                unordered_set<string> results6 = spaManager.query(query6);
                Assert::IsTrue(results6.size() == 0, L"Query 6 fails");
                // Expected results: none

                // Query 7
                string query7 = "assign aaa;\n"
                                "Select aaa such that Modifies(aaa, \"x\")"; // Modifies(aaa, "x")
                unordered_set<string> results7 = spaManager.query(query7);
				Assert::IsTrue(results7.size() == 1, L"Query 7 fails");
                // Expected results: 3
                Assert::IsTrue(results7.find("3") != results7.end(), L"Query 7 fails");

                // Query 8
                string query8 = "assign aaa;\n"
                                "Select aaa such that Uses(aaa, \"x\")";
                unordered_set<string> results8 = spaManager.query(query8);
                Assert::IsTrue(results8.size() == 2, L"Query 8 fails");
                // Expected results: 2, 3
                Assert::IsTrue(results8.find("2") != results8.end());
                Assert::IsTrue(results8.find("3") != results8.end());

                // Query 9
                string query9 = "stmt ssssHi; variable ssccvv;\n"
                                "Select ssssHi such that Modifies(ssssHi, ssccvv)";
                unordered_set<string> results9 = spaManager.query(query9);
                Assert::IsTrue(results9.size() == 3, L"Query 9 fails");
                // Expected results: 1, 2, 3
                Assert::IsTrue(results9.find("1") != results9.end());
                Assert::IsTrue(results9.find("2") != results9.end());
				Assert::IsTrue(results9.find("3") != results9.end());

                // Query 10
                string query10 = "if ssssif; variable myVar; assign myAssign;\n"
					"Select myVar such that Modifies(ssssif, myVar) pattern myAssign(_, _)";
                unordered_set<string> results10 = spaManager.query(query10);
                Assert::IsTrue(results10.size() == 2, L"Query 10 fails");
                // Expected results: z, x
                Assert::IsTrue(results10.find("z") != results10.end());
                Assert::IsTrue(results10.find("x") != results10.end());

                // Query 11
                string query11 = "if ssssif; variable myVar; assign myAssign;\n"
                                 "Select myVar such that Modifies(ssssif, myVar) pattern myAssign(_, _\"z\"_)";
                unordered_set<string> results11 = spaManager.query(query11);
                Assert::IsTrue(results11.size() == 2, L"Query 11 fails");
                // Expected results: x, z
                Assert::IsTrue(results11.find("x") != results11.end());
                Assert::IsTrue(results11.find("z") != results11.end());

                // Query 12
                string query12 = "if ssssif; variable myVar; assign myAssign;\n"
                                 "Select myVar such that Modifies(ssssif, myVar) pattern myAssign(myVar, _\"x\"_)";
                unordered_set<string> results12 = spaManager.query(query12);
                Assert::IsTrue(results12.size() == 2, L"Query 12 fails");
                // Expected results: x, z
                Assert::IsTrue(results12.find("x") != results12.end());
                Assert::IsTrue(results12.find("z") != results12.end());
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
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                // Query 1
                string query1 = "assign MyA;\n"
                                "Select MyA such that Modifies(MyA, \"x\")";
                unordered_set<string> results1 = spaManager.query(query1);
                Assert::IsTrue(results1.size() == 2, L"Query 1 fails");
                // Expected results: 3, 5
                Assert::IsTrue(results1.find("3") != results1.end());
                Assert::IsTrue(results1.find("5") != results1.end());

                // Query 2
                string query2 = "procedure myProc; assign myAssign; variable myVar;\n"
                                "Select myVar such that Modifies(myProc, myVar) pattern myAssign(myVar, _\"2 * y\"_)";
                unordered_set<string> results2 = spaManager.query(query2);
                Assert::IsTrue(results2.size() == 1, L"Query 2 fails");
                // Expected results: x
                Assert::IsTrue(results2.find("x") != results2.end());

                // Query 3
                string query3 = "procedure myProc; assign myAssign; variable myVar;\n"
                                "Select myProc such that Modifies(myProc, myVar) pattern myAssign(myVar, _\"i - 1\"_)";
                unordered_set<string> results3 = spaManager.query(query3);
                Assert::IsTrue(results3.size() == 1, L"Query 3 fails");
                // Expected results: p
                Assert::IsTrue(results3.find("p") != results3.end());

                // Query 4
                string query4 = "assign myAssign; variable myVar;\n"
                                "Select myVar such that Modifies(\"p\", myVar) pattern myAssign(myVar, _)";
                unordered_set<string> results4 = spaManager.query(query4);
                Assert::IsTrue(results4.size() == 3, L"Query 4 fails");
                // Expected results: x, i, z
                Assert::IsTrue(results4.find("x") != results4.end());
                Assert::IsTrue(results4.find("i") != results4.end());
                Assert::IsTrue(results4.find("z") != results4.end());

                // Query 5
                string query5 = "procedure procproc;"
                                "Select procproc such that Modifies(\"p\", _)";
                unordered_set<string> results5 = spaManager.query(query5);
                Assert::IsTrue(results5.size() == 1, L"Query 5 fails");
                // Expected results: p
                Assert::IsTrue(results5.find("p") != results5.end());

                // Query 6
                string query6 = "stmt ss;"
                                "Select ss such that Uses(ss, \"i\")";
                unordered_set<string> results6 = spaManager.query(query6);
                Assert::IsTrue(results6.size() == 4, L"Query 6 fails");
                // Expected results: 1, 2, 4, 8
                Assert::IsTrue(results6.find("1") != results6.end());
                Assert::IsTrue(results6.find("2") != results6.end());
                Assert::IsTrue(results6.find("4") != results6.end());
                Assert::IsTrue(results6.find("8") != results6.end());

                // Query 7
                string query7 = "stmt ss; assign aa;"
                                "Select ss such that Uses(ss, \"x\") pattern aa(\"z\", _)";
                unordered_set<string> results7 = spaManager.query(query7);
                Assert::IsTrue(results7.size() == 4, L"Query 7 fails");
                // Expected results: 1, 5, 6, 8
                Assert::IsTrue(results7.find("1") != results7.end());
                Assert::IsTrue(results7.find("5") != results7.end());
                Assert::IsTrue(results7.find("6") != results7.end());
                Assert::IsTrue(results7.find("8") != results7.end());

                // Query 8
                string query8 = "assign aa;"
                                "Select aa pattern aa(\"z\", _)";
                unordered_set<string> results8 = spaManager.query(query8);
                Assert::IsTrue(results8.size() == 3, L"Query 8 fails");
                // Expected results: 6, 7, 8
                Assert::IsTrue(results8.find("6") != results8.end());
                Assert::IsTrue(results8.find("7") != results8.end());
                Assert::IsTrue(results8.find("8") != results8.end());

                // Query 9
                string query9 = "assign a;"
                                "Select a pattern a(\"z\", _\"x\"_)";
                unordered_set<string> results9 = spaManager.query(query9);
                Assert::IsTrue(results9.size() == 2, L"Query 9 fails");
                // Expected results: 6, 8
                Assert::IsTrue(results9.find("6") != results9.end());
                Assert::IsTrue(results9.find("8") != results9.end());

                // Query 10
                string query10 = "assign a;"
                                "Select a pattern a(_, _\"x\"_)";
                unordered_set<string> results10 = spaManager.query(query10);
                Assert::IsTrue(results10.size() == 3, L"Query 10 fails");
                // Expected results: 6, 7, 8
                Assert::IsTrue(results10.find("5") != results10.end());
                Assert::IsTrue(results10.find("6") != results10.end());
                Assert::IsTrue(results10.find("8") != results10.end());
            }

            TEST_METHOD(TestNestedIfWhile) {
                string program = "procedure mainMain {\n"
                                 "        if (iii >= 500) then {\n"
                                 "            while (k <= 111) {\n"
                                 "                beingModified = num1 + num2;\n"
                                 "                while (k < 444) {\n"
                                 "                    beingMod2 = num1 + num2;\n"
                                 "                }\n"
                                 "            }\n"
                                 "        } else {\n"
                                 "            while (yyy != 7000) {\n"
                                 "                if (kkk > 666) then {\n"
                                 "                    beingMod3 = num1 + num2;\n"
                                 "                } else {\n"
                                 "                    call AnotherProc;\n"
                                 "                }\n"
                                 "            }\n"
                                 "        }\n"
                                 "}\n";
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                // Query 1
                string query1 = "assign a;"
                                "Select a pattern a(_, _\"num1\"_)";
                unordered_set<string> results1 = spaManager.query(query1);
                Assert::IsTrue(results1.size() == 3, L"Query 1 fails");
                // Expected results: 3, 5, 8
                Assert::IsTrue(results1.find("3") != results1.end());
                Assert::IsTrue(results1.find("5") != results1.end());
                Assert::IsTrue(results1.find("8") != results1.end());

                // Query 2
                string query2 = "stmt ks; variable sVar;\n"
                                "Select ks such that Modifies(ks, sVar)";
                unordered_set<string> results2 = spaManager.query(query2);
                Assert::IsTrue(results2.size() == 8, L"Query 2 fails");
                // Expected results: 1, 2, 3, 4, 5, 6, 7, 8
                Assert::IsTrue(results2.find("1") != results2.end());
                Assert::IsTrue(results2.find("2") != results2.end());
                Assert::IsTrue(results2.find("3") != results2.end());
                Assert::IsTrue(results2.find("4") != results2.end());
                Assert::IsTrue(results2.find("5") != results2.end());
                Assert::IsTrue(results2.find("6") != results2.end());
                Assert::IsTrue(results2.find("7") != results2.end());
                Assert::IsTrue(results2.find("8") != results2.end());

                // Query 3
                string query3 = "if kif; variable sVar;\n"
                                "Select kif such that Modifies(kif, _)";
                unordered_set<string> results3 = spaManager.query(query3);
                Assert::IsTrue(results3.size() == 2, L"Query 3 fails");
                // Expected results: 1, 7
                Assert::IsTrue(results3.find("1") != results3.end());
                Assert::IsTrue(results3.find("7") != results3.end());

                //TODO: Query 4 - For future milestone
                string query4 = "assign a;"
                                "Select a pattern a(_, \"num1\")";
                unordered_set<string> results4 = spaManager.query(query4);
                Assert::IsTrue(results4.size() == 0, L"Query 4 fails");
                //Expected results: none
            }

            TEST_METHOD(TestSmallProc1) {
                string prog1 = "procedure myProc {\n"
                               "    while (1>= 1%((1)) ) {\n"
                               "        read num1;\n"
                               "    }\n"
                               "}\n";
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(prog1);

                // Query 1
                string query1 = "stmt sss;\n"
                                "Select sss such that Modifies(sss, _)";
                unordered_set<string> results1 = spaManager.query(query1);
                Assert::IsTrue(results1.size() == 2, L"Query 1 fails");
                // Expected results: 1, 2
                Assert::IsTrue(results1.find("1") != results1.end());
                Assert::IsTrue(results1.find("2") != results1.end());
            }

            TEST_METHOD(TestSmallProc2) {
				string prog1 = "procedure myProc {\n"
                               "    while (1>= 1%((0-1)) ) {\n"
                               "        read num1;\n"
                               "    }\n"
                               "}\n";
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(prog1);

                // Query 1
                string query1 = "stmt sss;\n"
                                "Select sss such that Modifies(sss, _)";
                unordered_set<string> results1 = spaManager.query(query1);
                Assert::IsTrue(results1.size() == 2, L"Query 1 fails");
                // Expected results: 1, 2
                Assert::IsTrue(results1.find("1") != results1.end());
                Assert::IsTrue(results1.find("2") != results1.end());
            }

            TEST_METHOD(TestSmallProc3) {
                string prog1 = "procedure myProc {\n"
                               "    while (! ((1==0) && (1==0))) {\n"
                               "        read num1;\n"
                               "    }\n"
                               "}\n";
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(prog1);
            }

            TEST_METHOD(TestMilestone1dash1) {
                string program = "procedure oneDashone {\n"
                                 "        while (1>= 1%((1)) ) {\n"
                                 "            read num1;\n"
                                 "        }\n"
                                 "\n"
                                 "        while (1>= 1%((0-1)) ) {\n"
                                 "            read num2;\n"
                                 "        }\n"
                                 "\n"
                                 "        while (! ((1==0) && (1==0))) {\n"
                                 "            read num3;\n"
                                 "        }\n"
                                 "\n"
                                 "        while (1+100  - 4 == 1 % 5 + 66) {\n"
                                 "            read num4;\n"
                                 "        }\n"
                                 "}\n";
				
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);
				
                // Query 1
                string query1 = "stmt sss;\n"
                                "Select sss";
                unordered_set<string> results1 = spaManager.query(query1);
                Assert::IsTrue(results1.size() == 8, L"Query 1 fails");
                // Expected results: 1, 2, 3, 4, 5, 6, 7, 8
                Assert::IsTrue(results1.find("1") != results1.end());
                Assert::IsTrue(results1.find("2") != results1.end());
                Assert::IsTrue(results1.find("3") != results1.end());
                Assert::IsTrue(results1.find("4") != results1.end());
                Assert::IsTrue(results1.find("5") != results1.end());
                Assert::IsTrue(results1.find("6") != results1.end());
                Assert::IsTrue(results1.find("7") != results1.end());
                Assert::IsTrue(results1.find("8") != results1.end());

                // Query 2
                string query2 = "stmt sss; assign saf; variable vvv;\n"
                                "Select sss pattern saf(vvv, _\"num2\"_) such that Modifies(sss, \"num1\")";
                unordered_set<string> results2 = spaManager.query(query2);
                Assert::IsTrue(results2.size() == 0, L"Query 2 fails");
                // Expected results: none

                // Query 3
                string query3 = "stmt sss;\n"
                                "Select sss such that Follows*(1, sss)";
                unordered_set<string> results3 = spaManager.query(query3);
                Assert::IsTrue(results3.size() == 3, L"Query 3 fails");
                // Expected results: 3, 5, 7
                Assert::IsTrue(results1.find("3") != results1.end());
                Assert::IsTrue(results1.find("5") != results1.end());
                Assert::IsTrue(results1.find("7") != results1.end());
            }
    };
}
