#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include "SourceProcessor/SimpleToken.h"
#include "SourceProcessor/Parser.h"

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
                expectedResult.push_back(SimpleToken("procedure"));
                expectedResult.push_back(SimpleToken("computeAverage"));
                expectedResult.push_back(SimpleToken("{"));
                expectedResult.push_back(SimpleToken("read"));
                expectedResult.push_back(SimpleToken("num1"));
                expectedResult.push_back(SimpleToken(";"));
                expectedResult.push_back(SimpleToken("read"));
                expectedResult.push_back(SimpleToken("num2"));
                expectedResult.push_back(SimpleToken(";"));
                expectedResult.push_back(SimpleToken("sum"));
                expectedResult.push_back(SimpleToken("="));
                expectedResult.push_back(SimpleToken("num1"));
                expectedResult.push_back(SimpleToken("+"));
                expectedResult.push_back(SimpleToken("num2"));
                expectedResult.push_back(SimpleToken(";"));
                expectedResult.push_back(SimpleToken("print"));
                expectedResult.push_back(SimpleToken("sum"));
                expectedResult.push_back(SimpleToken(";"));
                expectedResult.push_back(SimpleToken("}"));

                SimpleParser simpleParser(str);
                Parser* parser = &simpleParser;
                vector<SimpleToken> result = parser->getTokens();
                Assert::AreEqual(expectedResult.size(), result.size());
                for (int i = 0; i < expectedResult.size(); i++) {
                    Assert::AreEqual(expectedResult[i].getValue(), result[i].getValue());
                }
            }

            TEST_METHOD(TestTokenizePoorFormat) {
                const std::string str = "procedure computeAverage{\n"
                                        "    read num1;\n"
                                        "    read num2;\n"
                                        "\n"
                                        "    sum= num1+num2;\n"
                                        "\n"
                                        "    print sum;}";
                vector<SimpleToken> expectedResult;
                expectedResult.push_back(SimpleToken("procedure"));
                expectedResult.push_back(SimpleToken("computeAverage"));
                expectedResult.push_back(SimpleToken("{"));
                expectedResult.push_back(SimpleToken("read"));
                expectedResult.push_back(SimpleToken("num1"));
                expectedResult.push_back(SimpleToken(";"));
                expectedResult.push_back(SimpleToken("read"));
                expectedResult.push_back(SimpleToken("num2"));
                expectedResult.push_back(SimpleToken(";"));
                expectedResult.push_back(SimpleToken("sum"));
                expectedResult.push_back(SimpleToken("="));
                expectedResult.push_back(SimpleToken("num1"));
                expectedResult.push_back(SimpleToken("+"));
                expectedResult.push_back(SimpleToken("num2"));
                expectedResult.push_back(SimpleToken(";"));
                expectedResult.push_back(SimpleToken("print"));
                expectedResult.push_back(SimpleToken("sum"));
                expectedResult.push_back(SimpleToken(";"));
                expectedResult.push_back(SimpleToken("}"));

                SimpleParser simpleParser(str);
                Parser* parser = &simpleParser;
                vector<SimpleToken> result = parser->getTokens();
                Assert::AreEqual(expectedResult.size(), result.size());
                for (int i = 0; i < expectedResult.size(); i++) {
                    Assert::AreEqual(expectedResult[i].getValue(), result[i].getValue());
                }
            }

            TEST_METHOD(TestTokenizeEmptyString) {
                const std::string str = "";
                vector<SimpleToken> expectedResult;
                SimpleParser simpleParser(str);
                Parser* parser = &simpleParser;
                vector<SimpleToken> result = parser->getTokens();
                Assert::AreEqual(expectedResult.size(), result.size());
                for (int i = 0; i < expectedResult.size(); i++) {
                    Assert::AreEqual(expectedResult[i].getValue(), result[i].getValue());
                }
            }

            /*
            TEST_METHOD(TestTokenizeRelationalOperators) {
                const std::string str = "procedure computeCentroid { \n"
                                        "count = 0; \n"
                                        "while ((x != 0) && (y != 0)) {\n"
                                        "count = count + 1;\n"
                                        "}\n"
                                        "if (count == 0) then {\n"
                                        "flag = 1;\n"
                                        "} else {\n"
                                        "cenX = cenX / count;\n"
                                        "}\n"
                                        "}";
                vector<SimpleToken> expectedResult;
                expectedResult.push_back(SimpleToken("procedure"));
                expectedResult.push_back(SimpleToken("computeCentroid"));
                expectedResult.push_back(SimpleToken("{"));
                expectedResult.push_back(SimpleToken("count"));
                expectedResult.push_back(SimpleToken("="));
                expectedResult.push_back(SimpleToken("0"));
                expectedResult.push_back(SimpleToken(";"));
                expectedResult.push_back(SimpleToken("while"));
                expectedResult.push_back(SimpleToken("("));
                expectedResult.push_back(SimpleToken("("));
                expectedResult.push_back(SimpleToken("x"));
                expectedResult.push_back(SimpleToken("!="));
                expectedResult.push_back(SimpleToken("0"));
                expectedResult.push_back(SimpleToken(")"));
                expectedResult.push_back(SimpleToken("&&"));
                expectedResult.push_back(SimpleToken("("));
                expectedResult.push_back(SimpleToken("y"));
                expectedResult.push_back(SimpleToken("!="));
                expectedResult.push_back(SimpleToken("0"));
                expectedResult.push_back(SimpleToken(")"));
                expectedResult.push_back(SimpleToken(")"));
                expectedResult.push_back(SimpleToken("{"));
                expectedResult.push_back(SimpleToken("count"));
                expectedResult.push_back(SimpleToken("="));
                expectedResult.push_back(SimpleToken("count"));
                expectedResult.push_back(SimpleToken("+"));
                expectedResult.push_back(SimpleToken("1"));
                expectedResult.push_back(SimpleToken(";"));
                expectedResult.push_back(SimpleToken("}"));
                expectedResult.push_back(SimpleToken("if"));
                expectedResult.push_back(SimpleToken("("));
                expectedResult.push_back(SimpleToken("count"));
                expectedResult.push_back(SimpleToken("=="));
                expectedResult.push_back(SimpleToken("0"));
                expectedResult.push_back(SimpleToken(")"));
                expectedResult.push_back(SimpleToken("then"));
                expectedResult.push_back(SimpleToken("{"));
                expectedResult.push_back(SimpleToken("flag"));
                expectedResult.push_back(SimpleToken("="));
                expectedResult.push_back(SimpleToken("1"));
                expectedResult.push_back(SimpleToken(";"));
                expectedResult.push_back(SimpleToken("}"));
                expectedResult.push_back(SimpleToken("else"));
                expectedResult.push_back(SimpleToken("{"));
                expectedResult.push_back(SimpleToken("cenX"));
                expectedResult.push_back(SimpleToken("="));
                expectedResult.push_back(SimpleToken("cenX"));
                expectedResult.push_back(SimpleToken("/"));
                expectedResult.push_back(SimpleToken("count"));
                expectedResult.push_back(SimpleToken(";"));
                expectedResult.push_back(SimpleToken("}"));
                expectedResult.push_back(SimpleToken("}"));
                SimpleParser simpleParser(str);
                Parser* parser = &simpleParser;
                vector<SimpleToken> result = parser->getTokens();
                Assert::AreEqual(expectedResult.size(), result.size());
                for (int i = 0; i < expectedResult.size(); i++) {
                    Assert::AreEqual(expectedResult[i].getValue(), result[i].getValue());
                }
            }
            */

            // Procedure names, variable names and terminals can all be the same.
            /*
            TEST_METHOD(TestTokenizeVariableNameProcedureNameClashWithNonTerminals) {
                const std::string str = "procedure call { \n"
                                        "read print;\n"
                                        "read = 0; \n"
                                        "}";
                vector<SimpleToken> expectedResult;
                expectedResult.push_back(SimpleToken("procedure"));
                expectedResult.push_back(SimpleToken("call"));
                expectedResult.push_back(SimpleToken("{"));
                expectedResult.push_back(SimpleToken("read"));
                expectedResult.push_back(SimpleToken("print"));
                expectedResult.push_back(SimpleToken(";"));
                expectedResult.push_back(SimpleToken("read"));
                expectedResult.push_back(SimpleToken("="));
                expectedResult.push_back(SimpleToken("0"));
                expectedResult.push_back(SimpleToken(";"));
                expectedResult.push_back(SimpleToken("}"));
                SimpleParser simpleParser(str);
                Parser* parser = &simpleParser;
                vector<SimpleToken> result = parser->getTokens();
                Assert::AreEqual(expectedResult.size(), result.size());
                for (int i = 0; i < expectedResult.size(); i++) {
                    Assert::AreEqual(expectedResult[i].getValue(), result[i].getValue());
                }
                // Now check whether the token types are correct
                Assert::IsTrue(SimpleToken::TokenType::PROCEDURE == result[0].getType(), L"Wrong token type");
                Assert::IsTrue(SimpleToken::TokenType::WORD == result[1].getType(), L"Wrong token type");
                Assert::IsTrue(SimpleToken::TokenType::OPEN_BRACES == result[2].getType());
                Assert::IsTrue(SimpleToken::TokenType::READ == result[3].getType());
                Assert::IsTrue(SimpleToken::TokenType::WORD == result[4].getType());
                Assert::IsTrue(SimpleToken::TokenType::SEMICOLON == result[5].getType());
                Assert::IsTrue(SimpleToken::TokenType::WORD == result[6].getType());
                Assert::IsTrue(SimpleToken::TokenType::ASSIGN == result[7].getType());
                Assert::IsTrue(SimpleToken::TokenType::NUMBER == result[8].getType());
                Assert::IsTrue(SimpleToken::TokenType::SEMICOLON == result[9].getType());
                Assert::IsTrue(SimpleToken::TokenType::CLOSE_BRACES == result[10].getType());
            }
             */
    };
}
