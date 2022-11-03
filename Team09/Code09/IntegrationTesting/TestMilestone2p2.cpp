#include "stdafx.h"
#include "CppUnitTest.h"
#include <SPAManager/SPAManager.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace IntegrationTesting {
    TEST_CLASS(TestMilestone2p2) {
            private:
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
                                  "                k = num1 + num2;\n" // line 14
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
                switch (ref) {
                    case 1:
                        return program1;
                    default:
                        return "";
                }
            }

            public:

            TEST_METHOD(TestExhaustive0) {
                string program = "procedure procOne {\n"
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
                    "}\n";

                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                string query1 = "assign a; Select a";
                unordered_set<string> queryResults1 = spaManager.query(query1);

                // Expected results: 
                Assert::AreEqual(0, (int)queryResults1.size());
            }

            TEST_METHOD(TestExhaustive00) {
                string program = "procedure procOne {\n"
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
                    "}\n";

                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                string query1 = "print a; Select a";
                unordered_set<string> queryResults1 = spaManager.query(query1);

                // Expected results: 2 4 6 8
                Assert::AreEqual(4, (int)queryResults1.size());
                Assert::IsTrue(queryResults1.find("2") != queryResults1.end());
                Assert::IsTrue(queryResults1.find("4") != queryResults1.end());
                Assert::IsTrue(queryResults1.find("6") != queryResults1.end());
                Assert::IsTrue(queryResults1.find("8") != queryResults1.end());
            }

            TEST_METHOD(TestExhaustive1) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1
                string query1 = "assign a; stmt ss;\n"
                                "Select <a, a> such that Follows(_, _) pattern a(_, _)";
                unordered_set<string> queryResults1 = spaManager.query(query1);
                // Expected results: 14 14, 16 16
                Assert::AreEqual(2, (int) queryResults1.size());
                Assert::IsTrue(queryResults1.find("14 14") != queryResults1.end());
                Assert::IsTrue(queryResults1.find("16 16") != queryResults1.end());
            }

            TEST_METHOD(TestExhaustive2) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 2
                string query2 = "assign a; stmt ss; call cc;\n"
                                "Select <a, a, cc, cc> such that Follows*(_, _) and Follows(_, _) pattern a(_, _)";
                unordered_set<string> queryResults2 = spaManager.query(query2);
                // Expected results: 14 14 17 17, 16 16 17 17
                Assert::AreEqual(2, (int) queryResults2.size());
                Assert::IsTrue(queryResults2.find("14 14 17 17") != queryResults2.end());
                Assert::IsTrue(queryResults2.find("16 16 17 17") != queryResults2.end());
            }

            TEST_METHOD(TestExhaustive3) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 3
                string query3 = "assign a; stmt ss; call cc, cc1;\n"
                                "Select <a, a, cc, cc1> such that Follows*(_, _) and Follows(_, _) pattern a(_, _)";
                unordered_set<string> queryResults3 = spaManager.query(query3);
                // Expected results: 14 14 17 17, 16 16 17 17
                Assert::AreEqual(2, (int) queryResults3.size());
                Assert::IsTrue(queryResults3.find("14 14 17 17") != queryResults3.end());
                Assert::IsTrue(queryResults3.find("16 16 17 17") != queryResults3.end());
            }

            TEST_METHOD(TestExhaustive4) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 4
                string query4 = "assign a, a1; stmt ss; call cc, cc1;\n"
                                "Select <a, a1, cc, cc1> such that Follows*(_, _)";
                unordered_set<string> queryResults4 = spaManager.query(query4);
                // Expected results: 14 14 17 17, 16 16 17 17, 14 16 17 17, 16 14 17 17
                Assert::AreEqual(4, (int) queryResults4.size());
                Assert::IsTrue(queryResults4.find("14 14 17 17") != queryResults4.end());
                Assert::IsTrue(queryResults4.find("16 16 17 17") != queryResults4.end());
                Assert::IsTrue(queryResults4.find("14 16 17 17") != queryResults4.end());
                Assert::IsTrue(queryResults4.find("16 14 17 17") != queryResults4.end());
            }

            TEST_METHOD(TestExhaustive5) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 5
                string query5 = "assign a, a1; stmt ss; call cc, cc1;\n"
                                "Select <a, a1, cc, cc> such that Follows(_, _)";
                unordered_set<string> queryResults5 = spaManager.query(query5);
                // Expected results: 14 14 17 17, 16 16 17 17, 14 16 17 17, 16 14 17 17
                Assert::AreEqual(4, (int) queryResults5.size());
                Assert::IsTrue(queryResults5.find("14 14 17 17") != queryResults5.end());
                Assert::IsTrue(queryResults5.find("16 16 17 17") != queryResults5.end());
                Assert::IsTrue(queryResults5.find("14 16 17 17") != queryResults5.end());
                Assert::IsTrue(queryResults5.find("16 14 17 17") != queryResults5.end());
            }

            TEST_METHOD(TestExhaustive6) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 6
                string query6 = "assign a, a1; stmt ss; call cc, cc1; while w, w1;\n"
                                "Select <a, a1, w, w1> such that Parent(_, _) pattern w(_, _)";
                unordered_set<string> queryResults6 = spaManager.query(query6);
                /*
                 * Possible results for a, a1 are: 14 and 16
                 * Possible results for w, w1 are: 1, 3, 5, 7, 9, 12, 13, 19
                 *
                 * 14 14 1 1, 14 14 3 3, 14 14 5 5, 14 14 7 7, 14 14 9 9, 14 14 12 12, 14 14 13 13, 14 14 19 19,
                 * 16 16 1 1, 16 16 3 3, 16 16 5 5, 16 16 7 7, 16 16 9 9, 16 16 12 12, 16 16 13 13, 16 16 19 19
                 * 14 16 1 1, 14 16 3 3, 14 16 5 5, 14 16 7 7, 14 16 9 9, 14 16 12 12, 14 16 13 13, 14 16 19 19,
                 * 16 14 1 1, 16 14 3 3, 16 14 5 5, 16 14 7 7, 16 14 9 9, 16 14 12 12, 16 14 13 13, 16 14 19 19
                 *
                 * 14 14 1 3, 14 14 1 5, 14 14 1 7, 14 14 1 9, 14 14 1 12, 14 14 1 13, 14 14 1 19,
                 * 14 14 3 1, 14 14 3 5, 14 14 3 7, 14 14 3 9, 14 14 3 12, 14 14 3 13, 14 14 3 19,
                 * 14 14 5 1, 14 14 5 3, 14 14 5 7, 14 14 5 9, 14 14 5 12, 14 14 5 13, 14 14 5 19,
                 * 14 14 7 1, 14 14 7 3, 14 14 7 5, 14 14 7 9, 14 14 7 12, 14 14 7 13, 14 14 7 19,
                 * 14 14 9 1, 14 14 9 3, 14 14 9 5, 14 14 9 7, 14 14 9 12, 14 14 9 13, 14 14 9 19,
                 * 14 14 12 1, 14 14 12 3, 14 14 12 5, 14 14 12 7, 14 14 12 9, 14 14 12 13, 14 14 12 19,
                 * 14 14 13 1, 14 14 13 3, 14 14 13 5, 14 14 13 7, 14 14 13 9, 14 14 13 12, 14 14 13 19,
                 * 14 14 19 1, 14 14 19 3, 14 14 19 5, 14 14 19 7, 14 14 19 9, 14 14 19 12, 14 14 19 13,
                 *
                 * 16 16 1 3, 16 16 1 5, 16 16 1 7, 16 16 1 9, 16 16 1 12, 16 16 1 13, 16 16 1 19,
                 * 16 16 3 1, 16 16 3 5, 16 16 3 7, 16 16 3 9, 16 16 3 12, 16 16 3 13, 16 16 3 19,
                 * 16 16 5 1, 16 16 5 3, 16 16 5 7, 16 16 5 9, 16 16 5 12, 16 16 5 13, 16 16 5 19,
                 * 16 16 7 1, 16 16 7 3, 16 16 7 5, 16 16 7 9, 16 16 7 12, 16 16 7 13, 16 16 7 19,
                 * 16 16 9 1, 16 16 9 3, 16 16 9 5, 16 16 9 7, 16 16 9 12, 16 16 9 13, 16 16 9 19,
                 * 16 16 12 1, 16 16 12 3, 16 16 12 5, 16 16 12 7, 16 16 12 9, 16 16 12 13, 16 16 12 19,
                 * 16 16 13 1, 16 16 13 3, 16 16 13 5, 16 16 13 7, 16 16 13 9, 16 16 13 12, 16 16 13 19,
                 * 16 16 19 1, 16 16 19 3, 16 16 19 5, 16 16 19 7, 16 16 19 9, 16 16 19 12, 16 16 19 13,
                 *
                 * 14 16 1 3, 14 16 1 5, 14 16 1 7, 14 16 1 9, 14 16 1 12, 14 16 1 13, 14 16 1 19,
                 * 14 16 3 1, 14 16 3 5, 14 16 3 7, 14 16 3 9, 14 16 3 12, 14 16 3 13, 14 16 3 19,
                 * 14 16 5 1, 14 16 5 3, 14 16 5 7, 14 16 5 9, 14 16 5 12, 14 16 5 13, 14 16 5 19,
                 * 14 16 7 1, 14 16 7 3, 14 16 7 5, 14 16 7 9, 14 16 7 12, 14 16 7 13, 14 16 7 19,
                 * 14 16 9 1, 14 16 9 3, 14 16 9 5, 14 16 9 7, 14 16 9 12, 14 16 9 13, 14 16 9 19,
                 * 14 16 12 1, 14 16 12 3, 14 16 12 5, 14 16 12 7, 14 16 12 9, 14 16 12 13, 14 16 12 19,
                 * 14 16 13 1, 14 16 13 3, 14 16 13 5, 14 16 13 7, 14 16 13 9, 14 16 13 12, 14 16 13 19,
                 * 14 16 19 1, 14 16 19 3, 14 16 19 5, 14 16 19 7, 14 16 19 9, 14 16 19 12, 14 16 19 13,
                 *
                 * 16 14 1 3, 16 14 1 5, 16 14 1 7, 16 14 1 9, 16 14 1 12, 16 14 1 13, 16 14 1 19,
                 * 16 14 3 1, 16 14 3 5, 16 14 3 7, 16 14 3 9, 16 14 3 12, 16 14 3 13, 16 14 3 19,
                 * 16 14 5 1, 16 14 5 3, 16 14 5 7, 16 14 5 9, 16 14 5 12, 16 14 5 13, 16 14 5 19,
                 * 16 14 7 1, 16 14 7 3, 16 14 7 5, 16 14 7 9, 16 14 7 12, 16 14 7 13, 16 14 7 19,
                 * 16 14 9 1, 16 14 9 3, 16 14 9 5, 16 14 9 7, 16 14 9 12, 16 14 9 13, 16 14 9 19,
                 * 16 14 12 1, 16 14 12 3, 16 14 12 5, 16 14 12 7, 16 14 12 9, 16 14 12 13, 16 14 12 19,
                 * 16 14 13 1, 16 14 13 3, 16 14 13 5, 16 14 13 7, 16 14 13 9, 16 14 13 12, 16 14 13 19,
                 * 16 14 19 1, 16 14 19 3, 16 14 19 5, 16 14 19 7, 16 14 19 9, 16 14 19 12, 16 14 19 13
                 *
                 * Total: 192
                 * unordered_set is guaranteed to have unique elements
                 */
                Assert::AreEqual(192, (int) queryResults6.size());
                // Test some random results
                Assert::IsTrue(queryResults6.find("14 14 1 1") != queryResults6.end());
                Assert::IsTrue(queryResults6.find("14 14 1 3") != queryResults6.end());
                Assert::IsTrue(queryResults6.find("14 14 1 5") != queryResults6.end());
                Assert::IsTrue(queryResults6.find("16 14 19 1") != queryResults6.end());
                Assert::IsTrue(queryResults6.find("16 14 7 12") != queryResults6.end());
                Assert::IsTrue(queryResults6.find("16 16 9 5") != queryResults6.end());
                Assert::IsTrue(queryResults6.find("16 16 19 13") != queryResults6.end());
                Assert::IsTrue(queryResults6.find("14 16 12 19") != queryResults6.end());
                Assert::IsTrue(queryResults6.find("14 16 19 13") != queryResults6.end());
            }

            TEST_METHOD(TestExhaustive7) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 7 - same results as query 6
                string query7 = "assign a, a1; stmt ss; call cc, cc1; while w, w1;\n"
                                "Select <a, a1, w, w1> such that Parent*(_, _)";
                unordered_set<string> queryResults7 = spaManager.query(query7);
                Assert::AreEqual(192, (int) queryResults7.size());
                // Test some random results
                Assert::IsTrue(queryResults7.find("14 14 1 1") != queryResults7.end());
                Assert::IsTrue(queryResults7.find("16 14 9 7") != queryResults7.end());
            }

            TEST_METHOD(TestExhaustive8) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 8 - same results as query 6
                string query8 = "assign a, a1; stmt ss; call cc, cc1; while w, w1;\n"
                                "Select <a, a1, w, w1> such that Parent*(_, _) pattern w(_, _)";
                unordered_set<string> queryResults8 = spaManager.query(query8);
                Assert::AreEqual(192, (int) queryResults8.size());
                // Test some random results
                Assert::IsTrue(queryResults8.find("14 14 1 1") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("16 14 9 9") != queryResults8.end());
            }

            TEST_METHOD(TestExhaustive9) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 9 - same results as query 6
                string query9 = "assign a, a1; stmt ss; call cc, cc1; while w, w1;\n"
                                "Select <a, a1, w, w1> such that Parent*(_, _) and Parent(_, _) pattern w(_, _)";
                unordered_set<string> queryResults9 = spaManager.query(query9);
                Assert::AreEqual(192, (int) queryResults9.size());
                // Test some random results
                Assert::IsTrue(queryResults9.find("14 16 1 12") != queryResults9.end());
                Assert::IsTrue(queryResults9.find("16 16 9 9") != queryResults9.end());
            }

            TEST_METHOD(TestExhaustive10) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 10
                string query10 = "assign a, a1; stmt ss; call cc, cc1; while w, w1; read rr, rr1; \n"
                                 "Select <rr, rr1, rr, rr1> pattern w(_, _)";
                unordered_set<string> queryResults10 = spaManager.query(query10);
                /*
                 * Possible results for rr and rr1: 18, 20
                 * 18 18 18 18, 20 20 20 20, 18 20 18 20, 20 18 20 18
                 */
                Assert::AreEqual(4, (int) queryResults10.size());
                Assert::IsTrue(queryResults10.find("18 18 18 18") != queryResults10.end());
                Assert::IsTrue(queryResults10.find("20 20 20 20") != queryResults10.end());
                Assert::IsTrue(queryResults10.find("18 20 18 20") != queryResults10.end());
                Assert::IsTrue(queryResults10.find("20 18 20 18") != queryResults10.end());
            }

            TEST_METHOD(TestExhaustive11) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 11
                string query11 = "assign a, a1; stmt ss; call cc, cc1; while w, w1; read rr, rr1; \n"
                                 "Select <rr1, rr, rr1, rr> such that Uses(ss, _)";
                unordered_set<string> queryResults11 = spaManager.query(query11);
                /*
                 * Possible results for rr and rr1: 18, 20
                 * 18 18 18 18, 20 20 20 20, 18 20 18 20, 20 18 20 18
                 */
                Assert::AreEqual(4, (int) queryResults11.size());
                Assert::IsTrue(queryResults11.find("18 18 18 18") != queryResults11.end());
                Assert::IsTrue(queryResults11.find("20 20 20 20") != queryResults11.end());
                Assert::IsTrue(queryResults11.find("18 20 18 20") != queryResults11.end());
                Assert::IsTrue(queryResults11.find("20 18 20 18") != queryResults11.end());
            }

            TEST_METHOD(TestExhaustive12) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 12 - same results as query 11
                string query12 = "assign a, a1; stmt ss; call cc, cc1; while w, w1; read rr, rr1; \n"
                                 "Select <rr1, rr, rr1, rr> such that Uses(ss, _) pattern w(_, _)";
                unordered_set<string> queryResults12 = spaManager.query(query12);
                Assert::AreEqual(4, (int) queryResults12.size());
                Assert::IsTrue(queryResults12.find("18 18 18 18") != queryResults12.end());
                Assert::IsTrue(queryResults12.find("20 20 20 20") != queryResults12.end());
                Assert::IsTrue(queryResults12.find("18 20 18 20") != queryResults12.end());
                Assert::IsTrue(queryResults12.find("20 18 20 18") != queryResults12.end());
            }

            TEST_METHOD(TestExhaustive13) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 13
                string query13 = "assign a, a1; stmt ss; call cc, cc1; while w, w1; read rr, rr1; if ifs;\n"
                                 "Select <rr1, rr, rr1, rr> such that Modifies(ss, _) pattern ifs(_, _, _)";
                unordered_set<string> queryResults13 = spaManager.query(query13);
                /*
                 * Possible results for rr and rr1: 18, 20
                 * 18 18 18 18, 20 20 20 20, 18 20 18 20, 20 18 20 18
                 */
                Assert::AreEqual(4, (int) queryResults13.size());
                Assert::IsTrue(queryResults13.find("18 18 18 18") != queryResults13.end());
                Assert::IsTrue(queryResults13.find("20 20 20 20") != queryResults13.end());
                Assert::IsTrue(queryResults13.find("18 20 18 20") != queryResults13.end());
                Assert::IsTrue(queryResults13.find("20 18 20 18") != queryResults13.end());
            }

            TEST_METHOD(TestExhaustive14) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 14
                string query14 = "assign a, a1; stmt ss; call cc, cc1; while w, w1; read rr, rr1; if ifs;\n"
                                 "Select <rr1, rr, rr1, rr> such that Modifies(ss, _)";
                unordered_set<string> queryResults14 = spaManager.query(query14);
                /*
                 * Possible results for rr and rr1: 18, 20
                 * 18 18 18 18, 20 20 20 20, 18 20 18 20, 20 18 20 18
                 */
                Assert::AreEqual(4, (int) queryResults14.size());
                Assert::IsTrue(queryResults14.find("18 18 18 18") != queryResults14.end());
                Assert::IsTrue(queryResults14.find("20 20 20 20") != queryResults14.end());
                Assert::IsTrue(queryResults14.find("18 20 18 20") != queryResults14.end());
                Assert::IsTrue(queryResults14.find("20 18 20 18") != queryResults14.end());
            }

            TEST_METHOD(TestExhaustive15) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 15
                string query15 = "assign a, a1; stmt ss; call cc, cc1; while w, w1; read rr, rr1; if ifs;\n"
                                 "Select <rr1, rr, rr1, rr> such that Calls(_, _) pattern ifs(_, _, _) and w(_, _)";
                unordered_set<string> queryResults15 = spaManager.query(query15);
                /*
                 * Possible results for rr and rr1: 18, 20
                 * 18 18 18 18, 20 20 20 20, 18 20 18 20, 20 18 20 18
                 */
                Assert::AreEqual(4, (int) queryResults15.size());
                Assert::IsTrue(queryResults15.find("18 18 18 18") != queryResults15.end());
                Assert::IsTrue(queryResults15.find("20 20 20 20") != queryResults15.end());
                Assert::IsTrue(queryResults15.find("18 20 18 20") != queryResults15.end());
                Assert::IsTrue(queryResults15.find("20 18 20 18") != queryResults15.end());
            }

            TEST_METHOD(TestExhaustive16) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 16
                string query16 = "assign a, a1; stmt ss; call cc, cc1; while w, w1; read rr, rr1; if ifs;\n"
                                 "Select <rr1, rr, rr1, rr> such that Calls(_, _)";
                unordered_set<string> queryResults16 = spaManager.query(query16);
                /*
                 * Possible results for rr and rr1: 18, 20
                 * 18 18 18 18, 20 20 20 20, 18 20 18 20, 20 18 20 18
                 */
                Assert::AreEqual(4, (int) queryResults16.size());
                Assert::IsTrue(queryResults16.find("18 18 18 18") != queryResults16.end());
                Assert::IsTrue(queryResults16.find("20 20 20 20") != queryResults16.end());
                Assert::IsTrue(queryResults16.find("18 20 18 20") != queryResults16.end());
                Assert::IsTrue(queryResults16.find("20 18 20 18") != queryResults16.end());
            }

            TEST_METHOD(TestExhaustive17) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 17
                string query17 = "assign a, a1; stmt ss; call cc, cc1; while w, w1; read rr, rr1; if ifs;\n"
                                 "Select <rr1, rr, rr1, rr> such that Calls*(_, _) pattern ifs(_, _, _) and w(_, _)";
                unordered_set<string> queryResults17 = spaManager.query(query17);
                /*
                 * Possible results for rr and rr1: 18, 20
                 * 18 18 18 18, 20 20 20 20, 18 20 18 20, 20 18 20 18
                 */
                Assert::AreEqual(4, (int) queryResults17.size());
                Assert::IsTrue(queryResults17.find("18 18 18 18") != queryResults17.end());
                Assert::IsTrue(queryResults17.find("20 20 20 20") != queryResults17.end());
                Assert::IsTrue(queryResults17.find("18 20 18 20") != queryResults17.end());
                Assert::IsTrue(queryResults17.find("20 18 20 18") != queryResults17.end());
            }

            TEST_METHOD(TestExhaustive18) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 18
                string query18 = "assign a, a1; stmt ss; call cc, cc1; while w, w1; read rr, rr1; if ifs;\n"
                                 "Select <rr1, rr, rr1, rr> such that Calls*(_, _)";
                unordered_set<string> queryResults18 = spaManager.query(query18);
                /*
                 * Possible results for rr and rr1: 18, 20
                 * 18 18 18 18, 20 20 20 20, 18 20 18 20, 20 18 20 18
                 */
                Assert::AreEqual(4, (int) queryResults18.size());
                Assert::IsTrue(queryResults18.find("18 18 18 18") != queryResults18.end());
                Assert::IsTrue(queryResults18.find("20 20 20 20") != queryResults18.end());
                Assert::IsTrue(queryResults18.find("18 20 18 20") != queryResults18.end());
                Assert::IsTrue(queryResults18.find("20 18 20 18") != queryResults18.end());
            }

            TEST_METHOD(TestExhaustive19) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 19
                string query19 = "assign a, a1; stmt ss; call cc, cc1; while w, w1; read rr, rr1; if ifs;\n"
                                 "Select <rr1, rr, rr1, rr> such that Affects(_, _) pattern ifs(_, _, _) and w(_, _)";
                unordered_set<string> queryResults19 = spaManager.query(query19);
                /*
                 * Possible results for rr and rr1: 18, 20
                 * 18 18 18 18, 20 20 20 20, 18 20 18 20, 20 18 20 18
                 */
                Assert::AreEqual(4, (int) queryResults19.size());
                Assert::IsTrue(queryResults19.find("18 18 18 18") != queryResults19.end());
                Assert::IsTrue(queryResults19.find("20 20 20 20") != queryResults19.end());
                Assert::IsTrue(queryResults19.find("18 20 18 20") != queryResults19.end());
                Assert::IsTrue(queryResults19.find("20 18 20 18") != queryResults19.end());
            }

            TEST_METHOD(TestExhaustive20) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 20
                string query20 = "assign a, a1; stmt ss; call cc, cc1; while w, w1; read rr, rr1; if ifs;\n"
                                 "Select <rr1, rr, rr1, rr> such that Affects(_, _)";
                unordered_set<string> queryResults20 = spaManager.query(query20);
                /*
                 * Possible results for rr and rr1: 18, 20
                 * 18 18 18 18, 20 20 20 20, 18 20 18 20, 20 18 20 18
                 */
                Assert::AreEqual(4, (int) queryResults20.size());
                Assert::IsTrue(queryResults20.find("18 18 18 18") != queryResults20.end());
                Assert::IsTrue(queryResults20.find("20 20 20 20") != queryResults20.end());
                Assert::IsTrue(queryResults20.find("18 20 18 20") != queryResults20.end());
                Assert::IsTrue(queryResults20.find("20 18 20 18") != queryResults20.end());
            }

            TEST_METHOD(TestExhaustive21) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 21
                string query21 = "assign a, a1; stmt ss; call cc, cc1; while w, w1; read rr, rr1; if ifs;\n"
                                 "Select <rr1, rr, rr1, rr> such that Affects*(_, _) pattern ifs(_, _, _) and w(_, _)";
                unordered_set<string> queryResults21 = spaManager.query(query21);
                /*
                 * Possible results for rr and rr1: 18, 20
                 * 18 18 18 18, 20 20 20 20, 18 20 18 20, 20 18 20 18
                 */
                Assert::AreEqual(4, (int) queryResults21.size());
                Assert::IsTrue(queryResults21.find("18 18 18 18") != queryResults21.end());
                Assert::IsTrue(queryResults21.find("20 20 20 20") != queryResults21.end());
                Assert::IsTrue(queryResults21.find("18 20 18 20") != queryResults21.end());
                Assert::IsTrue(queryResults21.find("20 18 20 18") != queryResults21.end());
            }

            TEST_METHOD(TestExhaustive22) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 22
                string query22 = "assign a, a1; stmt ss; call cc, cc1; while w, w1; read rr, rr1; if ifs;\n"
                                 "Select <rr1, rr, rr1, rr> such that Affects*(_, _)";
                unordered_set<string> queryResults22 = spaManager.query(query22);
                /*
                 * Possible results for rr and rr1: 18, 20
                 * 18 18 18 18, 20 20 20 20, 18 20 18 20, 20 18 20 18
                 */
                Assert::AreEqual(4, (int) queryResults22.size());
                Assert::IsTrue(queryResults22.find("18 18 18 18") != queryResults22.end());
                Assert::IsTrue(queryResults22.find("20 20 20 20") != queryResults22.end());
                Assert::IsTrue(queryResults22.find("18 20 18 20") != queryResults22.end());
                Assert::IsTrue(queryResults22.find("20 18 20 18") != queryResults22.end());
            }

            TEST_METHOD(TestExhaustive23) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 23
                string query23 = "assign a, a1; stmt ss; call cc, cc1; while w, w1; read rr, rr1; if ifs;\n"
                                 "Select <rr1, rr, rr1, rr> such that Next(_, _) pattern ifs(_, _, _) and w(_, _)";
                unordered_set<string> queryResults23 = spaManager.query(query23);
                /*
                 * Possible results for rr and rr1: 18, 20
                 * 18 18 18 18, 20 20 20 20, 18 20 18 20, 20 18 20 18
                 */
                Assert::AreEqual(4, (int) queryResults23.size());
                Assert::IsTrue(queryResults23.find("18 18 18 18") != queryResults23.end());
                Assert::IsTrue(queryResults23.find("20 20 20 20") != queryResults23.end());
                Assert::IsTrue(queryResults23.find("18 20 18 20") != queryResults23.end());
                Assert::IsTrue(queryResults23.find("20 18 20 18") != queryResults23.end());
            }

            TEST_METHOD(TestExhaustive24) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 24
                string query24 = "assign a, a1; stmt ss; call cc, cc1; while w, w1; read rr, rr1; if ifs;\n"
                                 "Select <rr1, rr, rr1, rr> such that Next(_, _)";
                unordered_set<string> queryResults24 = spaManager.query(query24);
                /*
                 * Possible results for rr and rr1: 18, 20
                 * 18 18 18 18, 20 20 20 20, 18 20 18 20, 20 18 20 18
                 */
                Assert::AreEqual(4, (int) queryResults24.size());
                Assert::IsTrue(queryResults24.find("18 18 18 18") != queryResults24.end());
                Assert::IsTrue(queryResults24.find("20 20 20 20") != queryResults24.end());
                Assert::IsTrue(queryResults24.find("18 20 18 20") != queryResults24.end());
                Assert::IsTrue(queryResults24.find("20 18 20 18") != queryResults24.end());
            }

            TEST_METHOD(TestExhaustive25) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 25
                string query25 = "assign a, a1; stmt ss; call cc, cc1; while w, w1; read rr, rr1; if ifs;\n"
                                 "Select <rr1, rr, rr1, rr> such that Next*(_, _) pattern ifs(_, _, _) and w(_, _)";
                unordered_set<string> queryResults25 = spaManager.query(query25);
                /*
                 * Possible results for rr and rr1: 18, 20
                 * 18 18 18 18, 20 20 20 20, 18 20 18 20, 20 18 20 18
                 */
                Assert::AreEqual(4, (int) queryResults25.size());
                Assert::IsTrue(queryResults25.find("18 18 18 18") != queryResults25.end());
                Assert::IsTrue(queryResults25.find("20 20 20 20") != queryResults25.end());
                Assert::IsTrue(queryResults25.find("18 20 18 20") != queryResults25.end());
                Assert::IsTrue(queryResults25.find("20 18 20 18") != queryResults25.end());
            }

            TEST_METHOD(TestExhaustive26) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 26
                string query26 = "assign a, a1; stmt ss; call cc, cc1; while w, w1; read rr, rr1; if ifs;\n"
                                 "Select <rr1, rr, rr1, rr> such that Next*(_, _)";
                unordered_set<string> queryResults26 = spaManager.query(query26);
                /*
                 * Possible results for rr and rr1: 18, 20
                 * 18 18 18 18, 20 20 20 20, 18 20 18 20, 20 18 20 18
                 */
                Assert::AreEqual(4, (int) queryResults26.size());
                Assert::IsTrue(queryResults26.find("18 18 18 18") != queryResults26.end());
                Assert::IsTrue(queryResults26.find("20 20 20 20") != queryResults26.end());
                Assert::IsTrue(queryResults26.find("18 20 18 20") != queryResults26.end());
                Assert::IsTrue(queryResults26.find("20 18 20 18") != queryResults26.end());
            }
    };
}
