#include "stdafx.h"
#include "CppUnitTest.h"
#include <SPAManager/SPAManager.h>
#include <QueryProcessingSubsystem/Validator/SemanticException.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace IntegrationTesting {
    TEST_CLASS(TestAffects) {
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
                string program2 = "procedure Second {\n"
                                  "                                x = 0;\n" // line 1
                                  "                                i = 5;\n" // line 2
                                  "                                while (i!=0) {\n" // line 3
                                  "                                        x = x + 2*y;\n" // line 4
                                  "                                        call Third;\n" // line 5
                                  "                                        i = i - 1; }\n" // line 6
                                  "                                if (x==1) then {\n" // line 7
                                  "                                        x = x+1; }\n" // line 8
                                  "                                else {\n"
                                  "                                        z = 1; }\n" // line 9
                                  "                                z = z + x + i;\n" // line 10
                                  "                                y = z + 2;\n" // line 11
                                  "                                x = x * y + z; }\n" // line 12
                                  "procedure Third {\n"
                                  "        call Fourth;\n" // line 13
                                  "}\n  ";

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
            TEST_METHOD(TestAffects1) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1
                string query1 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; \n "
                                "Select <a1, a2> such that Affects(ifs2, ifs2)";
                try {
                    unordered_set<string> result1 = spaManager.query(query1);
                    /*
                    * Expected semantic error
                    */
                    Assert::Fail(L"Expected a semantic exception because Affects only works for assign and stmt");
                } catch (SemanticError& e) {
                    Logger::WriteMessage("Query 1 (Correct) Exception seen: ");
                    Logger::WriteMessage(e.what());
                }
            }

            TEST_METHOD(TestAffects2) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 2
                string query2 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; \n "
                                "Select <a1, a2> such that Affects(9, 11)";
                /*
                 * Expected results: empty
                 */
                unordered_set<string> result2 = spaManager.query(query2);
                Assert::AreEqual(0, (int) result2.size());
            }

            TEST_METHOD(TestAffects3) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 3
                string query3 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; \n "
                                "Select <a1, a2> such that Affects(9, 10)";
                /*
                 * Expected results: All assignment pairs in this program
                 * 1 1, 1 2, 1 4, 1 6, 1 8, 1 9, 1 10, 1 11, 1 12
                 * 2 1, 2 2, 2 4, 2 6, 2 8, 2 9, 2 10, 2 11, 2 12
                 * 4 1, 4 2, 4 4, 4 6, 4 8, 4 9, 4 10, 4 11, 4 12
                 * 6 1, 6 2, 6 4, 6 6, 6 8, 6 9, 6 10, 6 11, 6 12
                 * 8 1, 8 2, 8 4, 8 6, 8 8, 8 9, 8 10, 8 11, 8 12
                 * 9 1, 9 2, 9 4, 9 6, 9 8, 9 9, 9 10, 9 11, 9 12
                 * 10 1, 10 2, 10 4, 10 6, 10 8, 10 9, 10 10, 10 11, 10 12
                 * 11 1, 11 2, 11 4, 11 6, 11 8, 11 9, 11 10, 11 11, 11 12
                 * 12 1, 12 2, 12 4, 12 6, 12 8, 12 9, 12 10, 12 11, 12 12
                 */
                unordered_set<string> result3 = spaManager.query(query3);
                Assert::AreEqual(81, (int) result3.size());
                //! Test random results
                Assert::IsTrue(result3.find("1 1") != result3.end());
                Assert::IsTrue(result3.find("1 12") != result3.end());
                Assert::IsTrue(result3.find("8 4") != result3.end());
                Assert::IsTrue(result3.find("9 6") != result3.end());
                Assert::IsTrue(result3.find("12 8") != result3.end());
                Assert::IsTrue(result3.find("11 9") != result3.end());
                Assert::IsTrue(result3.find("11 10") != result3.end());
                Assert::IsTrue(result3.find("12 11") != result3.end());
                Assert::IsTrue(result3.find("12 12") != result3.end());
            }

            TEST_METHOD(TestAffects4) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 4
                string query4 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; \n "
                                "Select <a1, a2> such that Affects(9, 12)";
                /*
                 * Expected results: empty
                 */
                unordered_set<string> result4 = spaManager.query(query4);
                Assert::AreEqual(0, (int) result4.size());
            }

            TEST_METHOD(TestAffects5) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 5
                string query5 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; \n "
                                "Select BOOLEAN such that Affects(9, 9)";
                /*
                 * Expected results: FALSE
                 */
                unordered_set<string> result5 = spaManager.query(query5);
                Assert::AreEqual(1, (int) result5.size());
                Assert::IsTrue(result5.find("FALSE") != result5.end());
            }

            TEST_METHOD(TestAffects6) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 6
                string query6 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; \n "
                                "Select <a1.stmt#, a2.stmt#> such that Affects(a1, a2)";
                /*
                 * Expected results: 1 4, 1 8, 1 10, 1 12
                 * 2 6, 2 10, 4 4, 4 8, 4 10, 4 12, 6 6, 6 10, 8 10, 8 12, 9 10, 10 11, 10 12, 11 12
                 */
                unordered_set<string> result6 = spaManager.query(query6);
                Assert::AreEqual(18, (int) result6.size());
                //! Test random results
                Assert::IsTrue(result6.find("1 4") != result6.end());
                Assert::IsTrue(result6.find("1 8") != result6.end());
                Assert::IsTrue(result6.find("1 10") != result6.end());
                Assert::IsTrue(result6.find("1 12") != result6.end());
                Assert::IsTrue(result6.find("2 6") != result6.end());
                Assert::IsTrue(result6.find("2 10") != result6.end());
                Assert::IsTrue(result6.find("4 4") != result6.end());
                Assert::IsTrue(result6.find("4 8") != result6.end());
                Assert::IsTrue(result6.find("4 10") != result6.end());
                Assert::IsTrue(result6.find("4 12") != result6.end());
                Assert::IsTrue(result6.find("6 6") != result6.end());
                Assert::IsTrue(result6.find("6 10") != result6.end());
                Assert::IsTrue(result6.find("8 10") != result6.end());
                Assert::IsTrue(result6.find("8 12") != result6.end());
                Assert::IsTrue(result6.find("9 10") != result6.end());
                Assert::IsTrue(result6.find("10 11") != result6.end());
                Assert::IsTrue(result6.find("10 12") != result6.end());
                Assert::IsTrue(result6.find("11 12") != result6.end());
            }

            TEST_METHOD(TestAffects7) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 7
                string query7 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; \n "
                                "Select <a1.stmt#, a2> such that Affects*(a1, a2)";
                /*
                 * Expected results: All assignment pairs in this program
                 */
                unordered_set<string> result7 = spaManager.query(query7);
                Assert::AreEqual(27, (int) result7.size());
            }

            TEST_METHOD(TestAffects8) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Recomputing query 6 for comparison
                string query6 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; \n "
                                "Select <a1.stmt#, a2.stmt#> such that Affects(a1, a2)";
                /*
                 * Expected results: 1 4, 1 8, 1 10
                 * 2 6, 2 10, 4 8, 4 10, 6 6, 6 10, 8 10, 8 12, 9 10, 10 11, 10 12, 11 12
                 */
                unordered_set<string> result6 = spaManager.query(query6);
                Assert::AreEqual(18, (int) result6.size());

                ////! Now for Query 8
                //string query8 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; \n "
                //                "Select <ss1, ss2> such that Affects(ss1, ss2)";
                ///*
                // * Expected results: Same 15 results as query 6
                // */
                //unordered_set<string> result8 = spaManager.query(query8);
                //Assert::IsTrue(result6 == result8);
            }

            TEST_METHOD(TestAffects9) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 9
                string query9 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; \n "
                                "Select <ss1.stmt#, ss2> such that Affects(2, 3)";
                /*
                 * Expected results: empty
                 */
                unordered_set<string> result9 = spaManager.query(query9);
                Assert::AreEqual(0, (int) result9.size());
            }

            TEST_METHOD(TestAffects10) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 10
                string query10 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p; \n "
                                 "Select p.procName such that Affects(2, _)";
                /*
                 * Expected results: Second, Third
                 */
                unordered_set<string> result10 = spaManager.query(query10);
                Assert::AreEqual(2, (int) result10.size());
                Assert::IsTrue(result10.find("Second") != result10.end());
                Assert::IsTrue(result10.find("Third") != result10.end());
            }

            TEST_METHOD(TestAffects11) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 11
                string query11 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p; \n "
                                 "Select ss2.stmt# such that Affects(_, 2)";
                /*
                 * Expected results: empty
                 */
                unordered_set<string> result11 = spaManager.query(query11);
                Assert::AreEqual(0, (int) result11.size());
            }

            TEST_METHOD(TestAffects12) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 12
                string query12 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p;  \n "
                                 "Select ss2.stmt# such that Affects(3, _)";
                /*
                 * Expected results: empty
                 */
                unordered_set<string> result12 = spaManager.query(query12);
                Assert::AreEqual(0, (int) result12.size());
            }

            TEST_METHOD(TestAffects13) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 13
                string query13 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p;  \n "
                                 "Select ss2.stmt# such that Affects(_, 3)";
                /*
                 * Expected results: empty
                 */
                unordered_set<string> result13 = spaManager.query(query13);
                Assert::AreEqual(0, (int) result13.size());
            }

            TEST_METHOD(TestAffects14) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 14
                string query14 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p;  \n "
                                 "Select <a1.stmt#, a1.stmt#, p, p.procName> such that Affects(2, a1)";
                /*
                 * Possible results for a1: 6 and 10
                 * Possible results for p: Second and Third
                 * Expected results:
                 * 6 6 Second Second, 10 10 Second Second, 6 6 Third Third, 10 10 Third Third
                 */
                unordered_set<string> result14 = spaManager.query(query14);
                Assert::AreEqual(4, (int) result14.size());
                Assert::IsTrue(result14.find("6 6 Second Second") != result14.end());
                Assert::IsTrue(result14.find("10 10 Second Second") != result14.end());
                Assert::IsTrue(result14.find("6 6 Third Third") != result14.end());
                Assert::IsTrue(result14.find("10 10 Third Third") != result14.end());
            }

            TEST_METHOD(TestAffects15) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 15
                string query15 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p;  \n "
                                 "Select <a1.stmt#, a1.stmt#, p, p.procName> such that Affects(a1, 10)";
                /*
                 * Possible results for a1: 1, 2, 4, 6, 8, 9
                 * Possible results for p: Second, Third
                 * Expected results:
                 * 1 1 Second Second, 2 2 Second Second, 4 4 Second Second, 6 6 Second Second, 8 8 Second Second, 9 9 Second Second,
                 * 1 1 Third Third, 2 2 Third Third, 4 4 Third Third, 6 6 Third Third, 8 8 Third Third, 9 9 Third Third
                 */
                unordered_set<string> result15 = spaManager.query(query15);
                Assert::AreEqual(12, (int) result15.size());
                Assert::IsTrue(result15.find("1 1 Second Second") != result15.end());
                Assert::IsTrue(result15.find("2 2 Second Second") != result15.end());
                Assert::IsTrue(result15.find("4 4 Second Second") != result15.end());
                Assert::IsTrue(result15.find("6 6 Second Second") != result15.end());
                Assert::IsTrue(result15.find("8 8 Second Second") != result15.end());
                Assert::IsTrue(result15.find("9 9 Second Second") != result15.end());
                Assert::IsTrue(result15.find("1 1 Third Third") != result15.end());
                Assert::IsTrue(result15.find("2 2 Third Third") != result15.end());
                Assert::IsTrue(result15.find("4 4 Third Third") != result15.end());
                Assert::IsTrue(result15.find("6 6 Third Third") != result15.end());
                Assert::IsTrue(result15.find("8 8 Third Third") != result15.end());
                Assert::IsTrue(result15.find("9 9 Third Third") != result15.end());
            }

            TEST_METHOD(TestAffects16) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 16
                string query16 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p;  \n "
                                 "Select <ifs1.stmt#, ifs2, ifs1, ifs2, ifs2> such that Affects(_, _)";
                /*
                 * Possible results for ifs1: 7
                 * Possible results for ifs2: 7
                 * Expected results:
                 * 7 7 7 7 7
                 */
                unordered_set<string> result16 = spaManager.query(query16);
                Assert::AreEqual(1, (int) result16.size());
                Assert::IsTrue(result16.find("7 7 7 7 7") != result16.end());
            }

            TEST_METHOD(TestAffects17) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 17
                string query17 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p; call cc1, cc2; \n "
                                 "Select BOOLEAN such that Affects*(cc2, cc2)";
                try {
                    unordered_set<string> result17 = spaManager.query(query17);
                    /*
                    * Expected semantic error
                    */
                    Assert::Fail(L"Expected a semantic exception because Affects only works for assign and stmt");
                } catch (SemanticError& e) {
                    Logger::WriteMessage("Query 17 (Correct) Exception seen: ");
                    Logger::WriteMessage(e.what());
                }
            }

            TEST_METHOD(TestAffects18) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 18
                string query18 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p; call cc1, cc2; \n "
                                 "Select BOOLEAN such that Affects*(cc2, 2)";
                try {
                    unordered_set<string> result18 = spaManager.query(query18);
                    /*
                    * Expected semantic error
                    */
                    Assert::Fail(L"Expected a semantic exception because Affects only works for assign and stmt");
                } catch (SemanticError& e) {
                    Logger::WriteMessage("Query 18 (Correct) Exception seen: ");
                    Logger::WriteMessage(e.what());
                }
            }

            TEST_METHOD(TestAffects19) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 19
                string query19 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p; call cc1, cc2; \n "
                                 "Select BOOLEAN such that Affects*(2, cc2)";
                try {
                    unordered_set<string> result19 = spaManager.query(query19);
                    /*
                    * Expected semantic error
                    */
                    Assert::Fail(L"Expected a semantic exception because Affects only works for assign and stmt");
                } catch (SemanticError& e) {
                    Logger::WriteMessage("Query 19 (Correct) Exception seen: ");
                    Logger::WriteMessage(e.what());
                }
            }

            TEST_METHOD(TestAffects20) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 20
                string query20 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p; call cc1, cc2; \n "
                                 "Select <a1, a2, a1.stmt#, a2.stmt#, a1> such that Affects*(a1, a2)";
                /*
                 * Expected results for a1 a2:
                 * 1 4, 1 8, 1 10, 1 11, 1 12,
                 * 2 6, 2 10, 2 11, 2 12,
                 * 4 4, 4 8, 4 10, 4 11, 4 12,
                 * 6 6, 6 10, 6 11, 6 12,
                 * 8 10, 8 11, 8 12,
                 * 9 10, 9 11, 9 12,
                 * 10 11, 10 12,
                 * 11 12
                 *
                 * Expected results for <a1, a2, a1.stmt#, a2.stmt#, a1>:
                 * 1 4 1 4 1, 1 8 1 8 1, 1 10 1 10 1, 1 11 1 11 1, 1 12 1 12 1,
                 * 2 6 2 6 2, 2 10 2 10 2, 2 11 2 11 2, 2 12 2 12 2,
                 * 4 8 4 8 4, 4 10 4 10 4, 4 11 4 11 4, 4 12 4 12 4,
                 * 6 6 6 6 6, 6 10 6 10 6, 6 11 6 11 6, 6 12 6 12 6,
                 * 8 10 8 10 8, 8 11 8 11 8, 8 12 8 12 8,
                 * 9 10 9 10 9, 9 11 9 11 9, 9 12 9 12 9,
                 * 10 11 10 11 10, 10 12 10 12 10,
                 * 11 12 11 12 11
                 */
                unordered_set<string> result20 = spaManager.query(query20);
                Assert::AreEqual(27, (int) result20.size());
                Assert::IsTrue(result20.find("1 4 1 4 1") != result20.end());
                Assert::IsTrue(result20.find("1 8 1 8 1") != result20.end());
                Assert::IsTrue(result20.find("1 10 1 10 1") != result20.end());
                Assert::IsTrue(result20.find("1 11 1 11 1") != result20.end());
                Assert::IsTrue(result20.find("1 12 1 12 1") != result20.end());
                Assert::IsTrue(result20.find("2 6 2 6 2") != result20.end());
                Assert::IsTrue(result20.find("2 10 2 10 2") != result20.end());
                Assert::IsTrue(result20.find("2 11 2 11 2") != result20.end());
                Assert::IsTrue(result20.find("2 12 2 12 2") != result20.end());
                Assert::IsTrue(result20.find("4 8 4 8 4") != result20.end());
                Assert::IsTrue(result20.find("4 10 4 10 4") != result20.end());
                Assert::IsTrue(result20.find("4 11 4 11 4") != result20.end());
                Assert::IsTrue(result20.find("4 12 4 12 4") != result20.end());
                Assert::IsTrue(result20.find("6 6 6 6 6") != result20.end());
                Assert::IsTrue(result20.find("6 10 6 10 6") != result20.end());
                Assert::IsTrue(result20.find("6 11 6 11 6") != result20.end());
                Assert::IsTrue(result20.find("6 12 6 12 6") != result20.end());
                Assert::IsTrue(result20.find("8 10 8 10 8") != result20.end());
                Assert::IsTrue(result20.find("8 11 8 11 8") != result20.end());
                Assert::IsTrue(result20.find("8 12 8 12 8") != result20.end());
                Assert::IsTrue(result20.find("9 10 9 10 9") != result20.end());
                Assert::IsTrue(result20.find("9 11 9 11 9") != result20.end());
                Assert::IsTrue(result20.find("9 12 9 12 9") != result20.end());
                Assert::IsTrue(result20.find("10 11 10 11 10") != result20.end());
                Assert::IsTrue(result20.find("10 12 10 12 10") != result20.end());
                Assert::IsTrue(result20.find("11 12 11 12 11") != result20.end());
            }

            TEST_METHOD(TestAffects21) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 21
                string query21 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p; call cc1, cc2; \n "
                                 "Select <a1, a2, a1.stmt#, a2.stmt#, a1> such that Affects*(a1, a2) with a1.stmt# = 1";
                /*
                 * Expected results (filtered from query 20):
                 * 1 4 1 4 1, 1 8 1 8 1, 1 10 1 10 1, 1 11 1 11 1, 1 12 1 12 1
                 */
                unordered_set<string> result21 = spaManager.query(query21);
                Assert::AreEqual(5, (int) result21.size());
                Assert::IsTrue(result21.find("1 4 1 4 1") != result21.end());
                Assert::IsTrue(result21.find("1 8 1 8 1") != result21.end());
                Assert::IsTrue(result21.find("1 10 1 10 1") != result21.end());
                Assert::IsTrue(result21.find("1 11 1 11 1") != result21.end());
                Assert::IsTrue(result21.find("1 12 1 12 1") != result21.end());
            }

            TEST_METHOD(TestAffects22) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 22
                string query22 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p; call cc1, cc2; \n "
                                 "Select <a1, a2> such that Affects*(9, 11)    with 8 = a1.stmt# \t  ";
                /*
                 * Expected results: All possible pairs of assignment statements with 8 = a1.stmt#
                 * 8 1, 8 2, 8 4, 8 6, 8 8, 8 9, 8 10, 8 11, 8 12
                 */
                unordered_set<string> result22 = spaManager.query(query22);
                Assert::AreEqual(9, (int) result22.size());
                Assert::IsTrue(result22.find("8 1") != result22.end());
                Assert::IsTrue(result22.find("8 2") != result22.end());
                Assert::IsTrue(result22.find("8 4") != result22.end());
                Assert::IsTrue(result22.find("8 6") != result22.end());
                Assert::IsTrue(result22.find("8 8") != result22.end());
                Assert::IsTrue(result22.find("8 9") != result22.end());
                Assert::IsTrue(result22.find("8 10") != result22.end());
                Assert::IsTrue(result22.find("8 11") != result22.end());
                Assert::IsTrue(result22.find("8 12") != result22.end());
            }

            TEST_METHOD(TestAffects23) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 23
                string query23 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p; call cc1, cc2; \n "
                                 "Select <a1, a2> such that Affects*(9, 10) with a1.stmt# = 10";
                /*
                 * Expected results: All possible pairs of <a1, a2> such that a1.stmt# = 10
                 * 10 1, 10 2, 10 4, 10 6, 10 8, 10 9, 10 10, 10 11, 10 12
                 */
                unordered_set<string> result23 = spaManager.query(query23);
                Assert::AreEqual(9, (int) result23.size());
                Assert::IsTrue(result23.find("10 1") != result23.end());
                Assert::IsTrue(result23.find("10 2") != result23.end());
                Assert::IsTrue(result23.find("10 4") != result23.end());
                Assert::IsTrue(result23.find("10 6") != result23.end());
                Assert::IsTrue(result23.find("10 8") != result23.end());
                Assert::IsTrue(result23.find("10 9") != result23.end());
                Assert::IsTrue(result23.find("10 10") != result23.end());
                Assert::IsTrue(result23.find("10 11") != result23.end());
                Assert::IsTrue(result23.find("10 12") != result23.end());
            }

            TEST_METHOD(TestAffects24) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 24
                string query24 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p; call cc1, cc2; \n "
                                 "Select <a1, a2, ifs1.stmt#> such that Affects*(9, 12) with 10 = a1.stmt# and 11   = a2.stmt# with 1 = 2";
                /*
                 * Expected results: empty
                 */
                unordered_set<string> result24 = spaManager.query(query24);
                Assert::AreEqual(0, (int) result24.size());
            }

            TEST_METHOD(TestAffects25) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 25
                string query25 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p; call cc1, cc2; \n "
                                 "Select <a2, cc2, a1, cc2.stmt#> such that Affects*(9, 12) with 10 = a1.stmt# and 11 = a2.stmt# with 1 = 1";
                /*
                 * Expected results: 10 5 11 5, 10 13 11 13
                 */
                unordered_set<string> result25 = spaManager.query(query25);
                Assert::AreEqual(2, (int) result25.size());
                Assert::IsTrue(result25.find("11 5 10 5") != result25.end());
                Assert::IsTrue(result25.find("11 13 10 13") != result25.end());
            }

            TEST_METHOD(TestAffects26) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 26
                string query26 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p; call cc1, cc2; \n "
                                 "Select <a1, a2, cc2.procName> such that Affects*(9, 12) with 10 = a1.stmt# and 11   = a2.stmt# with 1 = 1";
                /*
                 * Expected results: 10 11 Third, 10 11 Fourth
                 */
                unordered_set<string> result26 = spaManager.query(query26);
                Assert::AreEqual(2, (int) result26.size());
                Assert::IsTrue(result26.find("10 11 Third") != result26.end());
                Assert::IsTrue(result26.find("10 11 Fourth") != result26.end());
            }

            TEST_METHOD(TestAffects27) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 27
                string query27 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p; call cc1, cc2; variable v; \n "
                                 "Select <a1, a2, v.varName> such that Affects*(9, _) with 11 = a1.stmt# and 11   = a2.stmt# with 11 = 11";
                /*
                 * Expected results:
                 * 11 11 x, 11 11 i, 11 11 y, 11 11 z
                 */
                unordered_set<string> result27 = spaManager.query(query27);
                Assert::AreEqual(4, (int) result27.size());
                Assert::IsTrue(result27.find("11 11 x") != result27.end());
                Assert::IsTrue(result27.find("11 11 i") != result27.end());
                Assert::IsTrue(result27.find("11 11 y") != result27.end());
                Assert::IsTrue(result27.find("11 11 z") != result27.end());
            }

            TEST_METHOD(TestAffects28) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 28
                string query28 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p; call cc1, cc2; variable v; \n "
                                 "Select <a1, a2, v.varName> such that Affects*(9, _) with 11 = a1.stmt# and 11   = a2.stmt# and 11 = 11 and v.varName = \"y\"";
                /*
                 * Expected results:
                 * 11 11 y
                 */
                unordered_set<string> result28 = spaManager.query(query28);
                Assert::AreEqual(1, (int) result28.size());
                Assert::IsTrue(result28.find("11 11 y") != result28.end());
            }

            TEST_METHOD(TestAffects29) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 29
                string query29 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p; call cc1, cc2; variable v; \n "
                                 "Select <a1, a2, v.varName> such that Affects*(_, 9) and Affects*(_, _) with 11 = a1.stmt# and 11   = a2.stmt# and 11 = 11 and v.varName = \"y\" with 1 = 1";
                /*
                 * Expected results:
                 * empty
                 */
                unordered_set<string> result29 = spaManager.query(query29);
                Assert::AreEqual(0, (int) result29.size());
            }

            TEST_METHOD(TestAffects30) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 30
                string query30 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p; call cc1, cc2; variable v; \n "
                                 "Select <a1, a2, v.varName> such that Affects*(_, 4) and Affects*(_, _) with 11 = a1.stmt# and 11   = a2.stmt# and 11 = 11 and v.varName = \"y\"";
                /*
                 * Expected results:
                 * 11 11 y
                 */
                unordered_set<string> result30 = spaManager.query(query30);
                Assert::AreEqual(1, (int) result30.size());
                Assert::IsTrue(result30.find("11 11 y") != result30.end());
            }

            TEST_METHOD(TestAffects31) {
                string program = getCurrentProgram(2);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 31
                string query31 = "assign a1, a2; stmt ss1, ss2; if ifs1, ifs2; procedure p; call cc1, cc2; variable v; \n "
                                 "Select <a1, a2, v.varName> such that Affects*(_, _) with 11 = a1.stmt# and 1   = a2.stmt# and 121 = 121 and v.varName = \"z\"";
                /*
                 * Expected results:
                 * 11 1 z
                 */
                unordered_set<string> result31 = spaManager.query(query31);
                Assert::AreEqual(1, (int) result31.size());
                Assert::IsTrue(result31.find("11 1 z") != result31.end());
            }
    };
}
