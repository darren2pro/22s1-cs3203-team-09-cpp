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

                Logger::WriteMessage("[TestPureAssignmentStatements] Printing AST\n");
                Logger::WriteMessage(programNode->toString().c_str());

                Assert::AreEqual(programNode->procList.size(), (size_t) 1);
                Assert::AreEqual(programNode->procList[0]->procName, std::string("main1"));
                Assert::AreEqual(programNode->procList[0]->stmtList.size(), (size_t) 4);

                // Expected result
                // x = 1
                VariableNodePtr x = make_shared<VariableNode>("x");
                ConstantNodePtr one = make_shared<ConstantNode>("1");
                AssignmentNodePtr assign1 = make_shared<AssignmentNode>(x, one);
                // y = y + 34
                VariableNodePtr y = make_shared<VariableNode>("y");
                VariableNodePtr y2 = make_shared<VariableNode>("y");
                ConstantNodePtr thirtyFour = make_shared<ConstantNode>("34");
                BinOpNodePtr plus1 = make_shared<BinOpNode>("+", y2, thirtyFour);
                AssignmentNodePtr assign2 = make_shared<AssignmentNode>(y, plus1);
                // z = k + 123
                VariableNodePtr z = make_shared<VariableNode>("z");
                VariableNodePtr k = make_shared<VariableNode>("k");
                ConstantNodePtr oneHundredTwentyThree = make_shared<ConstantNode>("123");
                BinOpNodePtr plus2 = make_shared<BinOpNode>("+", k, oneHundredTwentyThree);
                AssignmentNodePtr assign3 = make_shared<AssignmentNode>(z, plus2);
                // a = x + y
                VariableNodePtr a = make_shared<VariableNode>("a");
                VariableNodePtr x2 = make_shared<VariableNode>("x");
                VariableNodePtr y3 = make_shared<VariableNode>("y");
                BinOpNodePtr plus3 = make_shared<BinOpNode>("+", x2, y3);
                AssignmentNodePtr assign4 = make_shared<AssignmentNode>(a, plus3);
                // Procedure node and program node
                ProcedureNodePtr main1 = make_shared<ProcedureNode>("main1", StmtLst{assign1, assign2, assign3, assign4});
                AST expectedProgramNode = make_shared<ProgramNode>(vector<ProcedureNodePtr>{main1});

                Assert::IsTrue(*programNode == *expectedProgramNode, L"ASTs are not equal");
            }
    };
}
