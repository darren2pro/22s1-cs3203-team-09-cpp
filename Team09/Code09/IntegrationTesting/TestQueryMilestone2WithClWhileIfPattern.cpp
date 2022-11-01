#include "stdafx.h"
#include "CppUnitTest.h"
#include <SPAManager/SPAManager.h>
#include <QueryProcessingSubsystem/Validator/SemanticException.h>
#include <SourceProcessor/exceptions/SimpleInvalidSyntaxException.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace IntegrationTesting {
    TEST_CLASS(TestQueryMilestone2WithClWhileIfPattern) {
            //! Remove or comment out this block to test your code
            //BEGIN_TEST_CLASS_ATTRIBUTE(TestQueryMilestone2WithClWhileIfPattern)
            //TEST_CLASS_ATTRIBUTE(L"Ignore", L"true")
            //END_TEST_CLASS_ATTRIBUTE()

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
            TEST_METHOD(TestMultipleWithClausePartOne1) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1 - Which procedure has name "nested"
                string query1 = "assign a; procedure mpp;\n"
                                "Select mpp with mpp.procName = \"nested\"    \t";
                unordered_set<string> queryResults1 = spaManager.query(query1);
                // Expected results: nested
                Assert::AreEqual(1, (int) queryResults1.size());
                Assert::IsTrue(queryResults1.find("nested") != queryResults1.end());
            }

            TEST_METHOD(TestMultipleWithClausePartOne2) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 2 - Which procedure has name "nested" and call another procedure
                string query2 = "assign a; procedure mpp, beingCalled; call c;\n"
                                "Select mpp with mpp.procName = \"nested\" such that Calls(mpp, beingCalled)";
                unordered_set<string> queryResults2 = spaManager.query(query2);
                // Expected results: nested
                Assert::AreEqual(1, (int) queryResults2.size());
                Assert::IsTrue(queryResults2.find("nested") != queryResults2.end());
            }

            TEST_METHOD(TestMultipleWithClausePartOne3) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 3 - Which procedure has name "nested" and calls procTwo
                string query3 = "assign a; procedure mpp, beingCalled; call c;\n"
                                "Select mpp.procName with mpp.procName = \"nested\" such that Calls(mpp, beingCalled) with beingCalled.procName = \"procTwo\"  \t";
                unordered_set<string> queryResults3 = spaManager.query(query3);
                // Expected results: nested
                Assert::AreEqual(1, (int) queryResults3.size());
                Assert::IsTrue(queryResults3.find("nested") != queryResults3.end());
            }

            TEST_METHOD(TestMultipleWithClausePartOne4) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 4 - Which procedure has name "nested" and calls procOne
                string query4 = "assign a; procedure mpp, beingCalled; call c;\n"
                                "Select mpp with mpp.procName = \"nested\" such that Calls(mpp, beingCalled) with beingCalled.procName = \"procOne\"  \t";
                unordered_set<string> queryResults4 = spaManager.query(query4);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults4.size());
            }

            TEST_METHOD(TestMultipleWithClausePartOne5) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 5 - Which procedure has name "nested" and modifies a variable called beingMod3
                string query5 = "assign a; procedure mpp; variable vv; \n"
                                "Select mpp.procName with mpp.procName = \"nested\" such that Modifies(mpp, vv) with vv.varName = \"beingMod3\" \t";
                unordered_set<string> queryResults5 = spaManager.query(query5);
                // Expected results: nested
                Assert::AreEqual(1, (int) queryResults5.size());
                Assert::IsTrue(queryResults5.find("nested") != queryResults5.end());
            }

            TEST_METHOD(TestMultipleWithClausePartOne6) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 6 - Which procedure uses variable num1
                string query6 = "assign a; procedure mpp; variable vv; \n"
                                "Select mpp such that Uses(mpp, vv) with vv.varName = \"num1\" \t";
                unordered_set<string> queryResults6 = spaManager.query(query6);
                // Expected results: nested, procOne
                Assert::AreEqual(2, (int) queryResults6.size());
                Assert::IsTrue(queryResults6.find("nested") != queryResults6.end());
                Assert::IsTrue(queryResults6.find("procOne") != queryResults6.end());
            }

            TEST_METHOD(TestMultipleWithClausePartOne7) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 7 - Same as 6 but changing the order of the clauses
                string query7 = "assign a; procedure mpp; variable vv; \n"
                                "Select mpp.procName with vv.varName = \"num1\" such that Uses(mpp, vv) \t";
                unordered_set<string> queryResults7 = spaManager.query(query7);
                // Expected results: nested, procOne
                Assert::AreEqual(2, (int) queryResults7.size());
                Assert::IsTrue(queryResults7.find("nested") != queryResults7.end());
                Assert::IsTrue(queryResults7.find("procOne") != queryResults7.end());
            }

            TEST_METHOD(TestMultipleWithClausePartOne8) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 8 - Which statement uses num2
                string query8 = "assign a; procedure mpp; variable vv; stmt s;\n"
                                "Select s such that Uses(s, vv) with vv.varName = \"num2\" \t";
                unordered_set<string> queryResults8 = spaManager.query(query8);
                // Expected results: 4, 14, 16
                Assert::AreEqual(8, (int) queryResults8.size());
                Assert::IsTrue(queryResults8.find("3") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("4") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("11") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("12") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("13") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("14") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("15") != queryResults8.end());
                Assert::IsTrue(queryResults8.find("16") != queryResults8.end());
            }

            TEST_METHOD(TestMultipleWithClausePartOne9) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 9 - Which statement uses num2 and has number of 66
                string query9 = "assign a; procedure mpp; variable vv; stmt s;\n"
                                "Select s.stmt# such that Uses(s, vv) with vv.varName = \"num2\" and s.stmt# = 66 \t";
                unordered_set<string> queryResults9 = spaManager.query(query9);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults9.size());
            }

            TEST_METHOD(TestMultipleWithClausePartOne10) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 10 - Which statement uses num2 and has number of 16
                string query10 = "assign a; procedure mpp; variable vv; stmt s;\n"
                                 "Select s.stmt# such that Uses(s, vv) with vv.varName = \"num2\" and s.stmt# = 16 \t";
                unordered_set<string> queryResults10 = spaManager.query(query10);
                // Expected results: 16
                Assert::AreEqual(1, (int) queryResults10.size());
                Assert::IsTrue(queryResults10.find("16") != queryResults10.end());
            }

            TEST_METHOD(TestMultipleWithClausePartOne11) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 11 - Which call statement has stmt# of 66
                string query11 = "assign a; procedure mpp; variable vv; stmt s; call c;\n"
                                 "Select c with c.stmt# = 66 \t";
                unordered_set<string> queryResults11 = spaManager.query(query11);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults11.size());
            }

            TEST_METHOD(TestMultipleWithClausePartOne12) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 12 - Which call statement has stmt# of 17
                string query12 = "assign a; procedure mpp; variable vv; stmt s; call c;\n"
                                 "Select c with c.stmt# = 17 \t";
                unordered_set<string> queryResults12 = spaManager.query(query12);
                // Expected results: 17
                Assert::AreEqual(1, (int) queryResults12.size());
                Assert::IsTrue(queryResults12.find("17") != queryResults12.end());
            }

            TEST_METHOD(TestMultipleWithClausePartOne13) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 13 - Which call statement has stmt# of 17 and calls procTwo
                string query13 = "assign a; procedure mpp, beingCalled; variable vv; stmt s; call c;\n"
                                 "Select c.stmt# with c.stmt# = 17 such that Calls(mpp, beingCalled) with beingCalled.procName = \"procTwo\" \t";
                unordered_set<string> queryResults13 = spaManager.query(query13);
                // Expected results: 17
                Assert::AreEqual(1, (int) queryResults13.size());
                Assert::IsTrue(queryResults13.find("17") != queryResults13.end());
            }

            TEST_METHOD(TestMultipleWithClausePartOne14) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 14 - Which call statement has stmt# of 17 and calls procOne
                string query14 = "assign a; procedure mpp, beingCalled; variable vv; stmt s; call c;\n"
                                 "Select c.stmt# with c.stmt# = 17 such that Calls(mpp, \"procTwo\")   \t";
                unordered_set<string> queryResults14 = spaManager.query(query14);
                // Expected results: 17
                Assert::AreEqual(1, (int) queryResults14.size());
                Assert::IsTrue(queryResults14.find("17") != queryResults14.end());
            }

            TEST_METHOD(TestMultipleWithClausePartOne15) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 15 - Which while statement has stmt# of 66
                string query15 = "assign a; procedure mpp; variable vv; stmt s; while w;\n"
                                 "Select w with w.stmt# = 66 \t";
                unordered_set<string> queryResults15 = spaManager.query(query15);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults15.size());
            }

            TEST_METHOD(TestMultipleWithClausePartOne16) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 16 - Which while statement has stmt# of 13 and uses kkk
                string query16 = "assign a; procedure mpp; variable vv; stmt s; while w;\n"
                                 "Select w.stmt# with w.stmt# = 13 such that Uses(w, vv) with vv.varName = \"kkk\" \t";
                unordered_set<string> queryResults16 = spaManager.query(query16);
                // Expected results: 13
                Assert::AreEqual(1, (int) queryResults16.size());
                Assert::IsTrue(queryResults16.find("13") != queryResults16.end());
            }

            TEST_METHOD(TestMultipleWithClausePartOne17) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 17 - Which while statement uses kkk
                string query17 = "assign a; procedure mpp; variable vv; stmt s; while w;\n"
                                 "Select w.stmt# such that Uses(w, vv) with vv.varName = \"kkk\"  \t";
                unordered_set<string> queryResults17 = spaManager.query(query17);
                // Expected results: 12, 13
                Assert::AreEqual(2, (int) queryResults17.size());
                Assert::IsTrue(queryResults17.find("12") != queryResults17.end());
                Assert::IsTrue(queryResults17.find("13") != queryResults17.end());
            }

            TEST_METHOD(TestMultipleWithClausePartOne18) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 18 - Which read statement has stmt# of 20
                string query18 = "assign a; procedure mpp; variable vv; stmt s; read r;\n"
                                 "Select r with r.stmt# = 20 \t";
                unordered_set<string> queryResults18 = spaManager.query(query18);
                // Expected results: 20
                Assert::AreEqual(1, (int) queryResults18.size());
                Assert::IsTrue(queryResults18.find("20") != queryResults18.end());
            }

            TEST_METHOD(TestMultipleWithClausePartOne19) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 19 - Which read statement has stmt# of 66
                string query19 = "assign a; procedure mpp; variable vv; stmt s; read r;\n"
                                 "Select r with r.stmt# = 66 \t";
                unordered_set<string> queryResults19 = spaManager.query(query19);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults19.size());
            }

            TEST_METHOD(TestMultipleWithClausePartOne20) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 20 - Which read statement has stmt# 18 and modifies num6
                string query20 = "assign a; procedure mpp; variable vv; stmt s; read r;\n"
                                 "Select r with r.stmt# = 18 such that Modifies(r, vv) with vv.varName = \"num6\" \t";
                unordered_set<string> queryResults20 = spaManager.query(query20);
                // Expected results: 18
                Assert::AreEqual(1, (int) queryResults20.size());
                Assert::IsTrue(queryResults20.find("18") != queryResults20.end());
            }

            TEST_METHOD(TestMultipleWithClausePartOne21) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 21 - Which read statement has stmt# 18 and modifies num6, written in a different way
                string query21 = "assign a; procedure mpp; variable vv; stmt s; read r;\n"
                                 "Select r with r.varName =     \"num6\"   with r.stmt# = 18   \t";
                unordered_set<string> queryResults21 = spaManager.query(query21);
                // Expected results: 18
                Assert::AreEqual(1, (int) queryResults21.size());
                Assert::IsTrue(queryResults21.find("18") != queryResults21.end());
            }

            TEST_METHOD(TestMultipleWithClausePartOne22) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 22 - Which read statement has stmt# of 20 and modifies num6
                string query22 = "assign a; procedure mpp; variable vv; stmt s; read r;\n"
                                 "Select r with r.stmt# = 20 such that Modifies(r, vv) with vv.varName = \"num6\" \t";
                unordered_set<string> queryResults22 = spaManager.query(query22);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults22.size());
            }

            TEST_METHOD(TestMultipleWithClausePartOne23) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 23 - Which read statement has stmt# of 20 and modifies num6, written in a different way
                string query23 = "assign a; procedure mpp; variable vv; stmt s; read r;\n"
                                 "Select r.stmt# with r.varName =     \"num6\"   with r.stmt# = 20   \t";
                unordered_set<string> queryResults23 = spaManager.query(query23);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults23.size());
            }

            TEST_METHOD(TestMultipleWithClausePartOne24) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 24 - Which read statement has stmt# of 20 and modifies num7
                string query24 = "assign a; procedure mpp; variable vv; stmt s; read r;\n"
                                 "Select r.stmt# with r.stmt# = 20  with r.varName = \"num7\" \t";
                unordered_set<string> queryResults24 = spaManager.query(query24);
                // Expected results: 20
                Assert::AreEqual(1, (int) queryResults24.size());
                Assert::IsTrue(queryResults24.find("20") != queryResults24.end());
            }

            TEST_METHOD(TestMultipleWithClausePartOne25) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 25 - Which statement has varName of num1, regardless of type
                string query25 = "assign a; print p; procedure mpp; variable vv; stmt s; read r; while w; call c;\n"
                                 "Select p.stmt# with p.varName = \"num1\" \t";
                unordered_set<string> queryResults25 = spaManager.query(query25);
                // Expected results: 2
                Assert::AreEqual(1, (int) queryResults25.size());
                Assert::IsTrue(queryResults25.find("2") != queryResults25.end());
            }

            TEST_METHOD(TestMultipleWithClausePartOne26) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 26 - Is the value 12 equals 12?
                string query26 = "assign a; procedure mpp; variable vv; stmt s; read r; while w; call c;\n"
                                 "Select w with 12 = 12 \t";
                unordered_set<string> queryResults26 = spaManager.query(query26);
                // Expected results: 1, 3, 5, 7, 9, 12, 13, 19
                Assert::AreEqual(8, (int) queryResults26.size());
                Assert::IsTrue(queryResults26.find("1") != queryResults26.end());
                Assert::IsTrue(queryResults26.find("3") != queryResults26.end());
                Assert::IsTrue(queryResults26.find("5") != queryResults26.end());
                Assert::IsTrue(queryResults26.find("7") != queryResults26.end());
                Assert::IsTrue(queryResults26.find("9") != queryResults26.end());
                Assert::IsTrue(queryResults26.find("12") != queryResults26.end());
                Assert::IsTrue(queryResults26.find("13") != queryResults26.end());
                Assert::IsTrue(queryResults26.find("19") != queryResults26.end());
            }

            TEST_METHOD(TestMultipleWithClausePartOne27) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 27 - Is the value 12 equals 13?
                string query27 = "assign a; procedure mpp; variable vv; stmt s; read r; while w; call c;\n"
                                 "Select w with 12 = 13 \t";
                unordered_set<string> queryResults27 = spaManager.query(query27);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults27.size());
            }

            TEST_METHOD(TestMultipleWithClausePartOne28) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 28 - Is the value 12 equals 12 and also w.stmt# is 11?
                string query28 = "assign a; procedure mpp; variable vv; stmt s; read r; while w; call c;\n"
                                 "Select w with 12 = 12 with w.stmt# = 11 \t";
                unordered_set<string> queryResults28 = spaManager.query(query28);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults28.size());
            }

            TEST_METHOD(TestWhileIfPatternClausePartOne1) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1 - Which while statement has kkk as control variable?
                string query1 = "assign a; procedure mpp; variable vv; stmt s; read r; while w; call c;\n"
                                "Select w pattern w   ( \"kkk\" ,  _  )    \t";
                unordered_set<string> queryResults1 = spaManager.query(query1);
                // Expected results: 12, 13
                Assert::AreEqual(2, (int) queryResults1.size());
                Assert::IsTrue(queryResults1.find("12") != queryResults1.end());
                Assert::IsTrue(queryResults1.find("13") != queryResults1.end());
            }

            TEST_METHOD(TestWhileIfPatternClausePartOne2) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 2 - Which if statement has iii as control variable?
                string query2 = "assign a; procedure mpp; variable vv; stmt s; read r; while w; call c; if ifs;\n"
                                "Select ifs pattern ifs   ( \"iii\" ,  _ , _ )    \t";
                unordered_set<string> queryResults2 = spaManager.query(query2);
                // Expected results: 11
                Assert::AreEqual(1, (int) queryResults2.size());
                Assert::IsTrue(queryResults2.find("11") != queryResults2.end());
            }

            TEST_METHOD(TestWhileIfPatternClausePartOne3) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 3 - Using variable synonyms. Select all ifs that use a variable
                string query3 = "assign a; procedure mpp; variable vv; stmt s; read r; while w; call c; if ifs;\n"
                                "Select ifs pattern ifs   ( vv ,  _ , _ )  \t";
                unordered_set<string> queryResults3 = spaManager.query(query3);
                // Expected results: 11
                Assert::AreEqual(2, (int) queryResults3.size());
                Assert::IsTrue(queryResults3.find("11") != queryResults3.end());
                Assert::IsTrue(queryResults3.find("15") != queryResults3.end());
            }

            TEST_METHOD(TestWhileIfPatternClausePartOne4) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 4 - Using variable synonyms. Select all the variables that appear in an ifs
                string query4 = "assign a; procedure mpp; variable vv; stmt s; read r; while w; call c; if ifs;\n"
                                "Select vv pattern ifs   ( vv ,  _ , _ )  \t";
                unordered_set<string> queryResults4 = spaManager.query(query4);
                // Expected results: iii
                Assert::AreEqual(2, (int) queryResults4.size());
                Assert::IsTrue(queryResults4.find("iii") != queryResults4.end());
                Assert::IsTrue(queryResults4.find("yyy") != queryResults4.end());
            }

            TEST_METHOD(TestWhileIfPatternClausePartOne5) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 5 - Using variable synonyms. Select all the variables that appear in an while stmt
                string query5 = "assign a; procedure mpp; variable vv; stmt s; read r; while w; call c; if ifs;\n"
                                "Select vv pattern w   ( vv ,  _ )  \t";
                unordered_set<string> queryResults5 = spaManager.query(query5);
                // Expected results: kkk
                Assert::AreEqual(1, (int) queryResults5.size());
                Assert::IsTrue(queryResults5.find("kkk") != queryResults5.end());
            }

            TEST_METHOD(TestWhileIfPatternClausePartOne6) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 6 - Using variable synonyms. Select all the while stmt that use a variable
                string query6 = "assign a; procedure mpp; variable vv; stmt s; read r; while w; call c; if ifs;\n"
                                "Select w pattern w   ( vv ,  _ )  \t";
                unordered_set<string> queryResults6 = spaManager.query(query6);
                // Expected results: 12, 13
                Assert::AreEqual(2, (int) queryResults6.size());
                Assert::IsTrue(queryResults6.find("12") != queryResults6.end());
                Assert::IsTrue(queryResults6.find("13") != queryResults6.end());
            }

            TEST_METHOD(TestWhileIfPatternClausePartOne7) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 7 - Multiple clauses including pattern
                string query7 = "assign a; procedure mpp; variable vv; stmt s; read r; while w; call c; if ifs;\n"
                                "Select w pattern w   ( vv ,  _ )  with w.stmt# = 12 \t";
                unordered_set<string> queryResults7 = spaManager.query(query7);
                // Expected results: 12
                Assert::AreEqual(1, (int) queryResults7.size());
                Assert::IsTrue(queryResults7.find("12") != queryResults7.end());
            }

            TEST_METHOD(TestWhileIfPatternClausePartOne8) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 8 - Multiple clauses including pattern
                string query8 = "assign a; procedure mpp; variable vv; stmt s; read r; while w; call c; if ifs;\n"
                                "Select w pattern w   ( vv ,  _ )  with w.stmt# = 12 with w.stmt# = 13 \t";
                unordered_set<string> queryResults8 = spaManager.query(query8);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults8.size());
            }

            TEST_METHOD(TestWhileIfPatternClausePartOne9) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 9 - Multiple clauses including pattern for assign
                string query9 = "assign a; procedure mpp; variable vv; stmt s; read r; while w; call c; if ifs;\n"
                                "Select a pattern a   ( vv ,  _ )  pattern w (vv, _)  \t";
                unordered_set<string> queryResults9 = spaManager.query(query9);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults9.size());
            }

            TEST_METHOD(TestWhileIfPatternClausePartOne10) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 10 - Multiple clauses including pattern for assign
                string query10 = "assign a; procedure mpp; variable vv; stmt s; read r; while w; call c; if ifs;\n"
                                 "Select a pattern a   ( vv ,  _ )  pattern ifs (vv,  _ , _) \t";
                unordered_set<string> queryResults10 = spaManager.query(query10);
                // Expected results: none
                Assert::AreEqual(0, (int) queryResults10.size());
            }

            TEST_METHOD(TestWhileIfPatternClausePartOne11) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 11 - Get ifs that has a control variable and also modifies a variable
                string query11 = "assign a; procedure mpp; variable vv; stmt s; read r; while w; call c; if ifs;\n"
                                 "Select ifs pattern ifs (vv,  _ , _) such that Modifies(ifs, _) \t";
                unordered_set<string> queryResults11 = spaManager.query(query11);
                // Expected results: 11
                Assert::AreEqual(2, (int) queryResults11.size());
                Assert::IsTrue(queryResults11.find("11") != queryResults11.end());
                Assert::IsTrue(queryResults11.find("15") != queryResults11.end());
            }

            TEST_METHOD(TestWhileIfPatternClausePartOne12) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 12 - Get while that has a control variable and also modifies a variable
                string query12 = "assign a; procedure mpp; variable vv; stmt s; read r; while w; call c; if ifs;\n"
                                 "Select w pattern w (vv,  _ ) such that Modifies(w, _) \t";
                unordered_set<string> queryResults12 = spaManager.query(query12);
                // Expected results: 12, 13
                Assert::AreEqual(2, (int) queryResults12.size());
                Assert::IsTrue(queryResults12.find("12") != queryResults12.end());
                Assert::IsTrue(queryResults12.find("13") != queryResults12.end());
            }
    };
}
