#include "stdafx.h"
#include "CppUnitTest.h"
#include <SPAManager/SPAManager.h>
#include <QueryProcessingSubsystem/Validator/SemanticException.h>
#include <SourceProcessor/exceptions/SimpleInvalidSyntaxException.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace IntegrationTesting {
    TEST_CLASS(TestQueryMilestone2) {
            public:
            TEST_METHOD(TestModifiesProcedure) {
                string program = "procedure main{\n"
                                 "        flag = 0;\n" // line 1
                                 "        call computeCentroid;\n" // line 2
                                 "        call printResults;\n" // line 3
                                 "}\n"
                                 "procedure readPoint{\n"
                                 "        read x;\n" // line 4
                                 "        read y;\n" // line 5
                                 "}\n"
                                 "procedure printResults{\n"
                                 "        print flag;\n" // line 6
                                 "        print cenX;\n" // line 7
                                 "        print cenY;\n" // line 8
                                 "        print normSq;\n" // line 9
                                 "}\n"
                                 "procedure computeCentroid{\n"
                                 "        x = y + 100 * 5;\n" // line 10
                                 "        count = 0;\n" // line 11
                                 "        cenX = 0;\n" // line 12
                                 "        cenY = 0;\n" // line 13
                                 "        call readPoint;\n" // line 14
                                 "        while ((x != 0) && (y != 0)) {\n" // line 15
                                 "            count = count + 1;\n" // line 16
                                 "            cenX = cenX + x;\n" // line 17
                                 "            cenY = cenY + y;\n" // line 18
                                 "            call readPoint;\n" // line 19
                                 "        }\n"
                                 "        if (count == 0) then {\n" // line 20
                                 "            flag = 1;\n" // line 21
                                 "        } else {\n"
                                 "            cenX = cenX / count;\n" // line 22
                                 "            cenY = cenY / count;\n" // line 23
                                 "        }\n"
                                 "        normSq = cenX * cenX + cenY * cenY;\n" // line 24
                                 "}";
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1
                string query1 = "procedure sppp; Select sppp such that Modifies(sppp, \"cenX\")";
                unordered_set<string> results1 = spaManager.query(query1);
                // Expected results: computeCentroid, main
                Assert::AreEqual(2, (int) results1.size(), L"Query 1 fails");
                Assert::IsTrue(results1.find("computeCentroid") != results1.end());
                Assert::IsTrue(results1.find("main") != results1.end());

                //! Query 2
                string query2 = "procedure p; Select p such that Modifies(p, \"x\")";
                unordered_set<string> results2 = spaManager.query(query2);
                // Expected results: computeCentroid, readPoint, main
                Assert::AreEqual(3, (int) results2.size(), L"Query 2 fails");
                Assert::IsTrue(results2.find("computeCentroid") != results2.end());
                Assert::IsTrue(results2.find("readPoint") != results2.end());
                Assert::IsTrue(results2.find("main") != results2.end());

                //! Query 3
                string query3 = "procedure p; variable v; Select p such that Modifies(p, v)";
                unordered_set<string> results3 = spaManager.query(query3);
                // Expected results: computeCentroid, readPoint, main
                Assert::AreEqual(3, (int) results3.size(), L"Query 3 fails");
                Assert::IsTrue(results3.find("computeCentroid") != results3.end());
                Assert::IsTrue(results3.find("readPoint") != results3.end());
                Assert::IsTrue(results3.find("main") != results3.end());

                //! Query 4
                string query4 = "procedure p; variable v; Select v such that Modifies(\"computeCentroid\", v)";
                unordered_set<string> results4 = spaManager.query(query4);
                // Expected results: cenX, cenY, count, x, y, flag, normSq
                Assert::AreEqual(7, (int) results4.size(), L"Query 4 fails");
                Assert::IsTrue(results4.find("cenX") != results4.end());
                Assert::IsTrue(results4.find("cenY") != results4.end());
                Assert::IsTrue(results4.find("count") != results4.end());
                Assert::IsTrue(results4.find("x") != results4.end());
                Assert::IsTrue(results4.find("y") != results4.end());
                Assert::IsTrue(results4.find("flag") != results4.end());
                Assert::IsTrue(results4.find("normSq") != results4.end());

                //! Query 5
                string query5 = "procedure p; variable v; Select v such that Modifies(\"readPoint\", v)";
                unordered_set<string> results5 = spaManager.query(query5);
                // Expected results: x, y
                Assert::AreEqual(2, (int) results5.size(), L"Query 5 fails");
                Assert::IsTrue(results5.find("x") != results5.end());
                Assert::IsTrue(results5.find("y") != results5.end());

                //! Query 6
                string query6 = "procedure p; variable v; Select v such that Modifies(\"main\", v)";
                unordered_set<string> results6 = spaManager.query(query6);
                // Expected results: flag, cenX, cenY, normSq, x, y, count
                Assert::AreEqual(7, (int) results6.size(), L"Query 6 fails");
                Assert::IsTrue(results6.find("flag") != results6.end());
                Assert::IsTrue(results6.find("cenX") != results6.end());
                Assert::IsTrue(results6.find("cenY") != results6.end());
                Assert::IsTrue(results6.find("normSq") != results6.end());
                Assert::IsTrue(results6.find("x") != results6.end());
                Assert::IsTrue(results6.find("y") != results6.end());
                Assert::IsTrue(results6.find("count") != results6.end());

                //! Query 7
                string query7 = "procedure p; variable v; call ccc; Select v such that Modifies(\"printResults\", v)";
                unordered_set<string> results7 = spaManager.query(query7);
                // Empty expected results
                Assert::AreEqual(0, (int) results7.size(), L"Query 7 fails");

                //! Query 8
                string query8 = "procedure p; variable v; call ccc; stmt ss; Select p such that Modifies(p, \"flag\")";
                unordered_set<string> results8 = spaManager.query(query8);
                // Expected results: main, computeCentroid
                Assert::AreEqual(2, (int) results8.size(), L"Query 8 fails");
                Assert::IsTrue(results8.find("main") != results8.end());
                Assert::IsTrue(results8.find("computeCentroid") != results8.end());

                //! Query 9
                string query9 = "procedure p; variable v; call ccc; stmt ss; Select p such that Modifies(p, _)";
                unordered_set<string> results9 = spaManager.query(query9);
                // Expected results: main, computeCentroid, readPoint
                Assert::AreEqual(3, (int) results9.size(), L"Query 9 fails");
                Assert::IsTrue(results9.find("main") != results9.end());
                Assert::IsTrue(results9.find("computeCentroid") != results9.end());
                Assert::IsTrue(results9.find("readPoint") != results9.end());

                //! Query 10
                string query10 = "procedure p; variable v; call ccc; stmt ss; Select p such that Modifies(p, \"y\")";
                unordered_set<string> results10 = spaManager.query(query10);
                // Expected results: main, computeCentroid, readPoint
                Assert::AreEqual(3, (int) results10.size(), L"Query 10 fails");
                Assert::IsTrue(results10.find("main") != results10.end());
                Assert::IsTrue(results10.find("computeCentroid") != results10.end());
                Assert::IsTrue(results10.find("readPoint") != results10.end());
            }

            TEST_METHOD(TestModifiesAndUsesStatement) {
                string program = "procedure main{\n"
                                 "        flag = 0;\n" // line 1
                                 "        call computeCentroid;\n" // line 2
                                 "        call printResults;\n" // line 3
                                 "}\n"
                                 "procedure readPoint{\n"
                                 "        read x;\n" // line 4
                                 "        read y;\n" // line 5
                                 "}\n"
                                 "procedure printResults{\n"
                                 "        print flag;\n" // line 6
                                 "        print cenX;\n" // line 7
                                 "        print cenY;\n" // line 8
                                 "        print normSq;\n" // line 9
                                 "}\n"
                                 "procedure computeCentroid{\n"
                                 "        x = y + 100 * 5;\n" // line 10
                                 "        count = 0;\n" // line 11
                                 "        cenX = 0;\n" // line 12
                                 "        cenY = 0;\n" // line 13
                                 "        call readPoint;\n" // line 14
                                 "        while ((x != 0) && (y != 0)) {\n" // line 15
                                 "            count = count + 1;\n" // line 16
                                 "            cenX = cenX + x;\n" // line 17
                                 "            cenY = cenY + y;\n" // line 18
                                 "            call readPoint;\n" // line 19
                                 "        }\n"
                                 "        if (count == 0) then {\n" // line 20
                                 "            flag = 1;\n" // line 21
                                 "        } else {\n"
                                 "            cenX = cenX / count;\n" // line 22
                                 "            cenY = cenY / count;\n" // line 23
                                 "        }\n"
                                 "        normSq = cenX * cenX + cenY * cenY;\n" // line 24
                                 "}";
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1
                string query1 = "procedure p; stmt s; Select s such that Uses(s, \"flag\")";
                unordered_set<string> results1 = spaManager.query(query1);
                // Expected results: 6, 3
                Assert::AreEqual(2, (int) results1.size(), L"Query 1 fails");
                Assert::IsTrue(results1.find("6") != results1.end());
                Assert::IsTrue(results1.find("3") != results1.end());

                //! Query 2
                string query2 = "procedure p; stmt s; Select s such that Modifies(s, \"x\")";
                unordered_set<string> results2 = spaManager.query(query2);
                // Expected results: 4, 10, 14, 19
                Assert::AreEqual(4, (int) results2.size(), L"Query 2 fails");
                Assert::IsTrue(results2.find("4") != results2.end());
                Assert::IsTrue(results2.find("10") != results2.end());
                Assert::IsTrue(results2.find("14") != results2.end());
                Assert::IsTrue(results2.find("19") != results2.end());

                //! Query 3
                string query3 = "procedure p; stmt s; call myC;\n"
                                "Select myC such that Modifies(myC, \"x\")";
                unordered_set<string> results3 = spaManager.query(query3);
                // Expected results: 14, 19
                Assert::AreEqual(2, (int) results3.size(), L"Query 3 fails");
                Assert::IsTrue(results3.find("14") != results3.end());
                Assert::IsTrue(results3.find("19") != results3.end());

                //! Query 4
                string query4 = "procedure p; stmt s; call myC;\n"
                                "Select myC such that Uses(myC, \"flag\")";
                unordered_set<string> results4 = spaManager.query(query4);
                // Expected results: 3
                Assert::AreEqual(1, (int) results4.size(), L"Query 4 fails");
                Assert::IsTrue(results4.find("3") != results4.end());
            }

            TEST_METHOD(TestUsesProcedure) {
                string program = "procedure procOne {\n"
                                 "        while (1>= 1%((1)) ) {\n"
                                 "            print num1;\n"
                                 "        }\n"
                                 "        while (1>= 1%((0-1)) ) {\n"
                                 "            print num2;\n"
                                 "        }\n"
                                 "        while (! ((1==0) && (1==0))) {\n"
                                 "            print num3;\n"
                                 "        }\n"
                                 "        while (1+100  - 4 == 1 % 5 + 66) {\n"
                                 "            print num4;\n"
                                 "        }\n"
                                 "}\n"
                                 "procedure procTwo {\n"
                                 "    while (! ((1==0) && (1==0))) {\n"
                                 "        print num5;\n"
                                 "    }\n"
                                 "}\n"
                                 "procedure nested {\n"
                                 "    if (iii >= 500) then {\n"
                                 "        while (1>= 1%((1) * kkk) ) {\n"
                                 "            while (!(kkk <= 111)) {\n"
                                 "                beingModified = num1 + num2;\n"
                                 "                if (yyy != 0) then {\n"
                                 "                    beingMod3 = 1 + k * (num1 + num2 * 5);\n"
                                 "                } else {\n"
                                 "                    call procTwo;\n"
                                 "                }\n"
                                 "            }\n"
                                 "            read num6;\n"
                                 "        }\n"
                                 "    } else {\n"
                                 "        while (1>= 1%((0-1)) ) {\n"
                                 "            read num7;\n"
                                 "        }\n"
                                 "    }\n"
                                 "}\n";
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1
                string query1 = "procedure p; variable v; Select p such that Uses(p, \"num1\")";
                unordered_set<string> results1 = spaManager.query(query1);
                // Expected results: procOne, nested
                Assert::AreEqual(2, (int) results1.size(), L"Query 1 fails");
                Assert::IsTrue(results1.find("procOne") != results1.end());
                Assert::IsTrue(results1.find("nested") != results1.end());

                //! Query 2
                string query2 = "procedure p; variable v; Select p such that Uses(p, \"num4\")";
                unordered_set<string> results2 = spaManager.query(query2);
                // Expected results: procOne
                Assert::AreEqual(1, (int) results2.size(), L"Query 2 fails");
                Assert::IsTrue(results2.find("procOne") != results2.end());

                //! Query 3
                string query3 = "procedure p; variable v; Select p such that Uses(p, \"num5\")";
                unordered_set<string> results3 = spaManager.query(query3);
                // Expected results: nested, procTwo
                Assert::AreEqual(2, (int) results3.size(), L"Query 3 fails");
                Assert::IsTrue(results3.find("nested") != results3.end());
                Assert::IsTrue(results3.find("procTwo") != results3.end());

                //! Query 4
                string query4 = "procedure p; variable v; Select v such that Uses(p, \"num6\")";
                unordered_set<string> results4 = spaManager.query(query4);
                // Empty expected results because there is no procedure that uses num6
                Assert::AreEqual(0, (int) results4.size(), L"Query 4 fails");

                //! Query 5
                string query5 = "procedure p; variable v; Select v such that Uses(p, \"num7\")";
                unordered_set<string> results5 = spaManager.query(query5);
                // Empty expected results because there is no procedure that uses num7
                Assert::AreEqual(0, (int) results5.size(), L"Query 5 fails");

                //! Query 6
                string query6 = "procedure p; variable v; Select v such that Uses(p, \"num8\")";
                unordered_set<string> results6 = spaManager.query(query6);
                // Empty expected results because there is no procedure that uses num8 (no such variable too)
                Assert::AreEqual(0, (int) results6.size(), L"Query 6 fails");

                //! Query 7
                string query7 = "procedure p; variable v; Select v such that Uses(\"procOne\", v)";
                unordered_set<string> results7 = spaManager.query(query7);
                // Expected results: num1, num2, num3, num4
                Assert::AreEqual(4, (int) results7.size(), L"Query 7 fails");
                Assert::IsTrue(results7.find("num1") != results7.end());
                Assert::IsTrue(results7.find("num2") != results7.end());
                Assert::IsTrue(results7.find("num3") != results7.end());
                Assert::IsTrue(results7.find("num4") != results7.end());

                //! Query 8
                string query8 = "procedure p; variable v, v1, v2; call c, c0; stmt ss, ss1;\n"
                                "Select v such that Uses(\"nested\", v)";
                unordered_set<string> results8 = spaManager.query(query8);
                // Expected results: num1, num2, num5, k, kkk, iii, yyy
                Assert::AreEqual(7, (int) results8.size(), L"Query 8 fails");
                Assert::IsTrue(results8.find("num1") != results8.end());
                Assert::IsTrue(results8.find("num2") != results8.end());
                Assert::IsTrue(results8.find("num5") != results8.end());
                Assert::IsTrue(results8.find("k") != results8.end());
                Assert::IsTrue(results8.find("kkk") != results8.end());
                Assert::IsTrue(results8.find("iii") != results8.end());
                Assert::IsTrue(results8.find("yyy") != results8.end());

                //! Query 9
                string query9 = "procedure p; variable v, v1, v2; call c, c0; stmt ss, ss1;\n"
                                "Select v such that Uses(\"procOne\", v)";
                unordered_set<string> results9 = spaManager.query(query9);
                // Expected results: num1, num2, num3, num4
                Assert::AreEqual(4, (int) results9.size(), L"Query 9 fails");
                Assert::IsTrue(results9.find("num1") != results9.end());
                Assert::IsTrue(results9.find("num2") != results9.end());
                Assert::IsTrue(results9.find("num3") != results9.end());
                Assert::IsTrue(results9.find("num4") != results9.end());

                //! Query 10
                string query10 = "procedure p; variable v, v1, v2; call c, c0; stmt ss, ss1;\n"
                                 "Select v such that Uses(\"procTwo\", v)";
                unordered_set<string> results10 = spaManager.query(query10);
                // Expected results: num5
                Assert::AreEqual(1, (int) results10.size(), L"Query 10 fails");
                Assert::IsTrue(results10.find("num5") != results10.end());
            }

            TEST_METHOD(TestUsesStatement) {
                string program = "procedure procOne {\n"
                                 "        while (1>= 1%((1)) ) {\n" // line 1
                                 "            print num1;\n"
                                 "        }\n"
                                 "        while (1>= 1%((0-1)) ) {\n" // line 3
                                 "            print num2;\n"
                                 "        }\n"
                                 "        while (! ((1==0) && (1==0))) {\n" // line 5
                                 "            print num3;\n"
                                 "        }\n"
                                 "        while (1+100  - 4 == 1 % 5 + 66) {\n" // line 7
                                 "            print num4;\n"
                                 "        }\n"
                                 "}\n"
                                 "procedure procTwo {\n"
                                 "    while (! ((1==0) && (1==0))) {\n" // line 9
                                 "        print num5;\n" // line 10
                                 "        print num5;\n" // line 11
                                 "    }\n"
                                 "}\n"
                                 "procedure nested {\n"
                                 "    if (iii >= 500) then {\n" // line 12
                                 "        while (1>= 1%((1) * kkk) ) {\n" // line 13
                                 "            while (!(kkk <= 111)) {\n" // line 14
                                 "                beingModified = num1 + num2;\n" // line 15
                                 "                if (yyy != 0) then {\n" // line 16
                                 "                    beingMod3 = 1 + k * (num1 + num2 * 5);\n" // line 17
                                 "                } else {\n"
                                 "                    call procTwo;\n" // line 18
                                 "                }\n"
                                 "            }\n"
                                 "            read num6;\n" // line 19
                                 "        }\n"
                                 "    } else {\n"
                                 "        while (1>= 1%((0-1)) ) {\n" // line 20
                                 "            read num7;\n" // line 21
                                 "        }\n"
                                 "    }\n"
                                 "}\t";
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1
                string query1 = "stmt s; variable v, v1;\n"
                                "Select s such that Uses(s, \"num1\")";
                unordered_set<string> results1 = spaManager.query(query1);
                // Expected results: 1, 2, 15, 14, 13, 12, 17, 16
                Assert::AreEqual(8, (int) results1.size(), L"Query 1 fails");
                Assert::IsTrue(results1.find("1") != results1.end());
                Assert::IsTrue(results1.find("2") != results1.end());
                Assert::IsTrue(results1.find("15") != results1.end());
                Assert::IsTrue(results1.find("14") != results1.end());
                Assert::IsTrue(results1.find("13") != results1.end());
                Assert::IsTrue(results1.find("12") != results1.end());
                Assert::IsTrue(results1.find("17") != results1.end());
                Assert::IsTrue(results1.find("16") != results1.end());

                //! Query 2
                string query2 = "while w, w1; variable v, v1;\n"
                                "Select w such that Uses(w, \"kkk\")";
                unordered_set<string> results2 = spaManager.query(query2);
                // Expected results: 13, 14
                Assert::AreEqual(2, (int) results2.size(), L"Query 2 fails");
                Assert::IsTrue(results2.find("13") != results2.end());
                Assert::IsTrue(results2.find("14") != results2.end());

                //! Query 3
                string query3 = "if i; variable v, v1;\n"
                                "Select i such that Uses(i, \"yyy\")";
                unordered_set<string> results3 = spaManager.query(query3);
                // Expected results: 16, 12
                Assert::AreEqual(2, (int) results3.size(), L"Query 3 fails");
                Assert::IsTrue(results3.find("16") != results3.end());
                Assert::IsTrue(results3.find("12") != results3.end());

                //! Query 4
                string query4 = "procedure p; variable v, v1; if i; while w;\n"
                                "Select w such that Uses(i, v)";
                unordered_set<string> results4 = spaManager.query(query4);
                // (basically all while loops) Expected results: 1, 3, 5, 7, 9, 13, 14, 20
                Assert::AreEqual(8, (int) results4.size(), L"Query 4 fails");
                Assert::IsTrue(results4.find("1") != results4.end());
                Assert::IsTrue(results4.find("3") != results4.end());
                Assert::IsTrue(results4.find("5") != results4.end());
                Assert::IsTrue(results4.find("7") != results4.end());
                Assert::IsTrue(results4.find("9") != results4.end());
                Assert::IsTrue(results4.find("13") != results4.end());
                Assert::IsTrue(results4.find("14") != results4.end());
                Assert::IsTrue(results4.find("20") != results4.end());

                //! Query 5
                string query5 = "procedure p; variable v, v1; if i; while w; stmt s;\n"
                                "Select s such that Uses(s, \"num5\")";
                unordered_set<string> results5 = spaManager.query(query5);
                // Expected results: 18, 9, 10, 11, 16, 14, 13, 12
                Assert::AreEqual(8, (int) results5.size(), L"Query 5 fails");
                Assert::IsTrue(results5.find("18") != results5.end());
                Assert::IsTrue(results5.find("9") != results5.end());
                Assert::IsTrue(results5.find("10") != results5.end());
                Assert::IsTrue(results5.find("11") != results5.end());
                Assert::IsTrue(results5.find("16") != results5.end());
                Assert::IsTrue(results5.find("14") != results5.end());
                Assert::IsTrue(results5.find("13") != results5.end());
                Assert::IsTrue(results5.find("12") != results5.end());

                //! Query 6
                string query6 = "procedure p; variable v, v1; if i; while w; assign a;\n"
                                "Select a such that Uses(a, \"k\")";
                unordered_set<string> results6 = spaManager.query(query6);
                // Expected results: 17
                Assert::AreEqual(1, (int) results6.size(), L"Query 6 fails");
                Assert::IsTrue(results6.find("17") != results6.end());
            }

            TEST_METHOD(TestCallsCallsTPart1) {
                string program = "procedure First {\n"
                                 "        read x;\n"
                                 "        read z;\n"
                                 "        call Second; }\n"
                                 "\n"
                                 "procedure Second {\n"
                                 "                x = 0;\n"
                                 "                i = 5;\n"
                                 "                while (i!=0) {\n"
                                 "                        x = x + 2*y;\n"
                                 "                        call Third;\n"
                                 "                        i = i - 1; }\n"
                                 "                if (x==1) then {\n"
                                 "                        x = x+1; }\n"
                                 "        else {\n"
                                 "                        z = 1; }\n"
                                 "                z = z + x + i;\n"
                                 "                y = z + 2;\n"
                                 "                x = x * y + z; }\n"
                                 "\n"
                                 "procedure Third {\n"
                                 "        z = 5;\n"
                                 "        v = z;\n"
                                 "        print v; }";
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1
                string query1 = "procedure p; variable v, v1;\n"
                                "Select p such that Calls(p, \"Second\")";
                unordered_set<string> results1 = spaManager.query(query1);
                // Expected results: First
                Assert::AreEqual(1, (int) results1.size(), L"Query 1 fails");
                Assert::IsTrue(results1.find("First") != results1.end());

                //! Query 2
                string query2 = "procedure p; variable v, v1;\n"
                                "Select p such that Calls(p, \"Third\")";
                unordered_set<string> results2 = spaManager.query(query2);
                // Expected results: Second
                Assert::AreEqual(1, (int) results2.size(), L"Query 2 fails");
                Assert::IsTrue(results2.find("Second") != results2.end());

                //! Query 3
                string query3 = "procedure p; variable v, v1;\n"
                                "Select p such that Calls(p, \"First\")";
                unordered_set<string> results3 = spaManager.query(query3);
                // Expected results: None
                Assert::AreEqual(0, (int) results3.size(), L"Query 3 fails");

                //! Query 4
                string query4 = "procedure p; variable v, v1;\n"
                                "Select p such that Calls(p, \"Fourth\")";
                unordered_set<string> results4 = spaManager.query(query4);
                // Expected results: None
                Assert::AreEqual(0, (int) results4.size(), L"Query 4 fails");

                //! Query 5
                string query5 = "procedure p; variable v, v1;\n"
                                "Select p such that Calls(\"Second\", p)";
                unordered_set<string> results5 = spaManager.query(query5);
                // Expected results: Third
                Assert::AreEqual(1, (int) results5.size(), L"Query 5 fails");
                Assert::IsTrue(results5.find("Third") != results5.end());

                //! Query 6
                string query6 = "procedure p; variable v, v1;\n"
                                "Select p such that Calls*(\"First\", p)";
                unordered_set<string> results6 = spaManager.query(query6);
                // Expected results: Second, Third
                Assert::AreEqual(2, (int) results6.size(), L"Query 6 fails");
                Assert::IsTrue(results6.find("Second") != results6.end());
                Assert::IsTrue(results6.find("Third") != results6.end());

                //! Query 7
                string query7 = "procedure p; variable v, v1;\n"
                                "Select p such that Calls*(p, \"Third\")";
                unordered_set<string> results7 = spaManager.query(query7);
                // Expected results: First, Second
                Assert::AreEqual(2, (int) results7.size(), L"Query 7 fails");
                Assert::IsTrue(results7.find("First") != results7.end());
                Assert::IsTrue(results7.find("Second") != results7.end());

                //! Query 8 - expecting semantic error
                string query8 = "procedure p; variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select s such that Calls*(s, _)";
                try {
                    unordered_set<string> results8 = spaManager.query(query8);
                    Assert::Fail(L"Expected a semantic exception");
                } catch (SemanticError &e) {
                    Logger::WriteMessage("Query 8 (Correct) Exception seen: ");
                    Logger::WriteMessage(e.what());
                }

                //! Query 9 - expecting semantic error
                string query9 = "procedure p; variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select s such that Calls*(\"First\", s)";
                try {
                    unordered_set<string> results9 = spaManager.query(query9);
                    Assert::Fail(L"Expected a semantic exception");
                } catch (SemanticError &e) {
                    Logger::WriteMessage("Query 9 (Correct) Exception seen: ");
                    Logger::WriteMessage(e.what());
                }

                //! Query 10 - expecting semantic error
                string query10 = "procedure p; variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select s such that Calls*(s, \"First\")";
                try {
                    unordered_set<string> results10 = spaManager.query(query10);
                    Assert::Fail(L"Expected a semantic exception");
                } catch (SemanticError &e) {
                    Logger::WriteMessage("Query 10 (Correct) Exception seen: ");
                    Logger::WriteMessage(e.what());
                }
            }

            TEST_METHOD(TestCallsCallsTPart2) {
                string program = "procedure procOne {\n"
                                 "        while (1>= 1%((1)) ) {\n" // line 1
                                 "            print num1;\n"
                                 "        }\n"
                                 "        while (1>= 1%((0-1)) ) {\n" // line 3
                                 "            print num2;\n"
                                 "        }\n"
                                 "        while (! ((1==0) && (1==0))) {\n" // line 5
                                 "            print num3;\n"
                                 "        }\n"
                                 "        while (1+100  - 4 == 1 % 5 + 66) {\n" // line 7
                                 "            print num4;\n"
                                 "        }\n"
                                 "}\n"
                                 "procedure procTwo {\n"
                                 "    while (! ((1==0) && (1==0))) {\n" // line 9
                                 "        print num5;\n" // line 10
                                 "        call procOne;\n" // line 11
                                 "    }\n"
                                 "}\n"
                                 "procedure nested {\n"
                                 "    if (iii >= 500) then {\n" // line 12
                                 "        while (1>= 1%((1) * kkk) ) {\n" // line 13
                                 "            while (!(kkk <= 111)) {\n" // line 14
                                 "                beingModified = num1 + num2;\n" // line 15
                                 "                if (yyy != 0) then {\n" // line 16
                                 "                    beingMod3 = 1 + k * (num1 + num2 * 5);\n" // line 17
                                 "                } else {\n"
                                 "                    call procTwo;\n" // line 18
                                 "                }\n"
                                 "            }\n"
                                 "            read num6;\n" // line 19
                                 "        }\n"
                                 "    } else {\n"
                                 "        while (1>= 1%((0-1)) ) {\n" // line 20
                                 "            read num7;\n" // line 21
                                 "        }\n"
                                 "    }\n"
                                 "}\t";
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1
                string query1 = "procedure p; variable v, v1;\n"
                                "Select p such that Calls*(p, \"procOne\")";
                unordered_set<string> results1 = spaManager.query(query1);
                // Expected results: procTwo, nested
                Assert::AreEqual(2, (int) results1.size(), L"Query 1 fails");
                Assert::IsTrue(results1.find("procTwo") != results1.end());
                Assert::IsTrue(results1.find("nested") != results1.end());

                //! Query 2
                string query2 = "procedure p; variable v, v1;\n"
                                "Select p such that Calls*(\"procOne\", p)";
                unordered_set<string> results2 = spaManager.query(query2);
                // Empty expected results
                Assert::AreEqual(0, (int) results2.size(), L"Query 2 fails");

                //! Query 3
                string query3 = "procedure p; variable v, v1;\n"
                                "Select p such that Calls*(p, \"procTwo\")";
                unordered_set<string> results3 = spaManager.query(query3);
                // Expected results: nested
                Assert::AreEqual(1, (int) results3.size(), L"Query 3 fails");
                Assert::IsTrue(results3.find("nested") != results3.end());

                //! Query 4
                string query4 = "procedure p; variable v, v1;\n"
                                "Select p such that Calls*(\"procTwo\", p)";
                unordered_set<string> results4 = spaManager.query(query4);
                // Expected results: procOne
                Assert::AreEqual(1, (int) results4.size(), L"Query 4 fails");
                Assert::IsTrue(results4.find("procOne") != results4.end());

                //! Query 5
                string query5 = "procedure p; variable v, v1;\n"
                                "Select p such that Calls*(p, \"nested\")";
                unordered_set<string> results5 = spaManager.query(query5);
                // Empty expected results
                Assert::AreEqual(0, (int) results5.size(), L"Query 5 fails");

                //! Query 6
                string query6 = "procedure p; variable v, v1;\n"
                                "Select p such that Calls*(\"nested\", p)";
                unordered_set<string> results6 = spaManager.query(query6);
                // Expected results: procOne, procTwo
                Assert::AreEqual(2, (int) results6.size(), L"Query 6 fails");
                Assert::IsTrue(results6.find("procOne") != results6.end());
                Assert::IsTrue(results6.find("procTwo") != results6.end());

                //! Query 7
                string query7 = "procedure p; variable v, v1;\n"
                                "Select p such that Calls(\"nested\", p)";
                unordered_set<string> results7 = spaManager.query(query7);
                // Expected results: procTwo
                Assert::AreEqual(1, (int) results7.size(), L"Query 7 fails");
                Assert::IsTrue(results7.find("procTwo") != results7.end());
            }

            TEST_METHOD(TestPatternMatchFull) {
                string program = "procedure read {\n"
                                 "    mod1 = print % (var1 + 1 * var2 - var3);\n" // line 1
                                 "    if (var1 - 6 % 1 == (var2 + 1) * 2) then {\n" // line 2
                                 "        mod2 = var1 + 1 * 100 - var2;\n" // line 3
                                 "    } else {\n"
                                 "        var2 = var2 + 1 % 100 - 30 + 60 * 2 / mod2;\n" // line 4
                                 "    }\n"
                                 "}\n"
                                 "procedure call {\n"
                                 "    mod3 = var1 + 1 * 100 - var2;\n" // line 5
                                 "    call read1;\n" // line 6
                                 "    mod4 = var1 + 1 * 100 - var2;\n" // line 7
                                 "}";
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1
                string query1 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Modifies(aa, v) pattern aa(v, \"var1\")";
                unordered_set<string> results1 = spaManager.query(query1);
                // Expected results: 1, 3, 5, 7
                Assert::AreEqual(4, (int) results1.size(), L"Query 1 fails");
                Assert::IsTrue(results1.find("1") != results1.end());
                Assert::IsTrue(results1.find("3") != results1.end());
                Assert::IsTrue(results1.find("5") != results1.end());
                Assert::IsTrue(results1.find("7") != results1.end());

                //! Query 2
                string query2 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Modifies(aa, v) pattern aa(v, _)";
                unordered_set<string> results2 = spaManager.query(query2);
                // Expected results: 1, 3, 4, 5, 7
                Assert::AreEqual(5, (int) results2.size(), L"Query 2 fails");
                Assert::IsTrue(results2.find("1") != results2.end());
                Assert::IsTrue(results2.find("3") != results2.end());
                Assert::IsTrue(results2.find("4") != results2.end());
                Assert::IsTrue(results2.find("5") != results2.end());
                Assert::IsTrue(results2.find("7") != results2.end());

                //! Query 3
                string query3 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Modifies(aa, v) pattern aa(v, \"var2 -var3\")";
                unordered_set<string> results3 = spaManager.query(query3);
                // Empty expected results
                Assert::AreEqual(0, (int) results3.size(), L"Query 3 fails");

                //! Query 4
                string query4 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Modifies(aa, v) pattern aa(v, \"print % (var1 + (1 * var2) - var3)\")";
                unordered_set<string> results4 = spaManager.query(query4);
                // Expected results: 1
                Assert::AreEqual(1, (int) results4.size(), L"Query 4 fails");
                Assert::IsTrue(results4.find("1") != results4.end());

                //! Query 5
                string query5 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Modifies(aa, v) pattern aa(v, \"(  (var1 + (1 * 100))  ) - var2\")";
                unordered_set<string> results5 = spaManager.query(query5);
                // Expected results: 3, 5, 7
                Assert::AreEqual(3, (int) results5.size(), L"Query 5 fails");
                Assert::IsTrue(results5.find("3") != results5.end());
                Assert::IsTrue(results5.find("5") != results5.end());
                Assert::IsTrue(results5.find("7") != results5.end());

                //! Query 6
                string query6 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Modifies(aa, v) pattern aa(v, \"var1 + 1 * 100 - var2\")";
                unordered_set<string> results6 = spaManager.query(query6);
                // Expected results: 3, 5, 7
                Assert::AreEqual(3, (int) results6.size(), L"Query 6 fails");
                Assert::IsTrue(results6.find("3") != results6.end());
                Assert::IsTrue(results6.find("5") != results6.end());
                Assert::IsTrue(results6.find("7") != results6.end());

                //! Query 7
                string query7 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Modifies(aa, v) pattern aa(\"var2\", \"var2 + 1 % 100 - 30 + 60 * 2 / mod2\")";
                unordered_set<string> results7 = spaManager.query(query7);
                // Expected results: 4
                Assert::AreEqual(1, (int) results7.size(), L"Query 7 fails");
                Assert::IsTrue(results7.find("4") != results7.end());

                //! Query 8
                string query8 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Modifies(aa, v) pattern aa(\"mod4\", _)";
                unordered_set<string> results8 = spaManager.query(query8);
                // Expected results: 7
                Assert::AreEqual(1, (int) results8.size(), L"Query 8 fails");
                Assert::IsTrue(results8.find("7") != results8.end());

                //! Query 9
                string query9 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Modifies(aa, v) pattern aa(\"var2\", \"var2 + (1 % 100) - 30 + (60 * 2) / mod2\")";
                unordered_set<string> results9 = spaManager.query(query9);
                // Expected results: 4
                Assert::AreEqual(1, (int) results9.size(), L"Query 9 fails");
                Assert::IsTrue(results9.find("4") != results9.end());

                //! Query 10
                string query10 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Modifies(aa, v) pattern aa(\"var2\", \"var2 + (1 % 100) - 30 + (60 ** 2) / mod2\")";
                try {
                    unordered_set<string> results10 = spaManager.query(query10);
                    Assert::Fail(L"Expected SimpleInvalidSyntaxException query 10");
                } catch (SimpleInvalidSyntaxException& simpleInvalidSyntaxFromParser) {
                    Logger::WriteMessage("Query 10 (Correct) Exception seen: ");
                    Logger::WriteMessage(simpleInvalidSyntaxFromParser.what());
                }

                //! Query 11
                string query11 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Modifies(aa, v) pattern aa(\"var2\", \"var2 && (1 % 100) - 30 + (60 * 2) / mod2\")";
                try {
                    unordered_set<string> results11 = spaManager.query(query11);
                    Assert::Fail(L"Expected SimpleInvalidSyntaxException query 11");
                } catch (SimpleInvalidSyntaxException& simpleInvalidSyntaxFromParser) {
                    Logger::WriteMessage("Query 11 (Correct) Exception seen: ");
                    Logger::WriteMessage(simpleInvalidSyntaxFromParser.what());
                }
            }

            TEST_METHOD(TestPatternMatchPartial) {
                string program = "procedure read {\n"
                                 "    mod1 = print % (var1 + 1 * var2 - var3);\n" // line 1
                                 "    if (var1 - 6 % 1 == (var2 + 1) * 2) then {\n" // line 2
                                 "        mod2 = var1 + 1 * 100 - var2;\n" // line 3
                                 "    } else {\n"
                                 "        var2 = var2 + 1 % 100 - 30 + 60 * 2 / mod2;\n" // line 4
                                 "    }\n"
                                 "}\n"
                                 "procedure call {\n"
                                 "    mod3 = var1 + 1 * 100 - var2;\n" // line 5
                                 "    call read1;\n" // line 6
                                 "    mod4 = var1 + 1 * 100 - var2;\n" // line 7
                                 "    mod4 = 1;\n" // line 8
                                 "}";
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1
                string query1 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Uses(aa, v) pattern aa(v, _)";
                unordered_set<string> results1 = spaManager.query(query1);
                // Expected results: 1, 3, 5, 7
                Assert::AreEqual(4, (int) results1.size(), L"Query 1 fails");
                Assert::IsTrue(results1.find("1") != results1.end());
                Assert::IsTrue(results1.find("3") != results1.end());
                Assert::IsTrue(results1.find("5") != results1.end());
                Assert::IsTrue(results1.find("7") != results1.end());

                //! Query 2
                string query2 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa pattern aa(v, _)";
                unordered_set<string> results2 = spaManager.query(query2);
                // Expected results: 1, 3, 5, 7, 8
                Assert::AreEqual(5, (int) results2.size(), L"Query 2 fails");
                Assert::IsTrue(results2.find("1") != results2.end());
                Assert::IsTrue(results2.find("3") != results2.end());
                Assert::IsTrue(results2.find("5") != results2.end());
                Assert::IsTrue(results2.find("7") != results2.end());
                Assert::IsTrue(results2.find("8") != results2.end());

                //! Query 3
                string query3 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Uses(aa, v) pattern aa(v, _\"var2\"_)";
                unordered_set<string> results3 = spaManager.query(query3);
                // Expected results: 1, 3, 4, 5, 7
                Assert::AreEqual(5, (int) results3.size(), L"Query 3 fails");
                Assert::IsTrue(results3.find("1") != results3.end());
                Assert::IsTrue(results3.find("3") != results3.end());
                Assert::IsTrue(results3.find("4") != results3.end());
                Assert::IsTrue(results3.find("5") != results3.end());
                Assert::IsTrue(results3.find("7") != results3.end());

                //! Query 4
                string query4 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Uses(aa, v) pattern aa(v, _\"mod2\"_)";
                unordered_set<string> results4 = spaManager.query(query4);
                // Expected results: 4
                Assert::AreEqual(1, (int) results4.size(), L"Query 4 fails");
                Assert::IsTrue(results4.find("4") != results4.end());

                //! Query 5
                string query5 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Uses(aa, v) pattern aa(v, _\"var3\"_)";
                unordered_set<string> results5 = spaManager.query(query5);
                // Expected results: 1
                Assert::AreEqual(1, (int) results5.size(), L"Query 5 fails");
                Assert::IsTrue(results5.find("1") != results5.end());

                //! Query 6
                string query6 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Uses(aa, v) pattern aa(v, _\"var1 +   (1) *     100\"_)";
                unordered_set<string> results6 = spaManager.query(query6);
                // Expected results: 3, 5, 7
                Assert::AreEqual(3, (int) results6.size(), L"Query 6 fails");
                Assert::IsTrue(results6.find("3") != results6.end());
                Assert::IsTrue(results6.find("5") != results6.end());
                Assert::IsTrue(results6.find("7") != results6.end());

                //! Query 7
                string query7 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Uses(aa, v) pattern aa(v, _\"1 % 100\"_)";
                unordered_set<string> results7 = spaManager.query(query7);
                // Expected results: 4
                Assert::AreEqual(1, (int) results7.size(), L"Query 7 fails");
                Assert::IsTrue(results7.find("4") != results7.end());

                //! Query 8
                string query8 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Uses(aa, v) pattern aa(v, _\"1 % 100 - 30\"_)";
                unordered_set<string> results8 = spaManager.query(query8);
                // Empty expected results
                Assert::AreEqual(0, (int) results8.size(), L"Query 8 fails");

                //! Query 9
                string query9 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Uses(aa, v) pattern aa(v, _\"60 * 2 / mod2\"_)";
                unordered_set<string> results9 = spaManager.query(query9);
                // Expected results: 4
                Assert::AreEqual(1, (int) results9.size(), L"Query 9 fails");
                Assert::IsTrue(results9.find("4") != results9.end());

                //! Query 10
                string query10 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Uses(aa, v) pattern aa(v, _\"var2 + 1 % 100 - 30 + 60\"_)";
                unordered_set<string> results10 = spaManager.query(query10);
                // Empty expected results
                Assert::AreEqual(0, (int) results10.size(), L"Query 10 fails");

                //! Query 11
                string query11 = "variable v, v1; stmt s; assign aa; if ii; while www, w1;\n"
                                "Select aa such that Uses(aa, v) pattern aa(v, _\"var2 + 1 % 100 - 30\"_)";
                unordered_set<string> results11 = spaManager.query(query11);
                // Expected results: 4
                Assert::AreEqual(1, (int) results11.size(), L"Query 11 fails");
                Assert::IsTrue(results11.find("4") != results11.end());
            }
    };
}
