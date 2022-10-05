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
            BEGIN_TEST_CLASS_ATTRIBUTE(TestQueryMilestone2WithClWhileIfPattern)
            TEST_CLASS_ATTRIBUTE(L"Ignore", L"true")
            END_TEST_CLASS_ATTRIBUTE()

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
            TEST_METHOD(TestWithClause) {
                // TODO: https://github.com/nus-cs3203/project-wiki/wiki/Advanced-SPA-Example-Queries#meaningless-queries
                // TODO: https://github.com/nus-cs3203/project-wiki/wiki/Advanced-SPA-PQL-Discussion#with-clauses
            }
    };
}
