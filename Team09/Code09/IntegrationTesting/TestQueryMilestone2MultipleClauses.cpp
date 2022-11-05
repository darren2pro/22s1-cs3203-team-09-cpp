#include "stdafx.h"
#include "CppUnitTest.h"
#include <SPAManager/SPAManager.h>
#include <QueryProcessingSubsystem/Validator/SemanticException.h>
#include <SourceProcessor/exceptions/SimpleInvalidSyntaxException.h>
#include <chrono>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace chrono;

namespace IntegrationTesting {
    TEST_CLASS(TestQueryMilestone2MultipleClauses) {
            string getCurrentProgram(int ref) {
                string program1 = "procedure procOne {\n"
                                  "        while (1>= 1%((1)) ) {\n" // line 1
                                  "            print num1;\n" // line 2
                                  "        }\n"
                                  "        while (1>= 1%((0-1)) ) {\n" // line 3
                                  "            print num2;\n" // line 4
                                  "        }\n"
                                  "        while (! ((1==0) && (1==0))) {\n" // line 5
                                  "            print num3;\n" // line 6
                                  "        }\n"
                                  "        while (1+100  - 4 == 1 % 5 + 66) {\n" // line 7
                                  "            print num4;\n" // line 8
                                  "        }\n"
                                  "}\n"
                                  "procedure procTwo {\n"
                                  "    while (! ((1==0) && (1==0))) {\n" // line 9
                                  "        print num5;\n" // line 10
                                  "    }\n"
                                  "}\n"
                                  "procedure nested {\n"
                                  "    if (iii >= 500) then {\n" // line 11
                                  "        while (1>= 1%((1) * kkk) ) {\n" // line 12
                                  "            while (!(kkk <= 111)) {\n" // line 13
                                  "                beingModified = num1 + num2;\n" // line 14
                                  "                if (yyy != 0) then {\n" // line 15
                                  "                    beingMod3 = 1 + k * (num1 + num2 * 5);\n" // line 16
                                  "                } else {\n"
                                  "                    call procTwo;\n" // line 17
                                  "                }\n"
                                  "            }\n"
                                  "            read num6;\n" // line 18
                                  "        }\n"
                                  "    } else {\n"
                                  "        while (1>= 1%((0-1)) ) {\n" // line 19
                                  "            read num7;\n" // line 20
                                  "        }\n"
                                  "    }\n"
                                  "}\n";

                string program2 = "procedure main{\n"
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
                switch (ref) {
                    case 1:
                        return program1;
                    case 2:
                        return program2;
                    default:
                        return "";
                }
            }

            public:
            TEST_METHOD(TestMultipleClausesNoAndPartOne1) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1
                string query1 = "assign a; \n"
                                "Select a such that Modifies(a, \"beingModified\") such that Uses(a, \"num1\")";
                unordered_set<string> queryResults1 = spaManager.query(query1);
                // Expected results: 14
                Assert::AreEqual(1, (int) queryResults1.size());
                Assert::IsTrue(queryResults1.find("14") != queryResults1.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne2) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 2
                string query2 = "assign a; \n"
                                "Select a such that Uses(a, \"num1\") such that Modifies(a, \"beingMod3\")";
                unordered_set<string> queryResults2 = spaManager.query(query2);
                // Expected results: 16
                Assert::AreEqual(1, (int) queryResults2.size());
                Assert::IsTrue(queryResults2.find("16") != queryResults2.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne3) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 3
                string query3 = "assign a; \n"
                                "Select a such that Uses(a, \"num1\") such that Uses(a, \"num2\")";
                unordered_set<string> queryResults3 = spaManager.query(query3);
                // Expected results: 14, 16
                Assert::AreEqual(2, (int) queryResults3.size());
                Assert::IsTrue(queryResults3.find("14") != queryResults3.end());
                Assert::IsTrue(queryResults3.find("16") != queryResults3.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne4) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 4
                string query4 = "assign a; \n"
                                "Select a such that Uses(a, \"num1\") such that Uses(a, \"num2\") such that Uses(a, \"num3\")";
                unordered_set<string> queryResults4 = spaManager.query(query4);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults4.size());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne5) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 5
                string query5 = "assign a; stmt s;\n"
                                "Select s such that Uses(s, \"num1\") such that Uses(s, \"num2\") such that Uses(s, \"k\")";
                unordered_set<string> queryResults5 = spaManager.query(query5);
                // Expected results: 11, 12, 13, 15, 16
                Assert::AreEqual(5, (int) queryResults5.size());
                Assert::IsTrue(queryResults5.find("11") != queryResults5.end());
                Assert::IsTrue(queryResults5.find("12") != queryResults5.end());
                Assert::IsTrue(queryResults5.find("13") != queryResults5.end());
                Assert::IsTrue(queryResults5.find("15") != queryResults5.end());
                Assert::IsTrue(queryResults5.find("16") != queryResults5.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne6) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 6
                string query6 = "assign a; stmt s; procedure p; \n"
                                "Select p such that Uses(p, \"num1\") such that Uses(p, \"num2\") such that Uses(p, \"k\")";

                auto start = high_resolution_clock::now();
                unordered_set<string> queryResults6 = spaManager.query(query6);
                auto stop = high_resolution_clock::now();
                auto durationOptimized = duration_cast<microseconds>(stop - start);

                start = high_resolution_clock::now();
                unordered_set<string> queryResults6Unoptimized = spaManager.query(query6);
                stop = high_resolution_clock::now();
                auto durationUnoptimized = duration_cast<microseconds>(stop - start);

                Logger::WriteMessage("\tTestMultipleClausesNoAndPartOne6");
                Logger::WriteMessage(("\tOptimized: " + to_string(durationOptimized.count()) + " microseconds").c_str());
                Logger::WriteMessage(("\tUnoptimized: " + to_string(durationUnoptimized.count()) + " microseconds\n\n").c_str());

                // Expected results: nested
                Assert::AreEqual(1, (int) queryResults6.size());
                Assert::IsTrue(queryResults6.find("nested") != queryResults6.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne7) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 7
                string query7 = "assign a; stmt s; procedure p; \n"
                                "Select s such that Uses(p, \"num100\") such that Modifies(s, \"num6\")";
                unordered_set<string> queryResults7 = spaManager.query(query7);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults7.size());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne8) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 8
                string query8 = "assign a; stmt s, anotherS; procedure p; \n"
                                "Select s such that Uses(anotherS, \"num1\") such that Modifies(s, \"num6\")";
                unordered_set<string> queryResults8 = spaManager.query(query8);
                // Expected results: 11, 12, 18
                Assert::AreEqual(3, (int) queryResults8.size());
                Assert::IsTrue(queryResults8.find("11") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("12") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("18") != queryResults8.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne9) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 9
                string query9 = "assign a; stmt parentS, childS; procedure p; \n"
                                "Select parentS such that Parent*(parentS, childS) such that Modifies(childS, \"num6\")";
                unordered_set<string> queryResults9 = spaManager.query(query9);
                // Expected results: 12, 11
                Assert::AreEqual(2, (int) queryResults9.size());
                Assert::IsTrue(queryResults9.find("12") != queryResults9.end());
                Assert::IsTrue(queryResults9.find("11") != queryResults9.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne10) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 10 - similar to 9 but additional pattern for assign. Should not affect results.
                string query10 = "assign a; stmt parentS, childS; procedure p; \n"
                                 "Select parentS such that Parent(7, 8) and Parent*(parentS, childS) such that Modifies(childS, \"num6\") pattern a(\"beingModified\", \"num1  +   num2\") and a(\"beingModified\", _\"(num1)\"_)";
                unordered_set<string> queryResults10 = spaManager.query(query10);
                // Expected results: 12, 11
                Assert::AreEqual(2, (int) queryResults10.size());
                Assert::IsTrue(queryResults10.find("12") != queryResults10.end());
                Assert::IsTrue(queryResults10.find("11") != queryResults10.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne11) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 11 - similar to 10 but it has such that with pattern. Get all parentS that has a childS that modifies num7, and has another childA which modifies beingModified.
                string query11 = "assign childA; stmt parentS, childS; procedure p; \n"
                                 "Select parentS such that Parent*(parentS, childS) such that Modifies(childS, \"num7\") pattern childA(\"beingModified\", \"num1  +   num2\") such that Parent*(parentS, childA)";
                unordered_set<string> queryResults11 = spaManager.query(query11);
                // Expected results: 11
                Assert::AreEqual(1, (int) queryResults11.size());
                Assert::IsTrue(queryResults11.find("11") != queryResults11.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne12) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 12 - get call statements which calls something, and also uses num5
                string query12 = "call c; stmt s; procedure p1, p2; \n"
                                 "Select c such that Calls(p1, _) such that Uses(c, \"num5\")";
                unordered_set<string> queryResults12 = spaManager.query(query12);
                // Expected results: 17
                Assert::AreEqual(1, (int) queryResults12.size());
                Assert::IsTrue(queryResults12.find("17") != queryResults12.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne13) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 13 - get procedures that uses num1, num2 and num3
                string query13 = "procedure p; \n"
                                 "Select p such that Uses(p, \"num1\") such that Uses(p, \"num2\") such that Uses(p, \"num3\")";
                unordered_set<string> queryResults13 = spaManager.query(query13);
                // Expected results: procOne
                Assert::AreEqual(1, (int) queryResults13.size());
                Assert::IsTrue(queryResults13.find("procOne") != queryResults13.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne14) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 14 - get procedures that uses num1, num2, and num5
                string query14 = "procedure p; \n"
                                 "Select p such that Uses(p, \"num1\") such that Uses(p, \"num2\") such that Uses(p, \"num5\")";
                unordered_set<string> queryResults14 = spaManager.query(query14);
                // Expected results: nested
                Assert::AreEqual(1, (int) queryResults14.size());
                Assert::IsTrue(queryResults14.find("nested") != queryResults14.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne15) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 15 - get procedures such that <something true>
                string query15 = "procedure p, p1; call c; stmt s;  read r; \n"
                                 "Select p such that Calls(p, p1) such that Modifies(r, \"num7\") such that Modifies(p, \"num7\")   ";
                unordered_set<string> queryResults15 = spaManager.query(query15);
                // Expected results: nested
                Assert::AreEqual(1, (int) queryResults15.size());
                Assert::IsTrue(queryResults15.find("nested") != queryResults15.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne16) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 16 - get procedures such that <something false>
                string query16 = "procedure p, p1; call c; stmt s;  read r; assign a; variable v;\n"
                                 "Select p such that Calls(_, _) pattern  a(_, _\"num1 + num9\"_)  ";
                unordered_set<string> queryResults16 = spaManager.query(query16);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults16.size());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne17) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 17 - multiple pattern clauses
                string query17 = "procedure p, p1; call c; stmt s;  read r; assign a, a1; variable v;\n"
                                 "Select a1 such that Calls(p, p1) pattern  a(v, _)  pattern a1(_, _\"num2\"_)  ";
                unordered_set<string> queryResults17 = spaManager.query(query17);
                // Expected results: 14, 16
                Assert::AreEqual(2, (int) queryResults17.size());
                Assert::IsTrue(queryResults17.find("14") != queryResults17.end());
                Assert::IsTrue(queryResults17.find("16") != queryResults17.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne18) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 18 - multiple pattern clauses with Next/*. Once Next is supported, this test case should pass.
                string query18 = "procedure p, p1; call c; stmt s;  read r; assign a, a1; variable v;\n"
                                 "Select s such that Calls(p, p1) pattern  a(v, _)  pattern a1(_, _\"num2\"_)  such that Next(a1, s)  ";
                unordered_set<string> queryResults18 = spaManager.query(query18);
                // Expected results: 15, 13
                Assert::AreEqual(2, (int) queryResults18.size());
                Assert::IsTrue(queryResults18.find("15") != queryResults18.end());

                Assert::IsTrue(queryResults18.find("13") != queryResults18.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne19) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 19 - multiple pattern clauses with Next/*. Once Next is supported, this test case should pass.
                string query19 = "procedure p, p1; call c; stmt s;  read r; assign a, a1; variable v;\n"
                                 "Select v pattern a(v, _) pattern a1(_, _\"num2\"_) such that Next*(11, a)";
                unordered_set<string> queryResults19 = spaManager.query(query19);
                // Expected results: beingModified, beingMod3
                Assert::AreEqual(2, (int) queryResults19.size());
                Assert::IsTrue(queryResults19.find("beingModified") != queryResults19.end());
                Assert::IsTrue(queryResults19.find("beingMod3") != queryResults19.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne20) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 20 - multiple pattern clauses with Next/*. Select read stmts that can come after 11. Once Next is supported, this test case should pass.
                string query20 = "procedure p, p1; call c; stmt s;  read r; assign a, a1; variable v;\n"
                                 "Select r pattern a(v, _) pattern a1(_, _\"num2\"_) such that Next*(11, r)";
                unordered_set<string> queryResults20 = spaManager.query(query20);
                // Expected results: 18, 20
                Assert::AreEqual(2, (int) queryResults20.size());
                Assert::IsTrue(queryResults20.find("18") != queryResults20.end());
                Assert::IsTrue(queryResults20.find("20") != queryResults20.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartOne21) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 21 - Same as query 20 but with num22. Once Next is supported, this test case should pass.
                string query21 = "procedure p, p1; call c; stmt s;  read r; assign a, a1; variable v;\n"
                                 "Select r pattern a(v, _) pattern a1(_, _\"num22\"_) such that Next*(11, r)";
                unordered_set<string> queryResults21 = spaManager.query(query21);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults21.size());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartTwo1) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1 - Multiple pattern clauses on the same while loop
                string query1 = "procedure p; while w; variable v; assign a; \n"
                                "Select w pattern a(v, _) pattern a(_, _\"cenY\"_) pattern w(\"y\", _) pattern w(\"x\", _)";
                unordered_set<string> queryResults1 = spaManager.query(query1);
                // Expected results: 15
                Assert::AreEqual(1, (int) queryResults1.size());
                Assert::IsTrue(queryResults1.find("15") != queryResults1.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartTwo2) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 2 - Multiple pattern clauses on the same while loop. Once if-pattern is supported, this test case should pass.
                string query2 = "procedure p; while w; variable v; assign a; if ifs;\n"
                                "Select ifs pattern a(v, _) pattern a(_, _\"cenY\"_) pattern ifs(\"count\", _, _)";
                unordered_set<string> queryResults2 = spaManager.query(query2);
                // Expected results: 20
                Assert::AreEqual(1, (int) queryResults2.size());
                Assert::IsTrue(queryResults2.find("20") != queryResults2.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartTwo3) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 3 - Pattern for while and also checking follows and parent. Once if-pattern is supported, this test case should pass.
                string query3 = "procedure p; while w; variable v; assign a; if ifs;\n"
                                "Select w pattern ifs(\"count\", _, _) such that Follows(w, ifs)";
                unordered_set<string> queryResults3 = spaManager.query(query3);
                // Expected results: 15
                Assert::AreEqual(1, (int) queryResults3.size());
                Assert::IsTrue(queryResults3.find("15") != queryResults3.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartTwo4) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 4 - Pattern for while and checking for next. Once if-pattern is supported, this test case should pass.
                string query4 = "procedure p; while w; variable v; assign a; if ifs;\n"
                                "Select w pattern ifs(\"count\", _, _) such that Next(w, ifs)";
                unordered_set<string> queryResults4 = spaManager.query(query4);
                // Expected results: 15
                Assert::AreEqual(1, (int) queryResults4.size());
                Assert::IsTrue(queryResults4.find("15") != queryResults4.end());
            }

            TEST_METHOD(TestMultipleClausesNoAndPartTwo5) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 5 - Pattern for while and checking for next*. Once if-pattern is supported, this test case should pass.
                string query5 = "procedure p; while w; variable v; assign a; if ifs;\n"
                                "Select w pattern ifs(\"count\", _, _) such that Next*(w, ifs)";
                unordered_set<string> queryResults5 = spaManager.query(query5);
                // Expected results: 15
                Assert::AreEqual(1, (int) queryResults5.size());
                Assert::IsTrue(queryResults5.find("15") != queryResults5.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne1) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1
                string query1 = "assign a; \n"
                                "Select a such that Modifies(a, \"beingModified\") and Uses(a, \"num1\")";
                unordered_set<string> queryResults1 = spaManager.query(query1);
                // Expected results: 14
                Assert::AreEqual(1, (int) queryResults1.size());
                Assert::IsTrue(queryResults1.find("14") != queryResults1.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne2) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 2
                string query2 = "assign a; \n"
                                "Select a such that Uses(a, \"num1\") and Modifies(a, \"beingMod3\")";
                unordered_set<string> queryResults2 = spaManager.query(query2);
                // Expected results: 16
                Assert::AreEqual(1, (int) queryResults2.size());
                Assert::IsTrue(queryResults2.find("16") != queryResults2.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne3) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 3
                string query3 = "assign a; \n"
                                "Select a such that Uses(a, \"num1\") and Uses(a, \"num2\")";
                unordered_set<string> queryResults3 = spaManager.query(query3);
                // Expected results: 14, 16
                Assert::AreEqual(2, (int) queryResults3.size());
                Assert::IsTrue(queryResults3.find("14") != queryResults3.end());
                Assert::IsTrue(queryResults3.find("16") != queryResults3.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne4) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 4
                string query4 = "assign a; \n"
                                "Select a such that Uses(a, \"num1\") and Uses(a, \"num2\") and Uses(a, \"num3\")";
                unordered_set<string> queryResults4 = spaManager.query(query4);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults4.size());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne5) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 5
                string query5 = "assign a; stmt s;\n"
                                "Select s such that Uses(s, \"num1\") and Uses(s, \"num2\") and Uses(s, \"k\")";
                unordered_set<string> queryResults5 = spaManager.query(query5);
                // Expected results: 11, 12, 13, 15, 16
                Assert::AreEqual(5, (int) queryResults5.size());
                Assert::IsTrue(queryResults5.find("11") != queryResults5.end());
                Assert::IsTrue(queryResults5.find("12") != queryResults5.end());
                Assert::IsTrue(queryResults5.find("13") != queryResults5.end());
                Assert::IsTrue(queryResults5.find("15") != queryResults5.end());
                Assert::IsTrue(queryResults5.find("16") != queryResults5.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne6) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 6
                string query6 = "assign a; stmt s; procedure p; \n"
                                "Select p such that Uses(p, \"num1\") and Uses(p, \"num2\") and Uses(p, \"k\")";
                unordered_set<string> queryResults6 = spaManager.query(query6);
                // Expected results: nested
                Assert::AreEqual(1, (int) queryResults6.size());
                Assert::IsTrue(queryResults6.find("nested") != queryResults6.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne7) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 7
                string query7 = "assign a; stmt s; procedure p; \n"
                                "Select s such that Uses(p, \"num100\") and Modifies(s, \"num6\")";
                unordered_set<string> queryResults7 = spaManager.query(query7);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults7.size());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne8) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 8
                string query8 = "assign a; stmt s, anotherS; procedure p; \n"
                                "Select s such that Uses(anotherS, \"num1\") and Modifies(s, \"num6\")";
                unordered_set<string> queryResults8 = spaManager.query(query8);
                // Expected results: 11, 12, 18
                Assert::AreEqual(3, (int) queryResults8.size());
                Assert::IsTrue(queryResults8.find("11") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("12") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("18") != queryResults8.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne9) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 9
                string query9 = "assign a; stmt parentS, childS; procedure p; \n"
                                "Select parentS such that Parent*(parentS, childS) and Modifies(childS, \"num6\")";
                unordered_set<string> queryResults9 = spaManager.query(query9);
                // Expected results: 12, 11
                Assert::AreEqual(2, (int) queryResults9.size());
                Assert::IsTrue(queryResults9.find("12") != queryResults9.end());
                Assert::IsTrue(queryResults9.find("11") != queryResults9.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne10) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 10 - similar to 9 but additional pattern for assign. Should not affect results.
                string query10 = "assign a; stmt parentS, childS; procedure p; \n"
                                 "Select parentS such that Parent*(parentS, childS) and Modifies(childS, \"num6\") pattern a(\"beingModified\", \"num1  +   num2\")";
                unordered_set<string> queryResults10 = spaManager.query(query10);
                // Expected results: 12, 11
                Assert::AreEqual(2, (int) queryResults10.size());
                Assert::IsTrue(queryResults10.find("12") != queryResults10.end());
                Assert::IsTrue(queryResults10.find("11") != queryResults10.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne11) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 11 - similar to 10 but it has such that with pattern. Get all parentS that has a childS that modifies num7, and has another childA which modifies beingModified.
                string query11 = "assign childA; stmt parentS, childS; procedure p; \n"
                                 "Select parentS such that Parent*(parentS, childS) and Modifies(childS, \"num7\") pattern childA(\"beingModified\", \"num1  +   num2\") such that Parent*(parentS, childA)";
                unordered_set<string> queryResults11 = spaManager.query(query11);
                // Expected results: 11
                Assert::AreEqual(1, (int) queryResults11.size());
                Assert::IsTrue(queryResults11.find("11") != queryResults11.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne12) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 12 - get call statements which calls something, and also uses num5
                string query12 = "call c; stmt s; procedure p, p1; \n"
                                 "Select c such that Calls(p1, _) and Uses(c, \"num5\")";
                unordered_set<string> queryResults12 = spaManager.query(query12);
                // Expected results: 17
                Assert::AreEqual(1, (int) queryResults12.size());
                Assert::IsTrue(queryResults12.find("17") != queryResults12.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne13) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 13 - get procedures that uses num1, num2 and num3
                string query13 = "procedure p; \n"
                                 "Select p such that Uses(p, \"num1\") and Uses(p, \"num2\") and Uses(p, \"num3\")";
                unordered_set<string> queryResults13 = spaManager.query(query13);
                // Expected results: procOne
                Assert::AreEqual(1, (int) queryResults13.size());
                Assert::IsTrue(queryResults13.find("procOne") != queryResults13.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne14) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 14 - get procedures that uses num1, num2, and num5
                string query14 = "procedure p; \n"
                                 "Select p such that Uses(p, \"num1\") and Uses(p, \"num2\") and Uses(p, \"num5\")";
                unordered_set<string> queryResults14 = spaManager.query(query14);
                // Expected results: nested
                Assert::AreEqual(1, (int) queryResults14.size());
                Assert::IsTrue(queryResults14.find("nested") != queryResults14.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne15) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 15 - get procedures such that <something true>
                string query15 = "procedure p, p1; call c; stmt s;  read r; \n"
                                 "Select p such that Calls(p, p1) and Modifies(r, \"num7\") and Modifies(p, \"num7\")  and Uses(s, \"num5\")";
                unordered_set<string> queryResults15 = spaManager.query(query15);
                // Expected results: nested
                Assert::AreEqual(1, (int) queryResults15.size());
                Assert::IsTrue(queryResults15.find("nested") != queryResults15.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne16) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 16 - get procedures such that <something false>
                string query16 = "procedure p, p1; call c; stmt s;  read r; assign a; variable v;\n"
                                 "Select p such that Calls(p, p1) pattern  a(_, _\"num1 + num9\"_)  ";
                unordered_set<string> queryResults16 = spaManager.query(query16);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults16.size());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne17) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 17 - multiple pattern clauses. Connecting the patterns with and keyword
                string query17 = "procedure p, p1; call c; stmt s;  read r; assign a, a1; variable v;\n"
                                 "Select a1 such that Calls(p, p1) pattern  a(v, _)  and a1(_, _\"num2\"_)  ";
                unordered_set<string> queryResults17 = spaManager.query(query17);
                // Expected results: 14, 16
                Assert::AreEqual(2, (int) queryResults17.size());
                Assert::IsTrue(queryResults17.find("14") != queryResults17.end());
                Assert::IsTrue(queryResults17.find("16") != queryResults17.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne18) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 18 - multiple pattern clauses with Next/*. This test case should pass after Next is implemented.
                string query18 = "procedure p, p1; call c; stmt s;  read r; assign a, a1; variable v;\n"
                                 "Select s such that Calls(p, p1) pattern  a(v, _)  and a1(_, _\"num2\"_)  such that Next(a1, s)  ";
                unordered_set<string> queryResults18 = spaManager.query(query18);
                // Expected results: 15, 13
                Assert::AreEqual(2, (int) queryResults18.size());
                Assert::IsTrue(queryResults18.find("15") != queryResults18.end());
                Assert::IsTrue(queryResults18.find("13") != queryResults18.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne19) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 19 - multiple pattern clauses with Next/*
                string query19 = "procedure p, p1; call c; stmt s;  read r; assign a, a1; variable v;\n"
                                 "Select v pattern a(v, _) and a1(_, _\"num2\"_) such that Next*(11, a)";
                unordered_set<string> queryResults19 = spaManager.query(query19);
                // Expected results: beingModified, beingMod3
                Assert::AreEqual(2, (int) queryResults19.size());
                Assert::IsTrue(queryResults19.find("beingModified") != queryResults19.end());
                Assert::IsTrue(queryResults19.find("beingMod3") != queryResults19.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne20) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 20 - multiple pattern clauses with Next/*. Select read stmts that can come after 11
                string query20 = "procedure p, p1; call c; stmt s;  read r; assign a, a1; variable v;\n"
                                 "Select r pattern a(v, _) and a1(_, _\"num2\"_) such that Next*(11, r)";
                unordered_set<string> queryResults20 = spaManager.query(query20);
                // Expected results: 18, 20
                Assert::AreEqual(2, (int) queryResults20.size());
                Assert::IsTrue(queryResults20.find("18") != queryResults20.end());
                Assert::IsTrue(queryResults20.find("20") != queryResults20.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartOne21) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 21 - Same as query 20 but with num22
                string query21 = "procedure p, p1; call c; stmt s;  read r; assign a, a1; variable v;\n"
                                 "Select r pattern a(v, _) and a1(_, _\"num22\"_) such that Next*(11, r)";
                unordered_set<string> queryResults21 = spaManager.query(query21);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults21.size());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartTwo1) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1 - Multiple pattern clauses on the same while loop. This test case should pass after while-pattern is implemented.
                string query1 = "procedure p; while w; variable v; assign a; \n"
                                "Select w pattern a(v, _) and a(_, _\"cenY\"_) and w(\"y\", _) pattern w(\"x\", _)";
                unordered_set<string> queryResults1 = spaManager.query(query1);
                // Expected results: 15
                Assert::AreEqual(1, (int) queryResults1.size());
                Assert::IsTrue(queryResults1.find("15") != queryResults1.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartTwo2) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 2 - Multiple pattern clauses on the same while loop. This test case should pass after if-pattern is implemented.
                string query2 = "procedure p; while w; variable v; assign a; if ifs;\n"
                                "Select w pattern a(v, _) pattern a(_, _\"cenY\"_) and ifs(\"count\", _, _)";
                unordered_set<string> queryResults2 = spaManager.query(query2);
                // Expected results: 15
                Assert::AreEqual(1, (int) queryResults2.size());
                Assert::IsTrue(queryResults2.find("15") != queryResults2.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartTwo3) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 3 - Pattern for while and also checking follows and parent. This test case should pass after if-pattern is implemented.
                string query3 = "procedure p; while w; variable v; assign a; if ifs;\n"
                                "Select w pattern ifs(\"count\", _,    _) such that Follows(w, ifs)";
                unordered_set<string> queryResults3 = spaManager.query(query3);
                // Expected results: 15
                Assert::AreEqual(1, (int) queryResults3.size());
                Assert::IsTrue(queryResults3.find("15") != queryResults3.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartTwo4) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 4 - Pattern for while and checking for next. This test case should pass after if-pattern is implemented.
                string query4 = "procedure p; while w; variable v; assign a; if ifs;\n"
                                "Select w pattern ifs(\"count\", _, _) such that Next(w, ifs)";
                unordered_set<string> queryResults4 = spaManager.query(query4);
                // Expected results: 15
                Assert::AreEqual(1, (int) queryResults4.size());
                Assert::IsTrue(queryResults4.find("15") != queryResults4.end());
            }

            TEST_METHOD(TestMultipleClausesWithAndPartTwo5) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 5 - Pattern for while and checking for next*. This test case should pass after if-pattern is implemented.
                string query5 = "procedure p; while w; variable v; assign a; if ifs;\n"
                                "Select w pattern ifs(\"count\", _, _  ) such that Next*(w, ifs)";
                unordered_set<string> queryResults5 = spaManager.query(query5);
                // Expected results: 15
                Assert::AreEqual(1, (int) queryResults5.size());
                Assert::IsTrue(queryResults5.find("15") != queryResults5.end());
            }
    };
}
