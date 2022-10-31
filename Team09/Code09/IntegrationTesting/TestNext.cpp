#include "stdafx.h"
#include "CppUnitTest.h"
#include <SPAManager/SPAManager.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace IntegrationTesting {
    TEST_CLASS(TestNext) {
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
                switch (ref) {
                    case 1:
                        return program1;
                    default:
                        return "";
                }
            }

            public:
            TEST_METHOD(TestNext1) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1
                string query1 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; \n"
                                "Select BOOLEAN such that Next(1, 3)   ";
                unordered_set<string> queryResults1 = spaManager.query(query1);
                /*
                 * Expected results:
                 * FALSE
                 */
                Assert::AreEqual(1, (int) queryResults1.size());
                Assert::IsTrue(queryResults1.find("FALSE") != queryResults1.end());
            }

            TEST_METHOD(TestNext2) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 2
                string query2 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; \n"
                                "Select BOOLEAN such that Next(2, 3)   ";
                unordered_set<string> queryResults2 = spaManager.query(query2);
                /*
                 * Expected results:
                 * FALSE
                 */
                Assert::AreEqual(1, (int) queryResults2.size());
                Assert::IsTrue(queryResults2.find("FALSE") != queryResults2.end());
            }

            TEST_METHOD(TestNext3) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 3
                string query3 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; \n"
                                "Select BOOLEAN such that Next(3, 4)   ";
                unordered_set<string> queryResults3 = spaManager.query(query3);
                /*
                 * Expected results:
                 * TRUE
                 */
                Assert::AreEqual(1, (int) queryResults3.size());
                Assert::IsTrue(queryResults3.find("TRUE") != queryResults3.end());
            }

            TEST_METHOD(TestNext4) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 4
                string query4 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; \n"
                                "Select <a1, a2> such that Next(a1, a2)   ";
                unordered_set<string> queryResults4 = spaManager.query(query4);
                /*
                 * Empty expected results
                 */
                Assert::AreEqual(0, (int) queryResults4.size());
            }

            TEST_METHOD(TestNext5) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 5
                string query5 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; if ifs1, ifs2;  \n"
                                "Select <a1, ifs1> such that Next(a1, ifs1)   ";
                unordered_set<string> queryResults5 = spaManager.query(query5);
                /*
                 * Expected results:
                 * 14 15
                 */
                Assert::AreEqual(1, (int) queryResults5.size());
                Assert::IsTrue(queryResults5.find("14 15") != queryResults5.end());
            }

            TEST_METHOD(TestNext6) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 6
                string query6 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; if ifs1, ifs2; while w1, w2; \n"
                                "Select <w1, w2> such that Next(w1, w2)   ";
                unordered_set<string> queryResults6 = spaManager.query(query6);
                /*
                 * Expected results:
                 * 1 3
                 * 3 5
                 * 5 7
                 */
                Assert::AreEqual(3, (int) queryResults6.size());
                Assert::IsTrue(queryResults6.find("1 3") != queryResults6.end());
                Assert::IsTrue(queryResults6.find("3 5") != queryResults6.end());
                Assert::IsTrue(queryResults6.find("5 7") != queryResults6.end());
            }

            TEST_METHOD(TestNext7) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 7
                string query7 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; if ifs1, ifs2; while w1, w2; \n"
                                "Select <w2, w1> such that Next(w1, w2)   ";
                unordered_set<string> queryResults7 = spaManager.query(query7);
                /*
                 * Expected results:
                 * 3 1
                 * 5 3
                 * 7 5
                 */
                Assert::AreEqual(3, (int) queryResults7.size());
                Assert::IsTrue(queryResults7.find("3 1") != queryResults7.end());
                Assert::IsTrue(queryResults7.find("5 3") != queryResults7.end());
                Assert::IsTrue(queryResults7.find("7 5") != queryResults7.end());
            }

            TEST_METHOD(TestNext8and9) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 8 and 9
                string query8 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; if ifs1, ifs2; while w1, w2; \n"
                                "Select <ss1, ss2> such that Next(ss1, ss2)   ";
                unordered_set<string> queryResults8 = spaManager.query(query8);
                /*
                 * Expected results:
                 * 1 3
                 * 3 5
                 * 5 7
                 * 1 2
                 * 3 4
                 * 5 6
                 * 7 8
                 * 9 10
                 * 11 19
                 * 15 17
                 * 15 16
                 * 13 18
                 * not 17 18
                 * not 15 18
                 * not 16 18
                 */
                Assert::IsTrue((int) queryResults8.size() > 17);
                Assert::IsTrue(queryResults8.find("1 3") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("3 5") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("5 7") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("1 2") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("3 4") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("5 6") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("7 8") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("9 10") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("11 19") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("15 17") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("15 16") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("13 18") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("17 18") == queryResults8.end());
                Assert::IsTrue(queryResults8.find("15 18") == queryResults8.end());
                Assert::IsTrue(queryResults8.find("16 18") == queryResults8.end());
                //! I didn't list out all the results because there are too many of them.

                string query9 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; if ifs1, ifs2; while w1, w2; \n"
                                "Select <ss2.stmt#, ss1> such that Next(ss1, ss2)   ";
                unordered_set<string> queryResults9 = spaManager.query(query9);
                /*
                 * Expected results:
                 * 3 1
                 * 5 3
                 * 7 5
                 * 2 1
                 * 4 3
                 * 6 5
                 * 8 7
                 * 10 9
                 * 19 11
                 * 17 15
                 * 16 15
                 * 18 13
                 * not 18 17
                 * not 18 15
                 * not 18 16
                 */
                Assert::IsTrue((int) queryResults9.size() > 17);
                //! Check that the results count is the same as query 8
                Assert::IsTrue((int) queryResults9.size() == (int) queryResults8.size());
                Assert::IsTrue(queryResults9.find("3 1") != queryResults9.end());
                Assert::IsTrue(queryResults9.find("5 3") != queryResults9.end());
                Assert::IsTrue(queryResults9.find("7 5") != queryResults9.end());
                Assert::IsTrue(queryResults9.find("2 1") != queryResults9.end());
                Assert::IsTrue(queryResults9.find("4 3") != queryResults9.end());
                Assert::IsTrue(queryResults9.find("6 5") != queryResults9.end());
                Assert::IsTrue(queryResults9.find("8 7") != queryResults9.end());
                Assert::IsTrue(queryResults9.find("10 9") != queryResults9.end());
                Assert::IsTrue(queryResults9.find("19 11") != queryResults9.end());
                Assert::IsTrue(queryResults9.find("17 15") != queryResults9.end());
                Assert::IsTrue(queryResults9.find("16 15") != queryResults9.end());
                Assert::IsTrue(queryResults9.find("18 13") != queryResults9.end());
                Assert::IsTrue(queryResults9.find("18 17") == queryResults9.end());
                Assert::IsTrue(queryResults9.find("18 15") == queryResults9.end());
                Assert::IsTrue(queryResults9.find("18 16") == queryResults9.end());
                //! I didn't list out all the results because there are too many of them.
            }

            TEST_METHOD(TestNext10) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 10
                string query10 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; if ifs1, ifs2; while w1, w2; \n"
                                 "Select <w1.stmt#, w2.stmt#> such that Next*(w1, w2)   ";
                unordered_set<string> queryResults10 = spaManager.query(query10);
                /*
                 * Expected results:
                 * 1 3, 1 5, 1 7, 3 5, 3 7, 5 7, 12 13
                 */
                Assert::AreEqual(7, (int) queryResults10.size());
                Assert::IsTrue(queryResults10.find("1 3") != queryResults10.end());
                Assert::IsTrue(queryResults10.find("1 5") != queryResults10.end());
                Assert::IsTrue(queryResults10.find("1 7") != queryResults10.end());
                Assert::IsTrue(queryResults10.find("3 5") != queryResults10.end());
                Assert::IsTrue(queryResults10.find("3 7") != queryResults10.end());
                Assert::IsTrue(queryResults10.find("5 7") != queryResults10.end());
                Assert::IsTrue(queryResults10.find("12 13") != queryResults10.end());
            }

            TEST_METHOD(TestNext11) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 11
                string query11 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; if ifs1, ifs2; while w1, w2; \n"
                                 "Select BOOLEAN such that Next*(w1, w2)   ";
                unordered_set<string> queryResults11 = spaManager.query(query11);
                /*
                 * Expected results:
                 * TRUE
                 */
                Assert::AreEqual(1, (int) queryResults11.size());
                Assert::IsTrue(queryResults11.find("TRUE") != queryResults11.end());
            }

            TEST_METHOD(TestNext12) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 12
                string query12 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; if ifs1, ifs2; while w1, w2, BOOLEAN; \n"
                                 "Select BOOLEAN such that Next*(w1, BOOLEAN)   ";
                unordered_set<string> queryResults12 = spaManager.query(query12);
                /*
                 * Expected results:
                 * 3, 5, 7, 13
                 */
                Assert::AreEqual(4, (int) queryResults12.size());
                Assert::IsTrue(queryResults12.find("3") != queryResults12.end());
                Assert::IsTrue(queryResults12.find("5") != queryResults12.end());
                Assert::IsTrue(queryResults12.find("7") != queryResults12.end());
                Assert::IsTrue(queryResults12.find("13") != queryResults12.end());
            }

            TEST_METHOD(TestNext13) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 13
                string query13 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; if ifs1, ifs2; while w1, w2, BOOLEAN; \n"
                                 "Select BOOLEAN such that Next*(BOOLEAN, w1)   ";
                unordered_set<string> queryResults13 = spaManager.query(query13);
                /*
                 * Expected results:
                 * 1, 3, 5, 12
                 */
                Assert::AreEqual(4, (int) queryResults13.size());
                Assert::IsTrue(queryResults13.find("1") != queryResults13.end());
                Assert::IsTrue(queryResults13.find("3") != queryResults13.end());
                Assert::IsTrue(queryResults13.find("5") != queryResults13.end());
                Assert::IsTrue(queryResults13.find("12") != queryResults13.end());
            }

            TEST_METHOD(TestNext14) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 14
                string query14 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; if ifs1, ifs2; while w1, w2, BOOLEAN; \n"
                                 "Select BOOLEAN such that Next*(BOOLEAN, BOOLEAN)   ";
                unordered_set<string> queryResults14 = spaManager.query(query14);
                /*
                 * Expected results: 13
                 */
                Assert::AreEqual(1, (int) queryResults14.size());
                Assert::IsTrue(queryResults14.find("13") != queryResults14.end());
            }

            TEST_METHOD(TestNext15) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 15
                string query15 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; if ifs1, ifs2; while w1, w2; \n"
                                 "Select <ss1, ss2> such that Next*(ss1, ss2) and Next*(ss2, ss1)   ";
                unordered_set<string> queryResults15 = spaManager.query(query15);
                /*
                 * Expected results:
                 * 12 12, 12 13, 13 12, 13 13, 14 16, 16 14, 16 16, 17 17, 18 18, 19 19, 12 18, 18 12
                 */
                //! More than the listed results are expected.
                Assert::IsTrue((int) queryResults15.size() > 12);
                Assert::IsTrue(queryResults15.find("12 12") != queryResults15.end());
                Assert::IsTrue(queryResults15.find("12 13") != queryResults15.end());
                Assert::IsTrue(queryResults15.find("13 12") != queryResults15.end());
                Assert::IsTrue(queryResults15.find("13 13") != queryResults15.end());
                Assert::IsTrue(queryResults15.find("14 16") != queryResults15.end());
                Assert::IsTrue(queryResults15.find("16 14") != queryResults15.end());
                Assert::IsTrue(queryResults15.find("16 16") != queryResults15.end());
                Assert::IsTrue(queryResults15.find("17 17") != queryResults15.end());
                Assert::IsTrue(queryResults15.find("18 18") != queryResults15.end());
                Assert::IsTrue(queryResults15.find("19 19") != queryResults15.end());
                Assert::IsTrue(queryResults15.find("12 18") != queryResults15.end());
                Assert::IsTrue(queryResults15.find("18 12") != queryResults15.end());
            }

            TEST_METHOD(TestNext16) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 16
                string query16 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; if ifs1, ifs2; while w1, w2; \n"
                                 "Select BOOLEAN such that Next(ss1, _)";
                unordered_set<string> queryResults16 = spaManager.query(query16);
                /*
                 * Expected results:
                 * TRUE
                 */
                Assert::AreEqual(1, (int) queryResults16.size());
                Assert::IsTrue(queryResults16.find("TRUE") != queryResults16.end());
            }

            TEST_METHOD(TestNext17) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 17
                string query17 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; if ifs1, ifs2; while w1, w2; \n"
                                 "Select ss1 such that Next(ss1, _)  ";
                unordered_set<string> queryResults17 = spaManager.query(query17);
                /*
                 * Expected results:
                 * 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20
                 */
                Assert::AreEqual(20, (int) queryResults17.size());
            }

            TEST_METHOD(TestNext18) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 18
                string query18 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; if ifs1, ifs2; while w1, w2; \n"
                                 "Select rr1.stmt# such that Next(rr1, _) and Next(_, rr1)  ";
                unordered_set<string> queryResults18 = spaManager.query(query18);
                /*
                 * Expected results:
                 * 18, 20
                 */
                Assert::AreEqual(2, (int) queryResults18.size());
                Assert::IsTrue(queryResults18.find("18") != queryResults18.end());
                Assert::IsTrue(queryResults18.find("20") != queryResults18.end());
            }

            TEST_METHOD(TestNext19) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 19
                string query19 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; if ifs1, ifs2; while w1, w2; procedure p; \n"
                                 "Select <rr1.stmt#, p.procName> such that Next(rr1, _) and Next(_, rr1) ";
                unordered_set<string> queryResults19 = spaManager.query(query19);
                /*
                 * Expected results:
                 * 18 procOne, 20 procOne
                 * 18 procTwo, 20 procTwo
                 * 18 nested, 20 nested
                 */
                Assert::IsTrue((int) queryResults19.size() == 6);
                Assert::IsTrue(queryResults19.find("18 procOne") != queryResults19.end());
                Assert::IsTrue(queryResults19.find("20 procOne") != queryResults19.end());
                Assert::IsTrue(queryResults19.find("18 procTwo") != queryResults19.end());
                Assert::IsTrue(queryResults19.find("20 procTwo") != queryResults19.end());
                Assert::IsTrue(queryResults19.find("18 nested") != queryResults19.end());
                Assert::IsTrue(queryResults19.find("20 nested") != queryResults19.end());
            }

            TEST_METHOD(TestNext20) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 20
                string query20 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; if ifs1, ifs2; while w1, w2; procedure p; \n"
                                 "Select <rr1, p.procName> such that Next(rr1, _) and Next(_, rr1) and Next(rr1, _) and Next(_, rr1) ";
                unordered_set<string> queryResults20 = spaManager.query(query20);
                /*
                 * Expected results:
                 * 18 procOne, 20 procOne
                 * 18 procTwo, 20 procTwo
                 * 18 nested, 20 nested
                 */
                Assert::IsTrue((int) queryResults20.size() == 6);
                Assert::IsTrue(queryResults20.find("18 procOne") != queryResults20.end());
                Assert::IsTrue(queryResults20.find("20 procOne") != queryResults20.end());
                Assert::IsTrue(queryResults20.find("18 procTwo") != queryResults20.end());
                Assert::IsTrue(queryResults20.find("20 procTwo") != queryResults20.end());
                Assert::IsTrue(queryResults20.find("18 nested") != queryResults20.end());
                Assert::IsTrue(queryResults20.find("20 nested") != queryResults20.end());
            }

            TEST_METHOD(TestNext21) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 21
                string query21 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; if ifs1, ifs2; while w1, w2; procedure p; \n"
                                 "Select <rr1, p.procName> such that Next*(rr1, _) and Next*(_, rr1)  ";
                unordered_set<string> queryResults21 = spaManager.query(query21);
                /*
                 * Expected results:
                 * 18 procOne, 20 procOne
                 * 18 procTwo, 20 procTwo
                 * 18 nested, 20 nested
                 */
                Assert::IsTrue((int) queryResults21.size() == 6);
                Assert::IsTrue(queryResults21.find("18 procOne") != queryResults21.end());
                Assert::IsTrue(queryResults21.find("20 procOne") != queryResults21.end());
                Assert::IsTrue(queryResults21.find("18 procTwo") != queryResults21.end());
                Assert::IsTrue(queryResults21.find("20 procTwo") != queryResults21.end());
                Assert::IsTrue(queryResults21.find("18 nested") != queryResults21.end());
                Assert::IsTrue(queryResults21.find("20 nested") != queryResults21.end());
            }

            TEST_METHOD(TestNext22) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 22
                string query22 = "assign a1, a2; stmt ss1, ss2; call cc1, cc2; read rr1, rr2; if ifs1, ifs2; while w1, w2; procedure p; \n"
                                 "Select a2 such that Next*(a1, 14)";
                unordered_set<string> queryResults22 = spaManager.query(query22);
                /*
                 * Expected results:
                 * 14, 16
                 */
                Assert::AreEqual(2, (int) queryResults22.size());
                Assert::IsTrue(queryResults22.find("14") != queryResults22.end());
                Assert::IsTrue(queryResults22.find("16") != queryResults22.end());
            }
    };
}
