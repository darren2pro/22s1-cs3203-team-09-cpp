#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include "SourceProcessor/parser/SimpleToken.h"
#include "SourceProcessor/parser/SimpleParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestTokenizer) {
            public:
            TEST_METHOD(TestTokenizeNiceFormat) {
                const std::string str = "procedure computeAverage {\n"
                                        "    read num1;\n"
                                        "    read num2;\n"
                                        "\n"
                                        "    sum = num1 + num2;\n"
                                        "\n"
                                        "    print sum;\n"
                                        "}";
                vector<SimpleToken> expectedResult;
                expectedResult.push_back(SimpleToken("procedure", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("computeAverage", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("{", SimpleToken::TokenType::OPEN_BRACES));
                expectedResult.push_back(SimpleToken("read", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("num1", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken(";", SimpleToken::TokenType::SEMICOLON));
                expectedResult.push_back(SimpleToken("read", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("num2", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken(";", SimpleToken::TokenType::SEMICOLON));
                expectedResult.push_back(SimpleToken("sum", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("=", SimpleToken::TokenType::ASSIGN));
                expectedResult.push_back(SimpleToken("num1", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("+", SimpleToken::TokenType::PLUS));
                expectedResult.push_back(SimpleToken("num2", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken(";", SimpleToken::TokenType::SEMICOLON));
                expectedResult.push_back(SimpleToken("print", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("sum", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken(";", SimpleToken::TokenType::SEMICOLON));
                expectedResult.push_back(SimpleToken("}", SimpleToken::TokenType::CLOSE_BRACES));
                expectedResult.push_back(SimpleToken("EOF", SimpleToken::TokenType::END_OF_FILE));

                std::istringstream iss(str);
                SimpleParser simpleParser(&iss);
                SimpleParser* parser = &simpleParser;
                SimpleParser::SOURCE_CODE_TOKENS result = parser->getTokens();
                Assert::AreEqual(expectedResult.size(), result.size());
                for (int i = 0; i < expectedResult.size(); i++) {
                    Assert::IsTrue(expectedResult[i] == *(result[i]));
                }
            }

            TEST_METHOD(TestTokenizePoorFormat) {
                const std::string str = "procedure computeAverage{\n"
                                        "    read num1;\n"
                                        "    read num2;\n"
                                        "\n"
                                        "\n"
                                        "    sum= num1+num2;\n"
                                        "\n"
                                        "    print sum;}";
                vector<SimpleToken> expectedResult;
                expectedResult.push_back(SimpleToken("procedure", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("computeAverage", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("{", SimpleToken::TokenType::OPEN_BRACES));
                expectedResult.push_back(SimpleToken("read", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("num1", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken(";", SimpleToken::TokenType::SEMICOLON));
                expectedResult.push_back(SimpleToken("read", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("num2", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken(";", SimpleToken::TokenType::SEMICOLON));
                expectedResult.push_back(SimpleToken("sum", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("=", SimpleToken::TokenType::ASSIGN));
                expectedResult.push_back(SimpleToken("num1", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("+", SimpleToken::TokenType::PLUS));
                expectedResult.push_back(SimpleToken("num2", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken(";", SimpleToken::TokenType::SEMICOLON));
                expectedResult.push_back(SimpleToken("print", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("sum", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken(";", SimpleToken::TokenType::SEMICOLON));
                expectedResult.push_back(SimpleToken("}", SimpleToken::TokenType::CLOSE_BRACES));
                expectedResult.push_back(SimpleToken("EOF", SimpleToken::TokenType::END_OF_FILE));

                std::istringstream iss(str);
                SimpleParser simpleParser(&iss);
                SimpleParser* parser = &simpleParser;
                SimpleParser::SOURCE_CODE_TOKENS result = parser->getTokens();
                Assert::AreEqual(expectedResult.size(), result.size());
                for (int i = 0; i < expectedResult.size(); i++) {
                    Assert::IsTrue(expectedResult[i] == *(result[i]));
                }
            }

            TEST_METHOD(TestTokenizeEmptyString) {
                const std::string str = "";
                vector<SimpleToken> expectedResult;
                expectedResult.push_back(SimpleToken("EOF", SimpleToken::TokenType::END_OF_FILE));

                std::istringstream iss(str);
                SimpleParser simpleParser(&iss);
                SimpleParser* parser = &simpleParser;
                SimpleParser::SOURCE_CODE_TOKENS result = parser->getTokens();
                Assert::AreEqual(expectedResult.size(), result.size());
                for (int i = 0; i < expectedResult.size(); i++) {
                    Assert::IsTrue(expectedResult[i] == *(result[i]));
                }
            }

            TEST_METHOD(TestTokenizeRelationalOperators) {
                const std::string str = "procedure myProc { \n"
                                        "count = 0; \n"
                                        "while ((x != 0) && (y <= 0)) {\n"
                                        "count = count + 1;\n"
                                        "}\n"
                                        "if (count == 0) then {\n"
                                        "flag = 1;\n"
                                        "} else {\n"
                                        "cenX = cenX / count;\n"
                                        "}\n"
                                        "}";
                vector<SimpleToken> expectedResult;
                expectedResult.push_back(SimpleToken("procedure", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("myProc", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("{", SimpleToken::TokenType::OPEN_BRACES));
                expectedResult.push_back(SimpleToken("count", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("=", SimpleToken::TokenType::ASSIGN));
                expectedResult.push_back(SimpleToken("0", SimpleToken::TokenType::NUMBER));
                expectedResult.push_back(SimpleToken(";", SimpleToken::TokenType::SEMICOLON));
                expectedResult.push_back(SimpleToken("while", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("(", SimpleToken::TokenType::OPEN_PARENTHESES));
                expectedResult.push_back(SimpleToken("(", SimpleToken::TokenType::OPEN_PARENTHESES));
                expectedResult.push_back(SimpleToken("x", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("!=", SimpleToken::TokenType::NOT_EQUAL_TO));
                expectedResult.push_back(SimpleToken("0", SimpleToken::TokenType::NUMBER));
                expectedResult.push_back(SimpleToken(")", SimpleToken::TokenType::CLOSE_PARENTHESES));
                expectedResult.push_back(SimpleToken("&&", SimpleToken::TokenType::AND));
                expectedResult.push_back(SimpleToken("(", SimpleToken::TokenType::OPEN_PARENTHESES));
                expectedResult.push_back(SimpleToken("y", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("<=", SimpleToken::TokenType::LESS_THAN_OR_EQUAL_TO));
                expectedResult.push_back(SimpleToken("0", SimpleToken::TokenType::NUMBER));
                expectedResult.push_back(SimpleToken(")", SimpleToken::TokenType::CLOSE_PARENTHESES));
                expectedResult.push_back(SimpleToken(")", SimpleToken::TokenType::CLOSE_PARENTHESES));
                expectedResult.push_back(SimpleToken("{", SimpleToken::TokenType::OPEN_BRACES));
                expectedResult.push_back(SimpleToken("count", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("=", SimpleToken::TokenType::ASSIGN));
                expectedResult.push_back(SimpleToken("count", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("+", SimpleToken::TokenType::PLUS));
                expectedResult.push_back(SimpleToken("1", SimpleToken::TokenType::NUMBER));
                expectedResult.push_back(SimpleToken(";", SimpleToken::TokenType::SEMICOLON));
                expectedResult.push_back(SimpleToken("}", SimpleToken::TokenType::CLOSE_BRACES));
                expectedResult.push_back(SimpleToken("if", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("(", SimpleToken::TokenType::OPEN_PARENTHESES));
                expectedResult.push_back(SimpleToken("count", SimpleToken::TokenType::WORD));
				expectedResult.push_back(SimpleToken("==", SimpleToken::TokenType::EQUALS));
                expectedResult.push_back(SimpleToken("0", SimpleToken::TokenType::NUMBER));
                expectedResult.push_back(SimpleToken(")", SimpleToken::TokenType::CLOSE_PARENTHESES));
                expectedResult.push_back(SimpleToken("then", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("{", SimpleToken::TokenType::OPEN_BRACES));
                expectedResult.push_back(SimpleToken("flag", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("=", SimpleToken::TokenType::ASSIGN));
                expectedResult.push_back(SimpleToken("1", SimpleToken::TokenType::NUMBER));
                expectedResult.push_back(SimpleToken(";", SimpleToken::TokenType::SEMICOLON));
                expectedResult.push_back(SimpleToken("}", SimpleToken::TokenType::CLOSE_BRACES));
                expectedResult.push_back(SimpleToken("else", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("{", SimpleToken::TokenType::OPEN_BRACES));
                expectedResult.push_back(SimpleToken("cenX", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("=", SimpleToken::TokenType::ASSIGN));
                expectedResult.push_back(SimpleToken("cenX", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("/", SimpleToken::TokenType::DIVIDE));
                expectedResult.push_back(SimpleToken("count", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken(";", SimpleToken::TokenType::SEMICOLON));
                expectedResult.push_back(SimpleToken("}", SimpleToken::TokenType::CLOSE_BRACES));
                expectedResult.push_back(SimpleToken("}", SimpleToken::TokenType::CLOSE_BRACES));
                expectedResult.push_back(SimpleToken("EOF", SimpleToken::TokenType::END_OF_FILE));

                std::istringstream iss(str);
                SimpleParser simpleParser(&iss);
                SimpleParser* parser = &simpleParser;
                SimpleParser::SOURCE_CODE_TOKENS result = parser->getTokens();
                Assert::AreEqual(expectedResult.size(), result.size());
                for (int i = 0; i < expectedResult.size(); i++) {
                    Assert::IsTrue(expectedResult[i] == *(result[i]));
                }
            }

            // Procedure names, variable names and terminals can all be the same.
            TEST_METHOD(TestTokenizeVariableNameProcedureNameClashWithNonTerminals) {
                const std::string str = "procedure call { \n"
                                        "read print;\n"
                                        "read = 04006; \n"
                                        "}";
                vector<SimpleToken> expectedResult;
                expectedResult.push_back(SimpleToken("procedure", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("call", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("{", SimpleToken::TokenType::OPEN_BRACES));
                expectedResult.push_back(SimpleToken("read", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("print", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken(";", SimpleToken::TokenType::SEMICOLON));
                expectedResult.push_back(SimpleToken("read", SimpleToken::TokenType::WORD));
                expectedResult.push_back(SimpleToken("=", SimpleToken::TokenType::ASSIGN));
                expectedResult.push_back(SimpleToken("04006", SimpleToken::TokenType::NUMBER));
                expectedResult.push_back(SimpleToken(";", SimpleToken::TokenType::SEMICOLON));
                expectedResult.push_back(SimpleToken("}", SimpleToken::TokenType::CLOSE_BRACES));
                expectedResult.push_back(SimpleToken("EOF", SimpleToken::TokenType::END_OF_FILE));

                std::istringstream iss(str);
                SimpleParser simpleParser(&iss);
                SimpleParser* parser = &simpleParser;
                SimpleParser::SOURCE_CODE_TOKENS result = parser->getTokens();
                Assert::AreEqual(expectedResult.size(), result.size());
                for (int i = 0; i < expectedResult.size(); i++) {
                    Assert::IsTrue(expectedResult[i] == *(result[i]));
                }
            }
    };
}
