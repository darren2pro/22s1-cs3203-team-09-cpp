#include "stdafx.h"
#include "CppUnitTest.h"
#include <SourceProcessor/Parser.h>
#include <SourceProcessor/astBuilder/SimpleAstBuilder.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting {
    TEST_CLASS(TestAstBuilder) {
            public:
            TEST_METHOD(TestPureAssignmentStatements) {
                string program = "procedure main1 {\n"
                                 "    x = 1;\n"
                                 "    y = y + 34;\n"
                                 "    z = k + 123;\n"
                                 "    a = x + y;\n"
                                 "}\n\n";
                std::istringstream iss(program);
                SimpleParser simpleParser(&iss);
                Parser* parser = &simpleParser;
                Parser::SOURCE_CODE_TOKENS result = parser->getTokens();

                SimpleAstBuilder astBuilder(result);
                AST programNode = astBuilder.build();

                Assert::AreEqual(programNode->procList.size(), (size_t) 1);
                Assert::AreEqual(programNode->procList[0]->procName, std::string("main1"));
                Assert::AreEqual(programNode->procList[0]->stmtList.size(), (size_t) 4);

                // Use the == operator for testing
            }
    };
}
