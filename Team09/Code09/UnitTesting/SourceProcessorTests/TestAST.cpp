#include "stdafx.h"
#include "CppUnitTest.h"
#include <SourceProcessor/Parser.h>
#include <SourceProcessor/astBuilder/SimpleAstBuilder.h>
#include <SourceProcessor/exceptions/SimpleInvalidSyntaxException.h>

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

            TEST_METHOD(TestVariousStatementTypes) {
                string program = "procedure main4 {\n"
                                 "  read x;\n"
                                 "      call main2;\n"
                                 "         \t  print y;\n"
                                 "}\n\n";
                std::istringstream iss(program);
                SimpleParser simpleParser(&iss);
                Parser* parser = &simpleParser;
                Parser::SOURCE_CODE_TOKENS result = parser->getTokens();
                SimpleAstBuilder astBuilder(result);
                AST programNode = astBuilder.build();

                Logger::WriteMessage("[TestVariousStatementTypes] Printing AST\n");
                Logger::WriteMessage(programNode->toString().c_str());

                // Expected result
                // read x
                VariableNodePtr x = make_shared<VariableNode>("x");
                ReadNodePtr read = make_shared<ReadNode>(x);
                // call main2
                ProcedureNodePtr main2 = make_shared<ProcedureNode>("main2", StmtLst{});
                CallNodePtr call = make_shared<CallNode>(main2);
                // print y
                VariableNodePtr y = make_shared<VariableNode>("y");
                PrintNodePtr print = make_shared<PrintNode>(y);
                // Procedure node and program node
                ProcedureNodePtr main4 = make_shared<ProcedureNode>("main4", StmtLst{read, call, print});
                AST expectedProgramNode = make_shared<ProgramNode>(vector<ProcedureNodePtr>{main4});

                Assert::IsTrue(*programNode == *expectedProgramNode, L"ASTs are not equal");
            }

            TEST_METHOD(TestClashingNamesAndVariables) {
                string program = "procedure main5 {\n"
                                 "    read read;\n"
                                 "    print print;\n"
                                 "    call call;\n"
                                 "    \n"
                                 "    print read;\n"
                                 "    call print;\n"
                                 "    read call;\n"
                                 "}\n";
                std::istringstream iss(program);
                SimpleParser simpleParser(&iss);
                Parser* parser = &simpleParser;
                Parser::SOURCE_CODE_TOKENS result = parser->getTokens();
                SimpleAstBuilder astBuilder(result);
                AST programNode = astBuilder.build();

                Logger::WriteMessage("[TestClashingNamesAndVariables] Printing AST\n");
                Logger::WriteMessage(programNode->toString().c_str());

                // Expected result
                // read read
                VariableNodePtr read1 = make_shared<VariableNode>("read");
                ReadNodePtr read2 = make_shared<ReadNode>(read1);
                // print print
                VariableNodePtr print1 = make_shared<VariableNode>("print");
                PrintNodePtr print2 = make_shared<PrintNode>(print1);
                // call call
                ProcedureNodePtr call1 = make_shared<ProcedureNode>("call", StmtLst{});
                CallNodePtr call2 = make_shared<CallNode>(call1);
                // print read
                VariableNodePtr read3 = make_shared<VariableNode>("read");
                PrintNodePtr print3 = make_shared<PrintNode>(read3);
                // call print
                ProcedureNodePtr print4 = make_shared<ProcedureNode>("print", StmtLst{});
                CallNodePtr call3 = make_shared<CallNode>(print4);
                // read call
                VariableNodePtr call4 = make_shared<VariableNode>("call");
                ReadNodePtr read4 = make_shared<ReadNode>(call4);
                // Procedure node and program node
                ProcedureNodePtr main5 = make_shared<ProcedureNode>("main5", StmtLst{read2, print2, call2, print3, call3, read4});
                AST expectedProgramNode = make_shared<ProgramNode>(vector<ProcedureNodePtr>{main5});

                Assert::IsTrue(*programNode == *expectedProgramNode, L"ASTs are not equal");
            }

            TEST_METHOD(TestSingleNestingWhileAndIf) {
                string program = "procedure main2 {\n"
                                 "    while (i == 0) {\n"
                                 "        read x;\n"
                                 "        print y;\n"
                                 "    }\n"
                                 "\n"
                                 "    if (j >= 11) then {\n"
                                 "        print y;\n"
                                 "        call anotherProc;\n"
                                 "        y = 456;\n"
                                 "        y = y + 1;\n"
                                 "    } else {\n"
                                 "        kk = 123 + yup;\n"
                                 "        print print;\n"
                                 "    }\n"
                                 "}\n";
                std::istringstream iss(program);
                SimpleParser simpleParser(&iss);
                Parser* parser = &simpleParser;
                Parser::SOURCE_CODE_TOKENS result = parser->getTokens();
                SimpleAstBuilder astBuilder(result);
                AST programNode = astBuilder.build();

                Logger::WriteMessage("[TestSingleNestingWhileAndIf] Printing AST\n");
                Logger::WriteMessage(programNode->toString().c_str());

                // Expected result
                // while (i == 0)
                VariableNodePtr i = make_shared<VariableNode>("i");
                ConstantNodePtr zero = make_shared<ConstantNode>("0");
                RelExprNodePtr relExpr = make_shared<RelExprNode>("==", i, zero);
                CondExprNodePtr condExpr = make_shared<CondExprNode>(relExpr);
                // read x
                VariableNodePtr x = make_shared<VariableNode>("x");
                ReadNodePtr read = make_shared<ReadNode>(x);
                // print y
                VariableNodePtr y = make_shared<VariableNode>("y");
                PrintNodePtr print = make_shared<PrintNode>(y);
                // while node
                WhileNodePtr whileNode = make_shared<WhileNode>(condExpr, StmtLst{read, print});
                // if (j >= 11)
                VariableNodePtr j = make_shared<VariableNode>("j");
                ConstantNodePtr eleven = make_shared<ConstantNode>("11");
                RelExprNodePtr relExpr2 = make_shared<RelExprNode>(">=", j, eleven);
                CondExprNodePtr condExpr2 = make_shared<CondExprNode>(relExpr2);
                // print y
                VariableNodePtr y1 = make_shared<VariableNode>("y");
                PrintNodePtr print2 = make_shared<PrintNode>(y1);
                // call anotherProc
                ProcedureNodePtr anotherProc = make_shared<ProcedureNode>("anotherProc", StmtLst{});
                CallNodePtr call = make_shared<CallNode>(anotherProc);
                // y = 456
                VariableNodePtr y2 = make_shared<VariableNode>("y");
                ConstantNodePtr fourHundredFiftySix = make_shared<ConstantNode>("456");
                AssignmentNodePtr assign = make_shared<AssignmentNode>(y2, fourHundredFiftySix);
                // y = y + 1
                VariableNodePtr y3 = make_shared<VariableNode>("y");
                VariableNodePtr y4 = make_shared<VariableNode>("y");
                ConstantNodePtr one = make_shared<ConstantNode>("1");
                BinOpNodePtr binOp = make_shared<BinOpNode>("+", y3, one);
                AssignmentNodePtr assign2 = make_shared<AssignmentNode>(y4, binOp);
                // else
                // kk = 123 + yup
                VariableNodePtr kk = make_shared<VariableNode>("kk");
                ConstantNodePtr oneHundredTwentyThree = make_shared<ConstantNode>("123");
                VariableNodePtr yup = make_shared<VariableNode>("yup");
                BinOpNodePtr binOp2 = make_shared<BinOpNode>("+", oneHundredTwentyThree, yup);
                AssignmentNodePtr assign3 = make_shared<AssignmentNode>(kk, binOp2);
                // print print
                VariableNodePtr print3 = make_shared<VariableNode>("print");
                PrintNodePtr print4 = make_shared<PrintNode>(print3);
                // if node
                IfNodePtr ifNode = make_shared<IfNode>(condExpr2, StmtLst{print2, call, assign, assign2}, StmtLst{assign3, print4});
                // program node and procedure node
                ProcedureNodePtr main2 = make_shared<ProcedureNode>("main2", StmtLst{whileNode, ifNode});
                AST expectedProgramNode = make_shared<ProgramNode>(vector<ProcedureNodePtr>{main2});

                Assert::IsTrue(*programNode == *expectedProgramNode, L"ASTs are not equal");
            }

            TEST_METHOD(TestNestedWhileAndIf) {
                string program = "procedure main3 {\n"
                                 "    while (j <= 22) {\n"
                                 "        while ((i < 0) && (j > 0)) {\n"
                                 "            digit = number % 10;\n"
                                 "            number = number / 10;\n"
                                 "        }\n"
                                 "    }\n"
                                 "\n"
                                 "    while ((k != 5) || (l == 0)) {\n"
                                 "        if (j >= 11) then {\n"
                                 "            print y;\n"
                                 "        } else {\n"
                                 "            x = 1;\n"
                                 "        }\n"
                                 "    }\n"
                                 "}\n";
                std::istringstream iss(program);
                SimpleParser simpleParser(&iss);
                Parser* parser = &simpleParser;
                Parser::SOURCE_CODE_TOKENS result = parser->getTokens();
                SimpleAstBuilder astBuilder(result);
                AST programNode = astBuilder.build();

                Logger::WriteMessage("[TestNestedWhileAndIf] Printing AST\n");
                Logger::WriteMessage(programNode->toString().c_str());

                // Expected result
                // while (j <= 22)
                VariableNodePtr j = make_shared<VariableNode>("j");
                ConstantNodePtr twentyTwo = make_shared<ConstantNode>("22");
                RelExprNodePtr relExpr = make_shared<RelExprNode>("<=", j, twentyTwo);
                CondExprNodePtr condExpr = make_shared<CondExprNode>(relExpr);
                // while ((i < 0) && (j > 0))
                VariableNodePtr i = make_shared<VariableNode>("i");
                ConstantNodePtr zero = make_shared<ConstantNode>("0");
                RelExprNodePtr relExpr2 = make_shared<RelExprNode>("<", i, zero);
                CondExprNodePtr condExpr2 = make_shared<CondExprNode>(relExpr2);
                VariableNodePtr j2 = make_shared<VariableNode>("j");
                ConstantNodePtr zero2 = make_shared<ConstantNode>("0");
                RelExprNodePtr relExpr3 = make_shared<RelExprNode>(">", j2, zero2);
                CondExprNodePtr condExpr3 = make_shared<CondExprNode>(relExpr3);
                CondExprNodePtr condExprNode = make_shared<CondExprNode>("&&", condExpr2, condExpr3);
                // digit = number % 10
                VariableNodePtr digit = make_shared<VariableNode>("digit");
                VariableNodePtr number = make_shared<VariableNode>("number");
                ConstantNodePtr ten = make_shared<ConstantNode>("10");
                BinOpNodePtr binOp = make_shared<BinOpNode>("%", number, ten);
                AssignmentNodePtr assign = make_shared<AssignmentNode>(digit, binOp);
                // number = number / 10
                VariableNodePtr number2 = make_shared<VariableNode>("number");
                VariableNodePtr number3 = make_shared<VariableNode>("number");
                ConstantNodePtr ten2 = make_shared<ConstantNode>("10");
                BinOpNodePtr binOp2 = make_shared<BinOpNode>("/", number3, ten2);
                AssignmentNodePtr assign2 = make_shared<AssignmentNode>(number2, binOp2);
                // while node
                WhileNodePtr whileNode = make_shared<WhileNode>(condExprNode, StmtLst{assign, assign2});
                // outer while node
                WhileNodePtr whileNode2 = make_shared<WhileNode>(condExpr, StmtLst{whileNode});
                // while ((k != 5) || (l == 0))
                VariableNodePtr k = make_shared<VariableNode>("k");
                ConstantNodePtr five = make_shared<ConstantNode>("5");
                RelExprNodePtr relExpr4 = make_shared<RelExprNode>("!=", k, five);
                CondExprNodePtr condExpr4 = make_shared<CondExprNode>(relExpr4);
                VariableNodePtr l = make_shared<VariableNode>("l");
                ConstantNodePtr zero3 = make_shared<ConstantNode>("0");
                RelExprNodePtr relExpr5 = make_shared<RelExprNode>("==", l, zero3);
                CondExprNodePtr condExpr5 = make_shared<CondExprNode>(relExpr5);
                CondExprNodePtr condExprNode2 = make_shared<CondExprNode>("||", condExpr4, condExpr5);
                // if (j >= 11)
                VariableNodePtr j3 = make_shared<VariableNode>("j");
                ConstantNodePtr eleven = make_shared<ConstantNode>("11");
                RelExprNodePtr relExpr6 = make_shared<RelExprNode>(">=", j3, eleven);
                CondExprNodePtr condExpr6 = make_shared<CondExprNode>(relExpr6);
                // print y
                VariableNodePtr y = make_shared<VariableNode>("y");
                PrintNodePtr print = make_shared<PrintNode>(y);
                // else x = 1
                VariableNodePtr x = make_shared<VariableNode>("x");
                ConstantNodePtr one = make_shared<ConstantNode>("1");
                AssignmentNodePtr assign3 = make_shared<AssignmentNode>(x, one);
                // if node
                IfNodePtr ifNode = make_shared<IfNode>(condExpr6, StmtLst{print}, StmtLst{assign3});
                // outer while node
                WhileNodePtr whileNode3 = make_shared<WhileNode>(condExprNode2, StmtLst{ifNode});
                // program node and procedure node
                ProcedureNodePtr main3 = make_shared<ProcedureNode>("main3", StmtLst{whileNode2, whileNode3});
                AST expectedProgramNode = make_shared<ProgramNode>(vector<ProcedureNodePtr>{main3});

                Assert::IsTrue(*programNode == *expectedProgramNode, L"ASTs are not equal");
            }

            TEST_METHOD(TestComplexArithmeticExpression) {
                string program = "procedure complexMain {\n"
                                 "    num1 = 1 + 2 * 3 - 4 / 5 + (6 - 7) - (8 + 9);\n"
                                 "    x = 1 + 2 * 3 - 4 / 5 % 6;\n"
                                 "    y = 1 + 2 * 3 - 4 / 5 % 6 + 7 * 8 - 9 % 10 / 11;\n"
                                 "}\t";
                std::istringstream iss(program);
                SimpleParser simpleParser(&iss);
                Parser* parser = &simpleParser;
                Parser::SOURCE_CODE_TOKENS result = parser->getTokens();
                SimpleAstBuilder astBuilder(result);
                AST programNode = astBuilder.build();

                Logger::WriteMessage("[TestComplexArithmeticExpression] Printing AST\n");
                Logger::WriteMessage(programNode->toString().c_str());

                // Expected result
                // num1 = 1 + 2 * 3 - 4 / 5 + (6 - 7) - (8 + 9)
                VariableNodePtr num1 = make_shared<VariableNode>("num1");
                ConstantNodePtr one = make_shared<ConstantNode>("1");
                ConstantNodePtr two = make_shared<ConstantNode>("2");
                ConstantNodePtr three = make_shared<ConstantNode>("3");
                BinOpNodePtr binOp = make_shared<BinOpNode>("*", two, three);
                BinOpNodePtr binOp2 = make_shared<BinOpNode>("+", one, binOp);
                ConstantNodePtr four = make_shared<ConstantNode>("4");
                ConstantNodePtr five = make_shared<ConstantNode>("5");
                BinOpNodePtr binOp3 = make_shared<BinOpNode>("/", four, five);
                BinOpNodePtr binOp4 = make_shared<BinOpNode>("-", binOp2, binOp3);
                ConstantNodePtr six = make_shared<ConstantNode>("6");
                ConstantNodePtr seven = make_shared<ConstantNode>("7");
                BinOpNodePtr binOp5 = make_shared<BinOpNode>("-", six, seven);
                BinOpNodePtr binOp6 = make_shared<BinOpNode>("+", binOp4, binOp5);
                ConstantNodePtr eight = make_shared<ConstantNode>("8");
                ConstantNodePtr nine = make_shared<ConstantNode>("9");
                BinOpNodePtr binOp7 = make_shared<BinOpNode>("+", eight, nine);
                BinOpNodePtr binOp8 = make_shared<BinOpNode>("-", binOp6, binOp7);
                AssignmentNodePtr assign = make_shared<AssignmentNode>(num1, binOp8);
                // x = 1 + 2 * 3 - 4 / 5 % 6
                VariableNodePtr x = make_shared<VariableNode>("x");
                ConstantNodePtr one2 = make_shared<ConstantNode>("1");
                ConstantNodePtr two2 = make_shared<ConstantNode>("2");
                ConstantNodePtr three2 = make_shared<ConstantNode>("3");
                BinOpNodePtr binOp9 = make_shared<BinOpNode>("*", two2, three2);
                BinOpNodePtr binOp10 = make_shared<BinOpNode>("+", one2, binOp9);
                ConstantNodePtr four2 = make_shared<ConstantNode>("4");
                ConstantNodePtr five2 = make_shared<ConstantNode>("5");
                BinOpNodePtr binOp11 = make_shared<BinOpNode>("/", four2, five2);
                ConstantNodePtr six2 = make_shared<ConstantNode>("6");
                BinOpNodePtr binOp12 = make_shared<BinOpNode>("%", binOp11, six2);
                BinOpNodePtr binOp13 = make_shared<BinOpNode>("-", binOp10, binOp12);
                AssignmentNodePtr assign2 = make_shared<AssignmentNode>(x, binOp13);
                // y = 1 + 2 * 3 - 4 / 5 % 6 + 7 * 8 - 9 % 10 / 11
                VariableNodePtr y = make_shared<VariableNode>("y");
                ConstantNodePtr one3 = make_shared<ConstantNode>("1");
                ConstantNodePtr two3 = make_shared<ConstantNode>("2");
                ConstantNodePtr three3 = make_shared<ConstantNode>("3");
                BinOpNodePtr binOp14 = make_shared<BinOpNode>("*", two3, three3);
                BinOpNodePtr binOp15 = make_shared<BinOpNode>("+", one3, binOp14);
                ConstantNodePtr four3 = make_shared<ConstantNode>("4");
                ConstantNodePtr five3 = make_shared<ConstantNode>("5");
                BinOpNodePtr binOp16 = make_shared<BinOpNode>("/", four3, five3);
                ConstantNodePtr six3 = make_shared<ConstantNode>("6");
                BinOpNodePtr binOp17 = make_shared<BinOpNode>("%", binOp16, six3);
                BinOpNodePtr binOp18 = make_shared<BinOpNode>("-", binOp15, binOp17);
                ConstantNodePtr seven3 = make_shared<ConstantNode>("7");
                ConstantNodePtr eight3 = make_shared<ConstantNode>("8");
                BinOpNodePtr binOp19 = make_shared<BinOpNode>("*", seven3, eight3);
                BinOpNodePtr binOp20 = make_shared<BinOpNode>("+", binOp18, binOp19);
                ConstantNodePtr nine3 = make_shared<ConstantNode>("9");
                ConstantNodePtr ten = make_shared<ConstantNode>("10");
                BinOpNodePtr binOp21 = make_shared<BinOpNode>("%", nine3, ten);
                ConstantNodePtr eleven = make_shared<ConstantNode>("11");
                BinOpNodePtr binOp22 = make_shared<BinOpNode>("/", binOp21, eleven);
                BinOpNodePtr binOp23 = make_shared<BinOpNode>("-", binOp20, binOp22);
                AssignmentNodePtr assign3 = make_shared<AssignmentNode>(y, binOp23);
                // program node and procedure node
                ProcedureNodePtr procComplexMain = make_shared<ProcedureNode>("complexMain", StmtLst{assign, assign2, assign3});
                AST programNode2 = make_shared<ProgramNode>(vector<ProcedureNodePtr>{procComplexMain});

                Assert::IsTrue(*programNode == *programNode2, L"ASTs are not equal");
            }

            TEST_METHOD(TestSimpleArithmetic) {
                string program = "procedure arithmeticSimple {\n"
                                 "    testing = hello + world * 8;\n"
                                 "}";
                std::istringstream iss(program);
                SimpleParser simpleParser(&iss);
                Parser* parser = &simpleParser;
                Parser::SOURCE_CODE_TOKENS result = parser->getTokens();
                SimpleAstBuilder astBuilder(result);
                AST programNode = astBuilder.build();

                Logger::WriteMessage("[TestSimpleArithmetic] Printing AST\n");
                Logger::WriteMessage(programNode->toString().c_str());

                // hello + world * 8
                VariableNodePtr hello = make_shared<VariableNode>("hello");
                VariableNodePtr world = make_shared<VariableNode>("world");
                ConstantNodePtr eight = make_shared<ConstantNode>("8");
                BinOpNodePtr binOp = make_shared<BinOpNode>("*", world, eight);
                BinOpNodePtr binOp2 = make_shared<BinOpNode>("+", hello, binOp);
                // testing = hello + world * 8
                VariableNodePtr testing = make_shared<VariableNode>("testing");
                AssignmentNodePtr assign = make_shared<AssignmentNode>(testing, binOp2);
                // procedure node
                ProcedureNodePtr procSimpleArithmetic = make_shared<ProcedureNode>("arithmeticSimple", StmtLst{assign});
                AST programNode2 = make_shared<ProgramNode>(vector<ProcedureNodePtr>{procSimpleArithmetic});

                Assert::IsTrue(*programNode == *programNode2, L"ASTs are not equal");
            }

            TEST_METHOD(TestInvalidSourceProgram) {
                string program = "procedure invalid1 {\n"
                                 "    if (k == 1) {\n"
                                 "        print k;\n"
                                 "    } else {\n"
                                 "        print y;\n"
                                 "    }\n"
                                 "}";

                std::istringstream iss(program);
                SimpleParser simpleParser(&iss);
                Parser* parser = &simpleParser;
                Parser::SOURCE_CODE_TOKENS result = parser->getTokens();
                SimpleAstBuilder astBuilder(result);

                // expect an exception
                try {
                    AST programNode = astBuilder.build();
                    Assert::Fail(L"Expected an exception");
                } catch (SimpleInvalidSyntaxException& e) {
                    Logger::WriteMessage("(Correct) Exception seen: ");
                    Logger::WriteMessage(e.what());
                }
            }
    };
}
