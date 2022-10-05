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
                        //! TestNextNextTPart1
                        return program1;
                    case 2:
                        //! TestFollowsParent
                        return program2;
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
                Assert::AreEqual(0, (int) queryResults13.size(), L"Query 13 fails");
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
