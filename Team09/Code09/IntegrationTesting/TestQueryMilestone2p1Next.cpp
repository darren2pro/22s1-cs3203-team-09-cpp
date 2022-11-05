#include "stdafx.h"
#include "CppUnitTest.h"
#include <SPAManager/SPAManager.h>
#include <QueryProcessingSubsystem/Validator/SemanticException.h>
#include <SourceProcessor/exceptions/SimpleInvalidSyntaxException.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace IntegrationTesting {
    TEST_CLASS(TestQueryMilestone2p1Next) {
            public:
            string getCurrentProgram (int ref) {
                string program1 = "procedure First {\n"
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
                                  "                else {\n"
                                  "                        z = 1; }\n" // line 12
                                  "                z = z + x + i;\n" // line 13
                                  "                y = z + 2;\n" // line 14
                                  "                x = x * y + z; }\n" // line 15
                                  "\n"
                                  "procedure Third {\n"
                                  "        z = 5;\n" // line 16
                                  "        v = z;\n" // line 17
                                  "        print v; }"; // line 18
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
                string program3 = "procedure procOne {\n"
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
                string program4 = "procedure read {\n"
                                  "                    if (k == 1) then {\n" // line 1
                                  "                        if (j <= 1 + 5 * ddd) then {\n" // line 2
                                  "                            read num1;\n" // line 3
                                  "                        } else {\n"
                                  "                            read num2;\n" // line 4
                                  "                            if (num2 == 0) then {\n" // line 5
                                  "                                read num3;\n" // line 6
                                  "                            } else {\n"
                                  "                                read num4;\n" // line 7
                                  "                            }\n"
                                  "                        }\n"
                                  "                        read x;\n" // line 8
                                  "                    } else {\n"
                                  "                        while (1>= 1%((1)) ) {\n" // line 9
                                  "                            read num5;\n" // line 10
                                  "                            if (num5 == 0) then {\n" // line 11
                                  "                                read num6;\n" // line 12
                                  "                                if (num6 == 0) then {\n" // line 13
                                  "                                    read num7;\n" // line 14
                                  "                                } else {\n"
                                  "                                    read num8;\n" // line 15
                                  "                                    if (num8 == 0) then {\n" // line 16
                                  "                                        read num9;\n" // line 17
                                  "                                    } else {\n"
                                  "                                        read num10;\n" // line 18
                                  "                                    }\n"
                                  "                                }\n"
                                  "                            } else {\n"
                                  "                                read num7;\n" // line 19
                                  "                            }\n"
                                  "                        }\n"
                                  "                        read y;\n" // line 20
                                  "                    }\n"
                                  "                }\t\t   ";
                string program5 = "procedure call {\n"
                                  "    if ( num1       > num2) then {\n" // line 1
                                  "        if ( num1   > num3) then {\n" // line 2
                                  "            if ( num2 > num3) then {\n" // line 3
                                  "                if ( num3 > kkk    ) then {\n" // line 4
                                  "                    read kk;\n" // line 5
                                  "                } else {\n"
                                  "                    print ll;\n" // line 6
                                  "                }\n"
                                  "                read num1;\n" // line 7
                                  "            } else {\n"
                                  "                read num3;\n" // line 8
                                  "                if ( num1 > num2) then {\n" // line 9
                                  "                    read num1;\n" // line 10
                                  "                } else {\n"
                                  "                    read num2;\n" // line 11
                                  "                }\n"
                                  "            }\n"
                                  "        } else {\n"
                                  "            read num3;\n" // line 12
                                  "        }\n"
                                  "        call num1;\n" // line 13
                                  "    } else          {\n"
                                  "        print num2;\n" // line 14
                                  "    }\n"
                                  "    call newProc;\n" // line 15
                                  "}\n"
                                  "procedure num1 {\n"
                                  "        print ll;\n"
                                  "}\n"
                                  "procedure newProc {\n"
                                  "        print ll;\n"
                                  "}\n";
                switch (ref) {
                    case 1:
                        //! TestNextNextTPart1
                        return program1;
                    case 2:
                        //! TestFollowsParent
                        return program2;
                    case 3:
                        //! TestFollowsParent and TestNextNextTPart2
                        return program3;
                    case 4:
                        //! TestNextNextTPart3
                        return program4;
                    case 5:
                        //! TestNextNextTPart4
                        return program5;
                    default:
                        return "";
                }
            }
            TEST_METHOD(TestNextNextTPartOne1) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1
                string query1 = "assign a; stmt s; read r;\n"
                                "Select r such that Next(a, 1)";
                unordered_set<string> queryResults1 = spaManager.query(query1);
                // Empty expected results
                Assert::AreEqual(0, (int) queryResults1.size(), L"Query 1 fails");
            }

            TEST_METHOD(TestNextNextTPartOne2) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 2
                string query2 = "assign a; stmt s; read r;\n"
                                "Select r such that Next(2, 3)";
                unordered_set<string> queryResults2 = spaManager.query(query2);
                // Expected results: 1, 2
                Assert::AreEqual(2, (int) queryResults2.size(), L"Query 2 fails");
                Assert::IsTrue(queryResults2.find("1") != queryResults2.end(), L"Query 2 fails");
                Assert::IsTrue(queryResults2.find("2") != queryResults2.end(), L"Query 2 fails");
            }

            TEST_METHOD(TestNextNextTPartOne3) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 3 - statements in different procedures
                string query3 = "assign a; stmt s; read r;\n"
                                "Select r such that Next(3, 4)";
                unordered_set<string> queryResults3 = spaManager.query(query3);
                // Empty expected results
                Assert::AreEqual(0, (int) queryResults3.size(), L"Query 3 fails");
            }

            TEST_METHOD(TestNextNextTPartOne4) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 4
                string query4 = "assign a; stmt s; read r;\n"
                                "Select r such that Next(6, 7)";
                unordered_set<string> queryResults4 = spaManager.query(query4);
                // Expected results: 1, 2
                Assert::AreEqual(2, (int) queryResults4.size(), L"Query 4 fails");
                Assert::IsTrue(queryResults4.find("1") != queryResults4.end(), L"Query 4 fails");
                Assert::IsTrue(queryResults4.find("2") != queryResults4.end(), L"Query 4 fails");
            }

            TEST_METHOD(TestNextNextTPartOne5) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 5
                string query5 = "assign a; stmt s; read r;\n"
                                "Select r such that Next(6, 10)";
                unordered_set<string> queryResults5 = spaManager.query(query5);
                // Expected results: 1, 2
                Assert::AreEqual(2, (int) queryResults5.size(), L"Query 5 fails");
                Assert::IsTrue(queryResults5.find("1") != queryResults5.end(), L"Query 5 fails");
                Assert::IsTrue(queryResults5.find("2") != queryResults5.end(), L"Query 5 fails");
            }

            TEST_METHOD(TestNextNextTPartOne6) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 6
                string query6 = "assign a; stmt s; read r;\n"
                                "Select r such that Next(10, 11)";
                unordered_set<string> queryResults6 = spaManager.query(query6);
                // Expected results: 1, 2
                Assert::AreEqual(2, (int) queryResults6.size(), L"Query 6 fails");
                Assert::IsTrue(queryResults6.find("1") != queryResults6.end(), L"Query 6 fails");
                Assert::IsTrue(queryResults6.find("2") != queryResults6.end(), L"Query 6 fails");
            }

            TEST_METHOD(TestNextNextTPartOne7) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 7
                string query7 = "assign a; stmt s; read r;\n"
                                "Select r such that Next(10, 12)";
                unordered_set<string> queryResults7 = spaManager.query(query7);
                // Expected results: 1, 2
                Assert::AreEqual(2, (int) queryResults7.size(), L"Query 7 fails");
                Assert::IsTrue(queryResults7.find("1") != queryResults7.end(), L"Query 7 fails");
                Assert::IsTrue(queryResults7.find("2") != queryResults7.end(), L"Query 7 fails");
            }

            TEST_METHOD(TestNextNextTPartOne8) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 8
                string query8 = "assign a; stmt s; read r;\n"
                                "Select r such that Next(8, 16)";
                unordered_set<string> queryResults8 = spaManager.query(query8);
                // Empty expected results
                Assert::AreEqual(0, (int) queryResults8.size(), L"Query 8 fails");
            }

            TEST_METHOD(TestNextNextTPartOne9) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 9
                string query9 = "assign a; stmt s; read r;\n"
                                "Select s such that Next*(6, s)";
                unordered_set<string> queryResults9 = spaManager.query(query9);
                // Expected results: 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
                Assert::AreEqual(10, (int) queryResults9.size(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("6") != queryResults9.end(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("7") != queryResults9.end(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("8") != queryResults9.end(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("9") != queryResults9.end(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("10") != queryResults9.end(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("11") != queryResults9.end(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("12") != queryResults9.end(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("13") != queryResults9.end(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("14") != queryResults9.end(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("15") != queryResults9.end(), L"Query 9 fails");
            }

            TEST_METHOD(TestNextNextTPartOne10) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 10
                string query10 = "assign a; stmt s; read r;\n"
                                 "Select s such that Next*(s, 16)";
                unordered_set<string> queryResults10 = spaManager.query(query10);
                // Empty expected results
                Assert::AreEqual(0, (int) queryResults10.size(), L"Query 10 fails");
            }

            TEST_METHOD(TestNextNextTPartOne11) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 11
                string query11 = "assign a; stmt s; read r;\n"
                                 "Select s such that Next*(s, 13)";
                unordered_set<string> queryResults11 = spaManager.query(query11);
                // Expected results: 12, 11, 10, 9, 8, 7, 6, 5, 4
                Assert::AreEqual(9, (int) queryResults11.size(), L"Query 11 fails");
                Assert::IsTrue(queryResults11.find("12") != queryResults11.end(), L"Query 11 fails");
                Assert::IsTrue(queryResults11.find("11") != queryResults11.end(), L"Query 11 fails");
                Assert::IsTrue(queryResults11.find("10") != queryResults11.end(), L"Query 11 fails");
                Assert::IsTrue(queryResults11.find("9") != queryResults11.end(), L"Query 11 fails");
                Assert::IsTrue(queryResults11.find("8") != queryResults11.end(), L"Query 11 fails");
                Assert::IsTrue(queryResults11.find("7") != queryResults11.end(), L"Query 11 fails");
                Assert::IsTrue(queryResults11.find("6") != queryResults11.end(), L"Query 11 fails");
                Assert::IsTrue(queryResults11.find("5") != queryResults11.end(), L"Query 11 fails");
                Assert::IsTrue(queryResults11.find("4") != queryResults11.end(), L"Query 11 fails");
            }

            TEST_METHOD(TestNextNextTPartOne12) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 12
                string query12 = "assign a; stmt s; read r;\n"
                                 "Select s such that Next*(18, s)";
                unordered_set<string> queryResults12 = spaManager.query(query12);
                // Empty expected results
                Assert::AreEqual(0, (int) queryResults12.size(), L"Query 12 fails");
            }

            TEST_METHOD(TestNextNextTPartOne13) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 13
                string query13 = "assign a; stmt s; read r;\n"
                                 "Select s such that Next*(11, s)";
                unordered_set<string> queryResults13 = spaManager.query(query13);
                // Empty expected results
                Assert::AreEqual(3, (int) queryResults13.size(), L"Query 13 fails");
                Assert::IsTrue(queryResults13.find("13") != queryResults13.end(), L"Query 13 fails");
                Assert::IsTrue(queryResults13.find("14") != queryResults13.end(), L"Query 13 fails");
                Assert::IsTrue(queryResults13.find("15") != queryResults13.end(), L"Query 13 fails");
            }

            TEST_METHOD(TestNextNextTPartTwo1) {
                string program = getCurrentProgram(3);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1
                string query1 = "assign a; stmt s; while w; if i; variable v;\n"
                                "Select s such that Next*(s, 2)";
                unordered_set<string> queryResults1 = spaManager.query(query1);
                // Empty expected results
                Assert::AreEqual(2, (int) queryResults1.size(), L"Query 1 fails");
                Assert::IsTrue(queryResults1.find("1") != queryResults1.end(), L"Query 1 fails");
                Assert::IsTrue(queryResults1.find("2") != queryResults1.end(), L"Query 1 fails");
            }

            TEST_METHOD(TestNextNextTPartTwo2) {
                string program = getCurrentProgram(3);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 2
                string query2 = "assign a; stmt s; while w; if i; variable v;\n"
                                "Select s such that Next(s, 3)";
                unordered_set<string> queryResults2 = spaManager.query(query2);
                // Expected results: 1, 2
                Assert::AreEqual(2, (int) queryResults2.size(), L"Query 2 fails");
                Assert::IsTrue(queryResults2.find("1") != queryResults2.end(), L"Query 2 fails");
                Assert::IsTrue(queryResults2.find("4") != queryResults2.end(), L"Query 2 fails");
            }

            TEST_METHOD(TestNextNextTPartTwo3) {
                string program = getCurrentProgram(3);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 3
                string query3 = "assign a; stmt dfgs; while w; if i; variable v;\n"
                                "Select dfgs such that Next(dfgs, 4)";
                unordered_set<string> queryResults3 = spaManager.query(query3);
                // Expected results: 3
                Assert::AreEqual(1, (int) queryResults3.size(), L"Query 3 fails");
                Assert::IsTrue(queryResults3.find("3") != queryResults3.end(), L"Query 3 fails");
            }

            TEST_METHOD(TestNextNextTPartTwo4) {
                string program = getCurrentProgram(3);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 4
                string query4 = "assign a; stmt s; while w; if i; variable v;\n"
                                "Select a such that Next*(a, 18)";
                unordered_set<string> queryResults4 = spaManager.query(query4);
                // Expected results: 16, 14
                Assert::AreEqual(2, (int) queryResults4.size(), L"Query 4 fails");
                Assert::IsTrue(queryResults4.find("16") != queryResults4.end(), L"Query 4 fails");
                Assert::IsTrue(queryResults4.find("14") != queryResults4.end(), L"Query 4 fails");
            }

            TEST_METHOD(TestNextNextTPartTwo5) {
                string program = getCurrentProgram(3);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 5
                string query5 = "assign a; stmt s; while w; if i; variable v;      call tgh;\n"
                                "Select tgh such that Next*(tgh, 18)";
                unordered_set<string> queryResults5 = spaManager.query(query5);
                // Expected results: 17
                Assert::AreEqual(1, (int) queryResults5.size(), L"Query 5 fails");
                Assert::IsTrue(queryResults5.find("17") != queryResults5.end(), L"Query 5 fails");
            }

            TEST_METHOD(TestNextNextTPartTwo6) {
                string program = getCurrentProgram(3);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 6
                string query6 = "assign a; stmt s; while w; if i; variable v;\n"
                                "Select a such that Next*(a, 19)";
                unordered_set<string> queryResults6 = spaManager.query(query6);
                // Empty expected results
                Assert::AreEqual(0, (int) queryResults6.size(), L"Query 6 fails");
            }

            TEST_METHOD(TestNextNextTPartTwo7) {
                string program = getCurrentProgram(3);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 7
                string query7 = "assign a; stmt s; while w; if i; variable v; read kkk;\n"
                                "Select s such that Next(s, 20)";
                unordered_set<string> queryResults7 = spaManager.query(query7);
                // Expected results: 19
                Assert::AreEqual(1, (int) queryResults7.size(), L"Query 7 fails");
                Assert::IsTrue(queryResults7.find("19") != queryResults7.end(), L"Query 7 fails");
            }

            TEST_METHOD(TestNextNextTPartTwo8) {
                string program = getCurrentProgram(3);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 8
                string query8 = "assign a; stmt s; while w; if i; variable v; print pp;\n"
                                "Select pp such that Next(pp, 6)";
                unordered_set<string> queryResults8 = spaManager.query(query8);
                // Empty expected results
                Assert::AreEqual(0, (int) queryResults8.size(), L"Query 8 fails");
            }

            TEST_METHOD(TestNextNextTPartTwo9) {
                string program = getCurrentProgram(3);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 9
                string query9 = "assign a; stmt s; while w; if i; variable v; print pp;\n"
                                "Select pp such that Next(pp, 5)";
                unordered_set<string> queryResults9 = spaManager.query(query9);
                // Expected  results: 6
                Assert::AreEqual(1, (int) queryResults9.size(), L"Query 9 fails");
                Assert::IsTrue(queryResults9.find("6") != queryResults9.end(), L"Query 9 fails");
            }

            TEST_METHOD(TestNextNextTPartTwo10) {
                string program = getCurrentProgram(3);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 10
                string query10 = "assign a; stmt s; while w; if i; variable v; print pp;\n"
                                 "Select pp such that Next*(pp, 6)";
                unordered_set<string> queryResults10 = spaManager.query(query10);
                // Expected results: 2, 4, 6
                Assert::AreEqual(3, (int) queryResults10.size(), L"Query 10 fails");
                Assert::IsTrue(queryResults10.find("2") != queryResults10.end(), L"Query 10 fails");
                Assert::IsTrue(queryResults10.find("4") != queryResults10.end(), L"Query 10 fails");
                Assert::IsTrue(queryResults10.find("6") != queryResults10.end(), L"Query 10 fails");
            }

            TEST_METHOD(TestNextNextTPartTwo11) {
                string program = getCurrentProgram(3);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 11 - immediately before 19
                string query11 = "assign a; stmt s; while w; if i; variable v; print pp;\n"
                                 "Select s such that Next(s, 19)";
                unordered_set<string> queryResults11 = spaManager.query(query11);
                // Expected results: 11, 20
                Assert::AreEqual(2, (int) queryResults11.size(), L"Query 11 fails");
                Assert::IsTrue(queryResults11.find("11") != queryResults11.end(), L"Query 11 fails");
                Assert::IsTrue(queryResults11.find("20") != queryResults11.end(), L"Query 11 fails");
            }

            TEST_METHOD(TestNextNextTPartTwo12) {
                string program = getCurrentProgram(3);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 12 - immediately before 18
                string query12 = "assign a; stmt s; while w; if i; variable v; print pp;\n"
                                 "Select s such that Next(s, 18)";
                unordered_set<string> queryResults12 = spaManager.query(query12);
                // Expected results: 13
                Assert::AreEqual(1, (int) queryResults12.size(), L"Query 12 fails");
                Assert::IsTrue(queryResults12.find("13") != queryResults12.end(), L"Query 12 fails");
            }

            TEST_METHOD(TestNextNextTPartThree1) {
                string program = getCurrentProgram(4);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1 - right before 8
                string query1 = "assign a; stmt s; while w; if i; variable v; print pp;\n"
                                "Select s such that Next(s, 8)";
                unordered_set<string> queryResults1 = spaManager.query(query1);
                // Expected results: 7, 6, 3
                Assert::AreEqual(3, (int) queryResults1.size(), L"Query 1 fails");
                Assert::IsTrue(queryResults1.find("7") != queryResults1.end(), L"Query 1 fails");
                Assert::IsTrue(queryResults1.find("6") != queryResults1.end(), L"Query 1 fails");
                Assert::IsTrue(queryResults1.find("3") != queryResults1.end(), L"Query 1 fails");
            }

            TEST_METHOD(TestNextNextTPartThree2) {
                string program = getCurrentProgram(4);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 2 - right before 20
                string query2 = "assign a; stmt s; while w; if i; variable v; print pp;\n"
                                "Select s such that Next(s, 20)";
                unordered_set<string> queryResults2 = spaManager.query(query2);
                // Expected results: 9
                Assert::AreEqual(1, (int) queryResults2.size(), L"Query 2 fails");
                Assert::IsTrue(queryResults2.find("9") != queryResults2.end(), L"Query 2 fails");
            }

            TEST_METHOD(TestNextNextTPartFour1) {
                string program = getCurrentProgram(5);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1 - right before 13
                string query1 = "assign a; stmt s; while w; if i; variable v; print pp;\n"
                                "Select s such that Next(s, 13)";
                unordered_set<string> queryResults1 = spaManager.query(query1);
                // Expected results: 12, 11, 10, 7
                Assert::AreEqual(4, (int) queryResults1.size(), L"Query 1 fails");
                Assert::IsTrue(queryResults1.find("12") != queryResults1.end(), L"Query 1 fails");
                Assert::IsTrue(queryResults1.find("11") != queryResults1.end(), L"Query 1 fails");
                Assert::IsTrue(queryResults1.find("10") != queryResults1.end(), L"Query 1 fails");
                Assert::IsTrue(queryResults1.find("7") != queryResults1.end(), L"Query 1 fails");

                // Should not have 3?
            }

            TEST_METHOD(TestNextNextTPartFour2) {
                string program = getCurrentProgram(5);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 2 - right before 15
                string query2 = "assign a; stmt s; while w; if i; variable v; print pp;\n"
                                "Select s such that Next(s, 15)";
                unordered_set<string> queryResults2 = spaManager.query(query2);
                // Expected results: 14, 13
                Assert::AreEqual(2, (int) queryResults2.size(), L"Query 2 fails");
                Assert::IsTrue(queryResults2.find("14") != queryResults2.end(), L"Query 2 fails");
                Assert::IsTrue(queryResults2.find("13") != queryResults2.end(), L"Query 2 fails");
            }

            TEST_METHOD(TestNextNextTPartFour3) {
                string program = getCurrentProgram(5);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);
                string query3 = "assign a; stmt s; while w; if i; variable v; print pp;\n"
                                "Select v such that Modifies(\"call\", v)";
                unordered_set<string> queryResults3 = spaManager.query(query3);
                // Expected results: kk, num1, num3, num2
                Assert::AreEqual(4, (int) queryResults3.size(), L"Query 3 fails");
                Assert::IsTrue(queryResults3.find("kk") != queryResults3.end(), L"Query 3 fails");
                Assert::IsTrue(queryResults3.find("num1") != queryResults3.end(), L"Query 3 fails");
                Assert::IsTrue(queryResults3.find("num3") != queryResults3.end(), L"Query 3 fails");
                Assert::IsTrue(queryResults3.find("num2") != queryResults3.end(), L"Query 3 fails");
            }

            TEST_METHOD(TestNextNextTPartFour4) {
                string program = getCurrentProgram(5);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                string query4 = "assign a; stmt s; while w; if i; variable v; print pp;\n"
                                "Select v such that Uses(\"newProc\", v)";
                unordered_set<string> queryResults4 = spaManager.query(query4);
                // Expected results: ll
                Assert::AreEqual(1, (int) queryResults4.size(), L"Query 4 fails");
				Assert::IsTrue(queryResults4.find("ll") != queryResults4.end(), L"Query 4 fails");
            }

            TEST_METHOD(TestNextNextTPartFour5) {
                string program = getCurrentProgram(5);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);
                string query5 = "assign a; stmt s; while w; if i; variable v; print pp;\n"
                                "Select v such that Uses(\"call\", v)";
                unordered_set<string> queryResults5 = spaManager.query(query5);
                // Expected results: num1, num2, num3, kkk, ll
                Assert::AreEqual(5, (int) queryResults5.size(), L"Query 5 fails");
                Assert::IsTrue(queryResults5.find("num1") != queryResults5.end(), L"Query 5 fails");
                Assert::IsTrue(queryResults5.find("num2") != queryResults5.end(), L"Query 5 fails");
                Assert::IsTrue(queryResults5.find("num3") != queryResults5.end(), L"Query 5 fails");
                Assert::IsTrue(queryResults5.find("kkk") != queryResults5.end(), L"Query 5 fails");
                Assert::IsTrue(queryResults5.find("ll") != queryResults5.end(), L"Query 5 fails");
            }

            TEST_METHOD(TestNextNextTPartFour6) {
                string program = getCurrentProgram(5);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);
                string query6 = "assign a; stmt s; while w; if i; variable v; print pp;\n"
                                "Select v such that Uses(\"newProccccc\", v)";
                unordered_set<string> queryResults6 = spaManager.query(query6);
                // Expected results: empty set
                Assert::AreEqual(0, (int) queryResults6.size(), L"Query 6 fails");
            }

            TEST_METHOD(TestFollowsParent1) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1
                string query1 = "call a; stmt s; while w; if i; variable v;\n"
                                "Select a such that Follows(1, a)";
                unordered_set<string> queryResults1 = spaManager.query(query1);
                // Expected results: 2
                Assert::AreEqual(1, (int) queryResults1.size(), L"Query 1 fails");
                Assert::IsTrue(queryResults1.find("2") != queryResults1.end(), L"Query 1 fails");
            }

            TEST_METHOD(TestFollowsParent2) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 2
                string query2 = "call a; stmt s; while w; if i; variable v;\n"
                                "Select a such that Follows(a, 3)";
                unordered_set<string> queryResults2 = spaManager.query(query2);
                // Expected results: 2
                Assert::AreEqual(1, (int) queryResults2.size(), L"Query 2 fails");
                Assert::IsTrue(queryResults2.find("2") != queryResults2.end(), L"Query 2 fails");
            }

            TEST_METHOD(TestFollowsParent3) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 3
                string query3 = "call a; stmt s; while w; if i; variable v;\n"
                                "Select a such that Follows(3, s)";
                unordered_set<string> queryResults3 = spaManager.query(query3);
                // Empty expected results
                Assert::AreEqual(0, (int) queryResults3.size(), L"Query 3 fails");
            }

            TEST_METHOD(TestFollowsParent4) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 4
                string query4 = "call a; stmt s; while w; if i; variable v;\n"
                                "Select a such that Follows*(5, 6)";
                unordered_set<string> queryResults4 = spaManager.query(query4);
                // Empty expected results
                Assert::AreEqual(0, (int) queryResults4.size(), L"Query 4 fails");
            }

            TEST_METHOD(TestFollowsParent5) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 5
                string query5 = "call ccc; stmt s; while w; if i; variable v;\n"
                                "Select ccc such that Follows*(10, ccc)";
                unordered_set<string> queryResults5 = spaManager.query(query5);
                // Expected results: 14
                Assert::AreEqual(1, (int) queryResults5.size(), L"Query 5 fails");
                Assert::IsTrue(queryResults5.find("14") != queryResults5.end(), L"Query 5 fails");
            }

            TEST_METHOD(TestFollowsParent6) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 6
                string query6 = "call ccc; stmt s; while w; if i; variable v;\n"
                                "Select s such that Follows*(s, 18)";
                unordered_set<string> queryResults6 = spaManager.query(query6);
                // Expected results: 16, 17
                Assert::AreEqual(2, (int) queryResults6.size(), L"Query 6 fails");
                Assert::IsTrue(queryResults6.find("16") != queryResults6.end(), L"Query 6 fails");
                Assert::IsTrue(queryResults6.find("17") != queryResults6.end(), L"Query 6 fails");
            }

            TEST_METHOD(TestFollowsParent7) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 7
                string query7 = "call ccc; stmt s; while w; if i; variable v;\n"
                                "Select i such that Follows*(15, i)";
                unordered_set<string> queryResults7 = spaManager.query(query7);
                // Expected results: 20
                Assert::AreEqual(1, (int) queryResults7.size(), L"Query 7 fails");
                Assert::IsTrue(queryResults7.find("20") != queryResults7.end(), L"Query 7 fails");
            }

            TEST_METHOD(TestFollowsParent8) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 8
                string query8 = "call ccc; stmt s; while w; if i; variable v;\n"
                                "Select s such that Follows*(s, i)";
                unordered_set<string> queryResults8 = spaManager.query(query8);
                // Expected results: 10, 11, 12, 13, 14, 15
                Assert::AreEqual(6, (int) queryResults8.size(), L"Query 8 fails");
                Assert::IsTrue(queryResults8.find("10") != queryResults8.end(), L"Query 8 fails");
                Assert::IsTrue(queryResults8.find("11") != queryResults8.end(), L"Query 8 fails");
                Assert::IsTrue(queryResults8.find("12") != queryResults8.end(), L"Query 8 fails");
                Assert::IsTrue(queryResults8.find("13") != queryResults8.end(), L"Query 8 fails");
                Assert::IsTrue(queryResults8.find("14") != queryResults8.end(), L"Query 8 fails");
                Assert::IsTrue(queryResults8.find("15") != queryResults8.end(), L"Query 8 fails");
            }

            TEST_METHOD(TestFollowsParent9) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 9
                string query9 = "call ccc; stmt s; while w; if i; variable v;\n"
                                "Select s such that Follows*(i, 23)";
                unordered_set<string> queryResults9 = spaManager.query(query9);
                // Empty expected results
                Assert::AreEqual(0, (int) queryResults9.size(), L"Query 9 fails");
            }

            TEST_METHOD(TestFollowsParent10) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 10
                string query10 = "call ccc; stmt s; while w; if i; variable v;\n"
                                 "Select s such that Follows*(21, 22)";
                unordered_set<string> queryResults10 = spaManager.query(query10);
                // Empty expected results
                Assert::AreEqual(0, (int) queryResults10.size(), L"Query 10 fails");
            }

            TEST_METHOD(TestFollowsParent11) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 11
                string query11 = "call ccc; stmt s; while w; if i; variable v;\n"
                                 "Select s such that Follows*(22, 23)";
                unordered_set<string> queryResults11 = spaManager.query(query11);
                // Expected results: all statements should return which is 1 to 24
                Assert::AreEqual(24, (int) queryResults11.size(), L"Query 11 fails");
                for (int i = 1; i <= 24; i++) {
                    Assert::IsTrue(queryResults11.find(to_string(i)) != queryResults11.end(), L"Query 11 fails");
                }
            }

            TEST_METHOD(TestFollowsParent12) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 12
                string query12 = "call ccc; stmt s; while w; if i; variable v;\n"
                                 "Select s such that Follows*(s, 22)";
                unordered_set<string> queryResults12 = spaManager.query(query12);
                // Empty expected results
                Assert::AreEqual(0, (int) queryResults12.size(), L"Query 12 fails");
            }

            TEST_METHOD(TestFollowsParent13) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 13
                string query13 = "call ccc; stmt s, s1; while w; if i; variable v;\n"
                                 "Select s such that Parent(s, s1)";
                unordered_set<string> queryResults13 = spaManager.query(query13);
                // Expected results: 15, 20
                Assert::AreEqual(2, (int) queryResults13.size(), L"Query 13 fails");
                Assert::IsTrue(queryResults13.find("15") != queryResults13.end(), L"Query 13 fails");
                Assert::IsTrue(queryResults13.find("20") != queryResults13.end(), L"Query 13 fails");
            }

            TEST_METHOD(TestFollowsParent14) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 14
                string query14 = "call ccc; stmt s, s1; while w; if i; variable v;\n"
                                 "Select s such that Parent*(s, s1)";
                unordered_set<string> queryResults14 = spaManager.query(query14);
                // Expected results: 15, 20
                Assert::AreEqual(2, (int) queryResults14.size(), L"Query 14 fails");
                Assert::IsTrue(queryResults14.find("15") != queryResults14.end(), L"Query 14 fails");
                Assert::IsTrue(queryResults14.find("20") != queryResults14.end(), L"Query 14 fails");
            }

            TEST_METHOD(TestFollowsParent15) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 15
                string query15 = "call ccc; stmt s, s1; while w; if i; variable v;\n"
                                 "Select s such that Parent*(s, 20)";
                unordered_set<string> queryResults15 = spaManager.query(query15);
                // Empty expected results
                Assert::AreEqual(0, (int) queryResults15.size(), L"Query 15 fails");
            }

            TEST_METHOD(TestFollowsParent16) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 16
                string query16 = "call ccc; stmt s, s1; while w; if i; variable v;\n"
                                 "Select s such that Parent*(15, s)";
                unordered_set<string> queryResults16 = spaManager.query(query16);
                // Expected results: 16, 17, 18, 19
                Assert::AreEqual(4, (int) queryResults16.size(), L"Query 16 fails");
                Assert::IsTrue(queryResults16.find("16") != queryResults16.end(), L"Query 16 fails");
                Assert::IsTrue(queryResults16.find("17") != queryResults16.end(), L"Query 16 fails");
                Assert::IsTrue(queryResults16.find("18") != queryResults16.end(), L"Query 16 fails");
                Assert::IsTrue(queryResults16.find("19") != queryResults16.end(), L"Query 16 fails");
            }

            TEST_METHOD(TestFollowsParent17) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 17
                string query17 = "call ccc; stmt s, s1; while w; if i; variable v;\n"
                                 "Select ccc such that Parent*(15, ccc)";
                unordered_set<string> queryResults17 = spaManager.query(query17);
                // Expected results: 19
                Assert::AreEqual(1, (int) queryResults17.size(), L"Query 17 fails");
                Assert::IsTrue(queryResults17.find("19") != queryResults17.end(), L"Query 17 fails");
            }

            TEST_METHOD(TestFollowsParent18) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 18
                string query18 = "call ccc; stmt s, s1; while w; if i; variable v;\n"
                                 "Select ccc such that Parent*(20, 21)";
                unordered_set<string> queryResults18 = spaManager.query(query18);
                // Expected results: 2, 3, 14, 19
                Assert::AreEqual(4, (int) queryResults18.size(), L"Query 18 fails");
                Assert::IsTrue(queryResults18.find("2") != queryResults18.end(), L"Query 18 fails");
                Assert::IsTrue(queryResults18.find("3") != queryResults18.end(), L"Query 18 fails");
                Assert::IsTrue(queryResults18.find("14") != queryResults18.end(), L"Query 18 fails");
                Assert::IsTrue(queryResults18.find("19") != queryResults18.end(), L"Query 18 fails");
            }
    };
}
