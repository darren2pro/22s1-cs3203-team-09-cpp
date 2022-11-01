#include "stdafx.h"
#include "CppUnitTest.h"
#include <SPAManager/SPAManager.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace IntegrationTesting {
    TEST_CLASS(TestBooleanTuple) {
            private:
            string getCurrentProgram(int ref) {
                string program1 = "procedure main{\n"
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
                string program2 = "procedure read {\n"
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
                // You can add more program strings here and add more switch cases here
                switch (ref) {
                    case 1:
                        return program1;
                    case 2:
                        //! TestPatternMatchFull
                        return program2;
                    default:
                        return "";
                }
            }

            public:
            TEST_METHOD(TestBoolean1) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1
                string query1 = "procedure sppp; \n"
                                "Select BOOLEAN such that Modifies(sppp, \"cenX\")";
                unordered_set <string> results1 = spaManager.query(query1);
                // Expected results: TRUE
                Assert::AreEqual(1, (int) results1.size());
                Assert::IsTrue(results1.find("TRUE") != results1.end());
            }

            TEST_METHOD(TestBoolean2) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 2
                string query2 = "procedure p; \n"
                                "Select BOOLEAN      such that Modifies(p, \"x\")";
                unordered_set <string> results2 = spaManager.query(query2);
                // Expected results: TRUE
                Assert::AreEqual(1, (int) results2.size());
                Assert::IsTrue(results2.find("TRUE") != results2.end());
            }

            TEST_METHOD(TestBoolean3) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 3
                string query3 = "procedure p; variable v; \n"
                                "Select BOOLEAN such that Modifies(\"readPoint\", v)";
                unordered_set <string> results3 = spaManager.query(query3);
                // Expected results: TRUE
                Assert::AreEqual(1, (int) results3.size());
                Assert::IsTrue(results3.find("TRUE") != results3.end());
            }

            TEST_METHOD(TestBoolean4) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 4
                string query4 = "procedure p; variable v; call ccc; \n  "
                                "Select BOOLEAN  such that Modifies(\"printResults\", v)";
                unordered_set <string> results4 = spaManager.query(query4);
                // Expected results: FALSE
                Assert::AreEqual(1, (int) results4.size());
                Assert::IsTrue(results4.find("FALSE") != results4.end());
            }

            TEST_METHOD(TestBoolean5) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 5
                string query5 = "procedure p; variable v; call ccc; \n  "
                                "Select BOOLEAN  such that Modifies(\"main\", \"random\")";
                unordered_set <string> results5 = spaManager.query(query5);
                // Expected results: FALSE
                Assert::AreEqual(1, (int) results5.size());
                Assert::IsTrue(results5.find("FALSE") != results5.end());
            }

            TEST_METHOD(TestBoolean6) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 6
                string query6 = "procedure p; variable BOOLEAN; \n    "
                                "Select BOOLEAN such that Modifies(\"readPoint\", BOOLEAN)";
                unordered_set <string> results6 = spaManager.query(query6);
                // Expected results: x, y
                Assert::AreEqual(2, (int) results6.size());
                Assert::IsTrue(results6.find("x") != results6.end());
                Assert::IsTrue(results6.find("y") != results6.end());
            }

            TEST_METHOD(TestBoolean7) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 7
                string query7 = "procedure p; variable BOOLEAN; call ccc; \n   "
                                "Select BOOLEAN such that Modifies(\"printResults\", BOOLEAN)";
                unordered_set<string> results7 = spaManager.query(query7);
                // Expected results: empty
                Assert::AreEqual(0, (int) results7.size());
            }

            TEST_METHOD(TestSelectTuple1) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1
                string query1 = "procedure p; while w; variable v; assign a; \n"
                                "Select <w, p> pattern a(v, _) pattern a(_, _\"cenY\"_) pattern w(\"y\", _) pattern w(\"x\", _)";
                unordered_set<string> results1 = spaManager.query(query1);
                // Expected results: 15 main, 15 readPoint, 15 printResults, 15 computeCentroid
                Assert::AreEqual(4, (int) results1.size());
                Assert::IsTrue(results1.find("15 main") != results1.end());
                Assert::IsTrue(results1.find("15 readPoint") != results1.end());
                Assert::IsTrue(results1.find("15 printResults") != results1.end());
                Assert::IsTrue(results1.find("15 computeCentroid") != results1.end());
            }

            TEST_METHOD(TestSelectTuple2) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 2
                string query2 = "procedure sppp;  call ccc;  \n"
                                "Select <sppp, ccc> such that Modifies(sppp, \"cenX\")";
                unordered_set<string> results2 = spaManager.query(query2);
                // Expected results: computeCentroid 2, computeCentroid 3, computeCentroid 14, computeCentroid 19, main 2, main 3, main 14, main 19
                Assert::AreEqual(8, (int) results2.size());
                Assert::IsTrue(results2.find("computeCentroid 2") != results2.end());
                Assert::IsTrue(results2.find("computeCentroid 3") != results2.end());
                Assert::IsTrue(results2.find("computeCentroid 14") != results2.end());
                Assert::IsTrue(results2.find("computeCentroid 19") != results2.end());
                Assert::IsTrue(results2.find("main 2") != results2.end());
                Assert::IsTrue(results2.find("main 3") != results2.end());
                Assert::IsTrue(results2.find("main 14") != results2.end());
                Assert::IsTrue(results2.find("main 19") != results2.end());
            }

            TEST_METHOD(TestSelectTuple3) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 3
                string query3 = "procedure p; variable v;  read rr; \n"
                                "Select <v, p, rr> such that Modifies(\"readPoint\", v)";
                unordered_set <string> results3 = spaManager.query(query3);
                /*
                 * Expected results:
                 * x main 4, x readPoint 4, x printResults 4, x computeCentroid 4,
                 * x main 5, x readPoint 5, x printResults 5, x computeCentroid 5,
                 * y main 4, y readPoint 4, y printResults 4, y computeCentroid 4,
                 * y main 5, y readPoint 5, y printResults 5, y computeCentroid 5
                 */
                Assert::AreEqual(16, (int) results3.size());
                Assert::IsTrue(results3.find("x main 4") != results3.end());
                Assert::IsTrue(results3.find("x readPoint 4") != results3.end());
                Assert::IsTrue(results3.find("x printResults 4") != results3.end());
                Assert::IsTrue(results3.find("x computeCentroid 4") != results3.end());
                Assert::IsTrue(results3.find("x main 5") != results3.end());
                Assert::IsTrue(results3.find("x readPoint 5") != results3.end());
                Assert::IsTrue(results3.find("x printResults 5") != results3.end());
                Assert::IsTrue(results3.find("x computeCentroid 5") != results3.end());
                Assert::IsTrue(results3.find("y main 4") != results3.end());
                Assert::IsTrue(results3.find("y readPoint 4") != results3.end());
                Assert::IsTrue(results3.find("y printResults 4") != results3.end());
                Assert::IsTrue(results3.find("y computeCentroid 4") != results3.end());
                Assert::IsTrue(results3.find("y main 5") != results3.end());
                Assert::IsTrue(results3.find("y readPoint 5") != results3.end());
                Assert::IsTrue(results3.find("y printResults 5") != results3.end());
                Assert::IsTrue(results3.find("y computeCentroid 5") != results3.end());
            }
    };
}
