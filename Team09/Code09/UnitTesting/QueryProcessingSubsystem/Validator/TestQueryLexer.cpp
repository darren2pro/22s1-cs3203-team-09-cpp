#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include <vector>
#include "QueryProcessingSubsystem/Validator/QueryLexer.h"
#include "QueryProcessingSubsystem/Validator/QPSValidatorException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestQueryLexer) {
    
    public:

        TEST_METHOD(TestLexer) {
            const std::string query = "assign a; Select a pattern a(_, _\"x\"_) such that Modifies(a, \"x\")";

            std::vector<std::string> expectedResult = std::vector<std::string>(
                { "assign", "a", ";", "Select", "a", "pattern", "a", "(", "_", ",", "_",
                "\"x\"", "_", ")", "such", "that", "Modifies", "(", "a", ",", "\"x\"", ")" });

            QueryLexer lexer = QueryLexer(query);
            std::vector<std::string> result = lexer.lex();

            Assert::AreEqual(expectedResult.size(), result.size());
            for (int i = 0; i < expectedResult.size(); i++) {
                Assert::AreEqual(expectedResult[i], result[i]);
            }
        }

        TEST_METHOD(TestLexerQueryWithSpaces) {
            const std::string query = "assign a;\n"
                                      "Select      a pattern a(_, _       \"x        \"_)"
                                      "such that Modifies \t ( a , \"\tx       \")";

            std::vector<std::string> expectedResult = std::vector<std::string>(
                { "assign", "a", ";", "Select", "a", "pattern", "a", "(", "_", ",", "_",
                "\"x\"", "_", ")", "such", "that", "Modifies", "(", "a", ",", "\"x\"", ")" });

            QueryLexer lexer = QueryLexer(query);
            std::vector<std::string> result = lexer.lex();

            Assert::AreEqual(expectedResult.size(), result.size());
            for (int i = 0; i < expectedResult.size(); i++) {
                Assert::AreEqual(expectedResult[i], result[i]);
            }
        }

        TEST_METHOD(TestLexerEmptyQuery) {
            const std::string query = "";

            std::vector<std::string> expectedResult;

            QueryLexer lexer = QueryLexer(query);
            std::vector<std::string> result = lexer.lex();

            Assert::AreEqual(expectedResult.size(), result.size());
            for (int i = 0; i < expectedResult.size(); i++) {
                Assert::AreEqual(expectedResult[i], result[i]);
            }
        }

        TEST_METHOD(TestLexerInvalidToken) {
            const std::string query = "assign a$; Select a pattern a(_, _\"x\"_) such that Modifies(a, \"x\")";
            QueryLexer lexer = QueryLexer(query);

            bool exceptionThrown = false;
            try
            {
                lexer.lex();
            }
            catch (SyntaxError&)
            {
                exceptionThrown = true;
            }


            Assert::IsTrue(exceptionThrown);
        }
    
        TEST_METHOD(TestLexerExpression) {
            const std::string query = "assign a; Select a pattern a(_, \"x / y*(2 + z)\"_) such that Modifies(a, _\"(x - (y - (z -6)))\")";

            std::vector<std::string> expectedResult = std::vector<std::string>(
                { "assign", "a", ";", "Select", "a", "pattern", "a", "(", "_", ",",
                "\"x / y*(2 + z)\"", "_", ")", "such", "that", "Modifies", "(", "a", ",", "_", "\"(x - (y - (z -6)))\"", ")" });

            QueryLexer lexer = QueryLexer(query);
            std::vector<std::string> result = lexer.lex();

            Assert::AreEqual(expectedResult.size(), result.size());
            for (int i = 0; i < expectedResult.size(); i++) {
                Assert::AreEqual(expectedResult[i], result[i]);
            }
        }
    };
}