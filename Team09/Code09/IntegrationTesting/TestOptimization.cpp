#include "stdafx.h"
#include "CppUnitTest.h"
#include <SPAManager/SPAManager.h>
#include <chrono>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace chrono;

namespace IntegrationTesting {
    TEST_CLASS(TestOptimization) {
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
            TEST_METHOD(TestOptimization_1) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 1 - All clauses except the final one returns true. If optimized the clause will re-order, should return false immediately.
                string query1 = "assign a, a1, a2; \n"
                                "Select a such that Modifies(a, \"beingModified\") and Uses(a1, \"num1\") and Uses(a1, \"num2\") and Uses(14, \"num20\") ";
                auto start = high_resolution_clock::now();
                unordered_set<string> result1Optimized = spaManager.query(query1);
                auto stop = high_resolution_clock::now();
                auto durationOptimized = duration_cast<microseconds>(stop - start);

                start = high_resolution_clock::now();
                unordered_set<string> result1Unoptimized = spaManager.query(query1, false);
                stop = high_resolution_clock::now();
                auto durationUnoptimized = duration_cast<microseconds>(stop - start);

                Logger::WriteMessage("\tTestOptimization_1\n");
                Logger::WriteMessage(("\tOptimized: " + to_string(durationOptimized.count()) + " microseconds\n").c_str());
                Logger::WriteMessage(("\tUnoptimized: " + to_string(durationUnoptimized.count()) + " microseconds\n\n").c_str());

                // Expected result: empty
                Assert::AreEqual(0, (int) result1Optimized.size());
                Assert::AreEqual(0, (int) result1Unoptimized.size());
            }

            TEST_METHOD(TestOptimization_2) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 2 - The pattern execution should be shifted to the last clause, because it takes the longest.
                //! All other clauses should return some results so that we see whether there are benefits of shifting
                //! The pattern execution to the last clause.

            }

            TEST_METHOD(TestOptimization_3) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 3 - The NextT execution should be shifted to the last clause, because it takes the longest and
                //! is calculated on-the-fly. All other clauses should return some results so that we see whether
                //! there are benefits of shifting the Next/T execution to the last clause.

            }

            TEST_METHOD(TestOptimization_4) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 4 - The Next execution should be shifted to the last clause, because it takes the longest and
                //! is calculated on-the-fly. All other clauses should return some results so that we see whether
                //! there are benefits of shifting the Next/T execution to the last clause.

            }

            TEST_METHOD(TestOptimization_5) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 5 - The AffectsT execution should be shifted to the last clause, because it takes the longest and
                //! is calculated on-the-fly. All other clauses should return some results so that we see whether
                //! there are benefits of shifting the Affects/T execution to the last clause.

            }

            TEST_METHOD(TestOptimization_6) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 6 - The Affects execution should be shifted to the last clause, because it takes the longest and
                //! is calculated on-the-fly. All other clauses should return some results so that we see whether
                //! there are benefits of shifting the Affects/T execution to the last clause.

            }

            TEST_METHOD(TestOptimization_7) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 7 - The With execution should be shifted to the front to be executed. This test case will be
                //! a positive test case, meaning that we expect the query to return some results.

            }

            TEST_METHOD(TestOptimization_8) {
                string program = getCurrentProgram(1);
                SPAManager spaManager;
                spaManager.loadSimpleSourceFromProgram(program);

                //! Query 8 - The With execution should be shifted to the front to be executed. This test case will be
                //! a negative test case, meaning that we expect the query to return no results.

            }
    };
}
