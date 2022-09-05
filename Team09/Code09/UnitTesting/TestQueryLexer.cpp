#include "stdafx.h"
#include "targetver.h"
#include "CppUnitTest.h"
#include <string>
#include <vector>
#include "../SPA/QueryLexer.h"
#include "../SPA/QPSValidatorException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestQueryLexer) {
    
    public:

        TEST_METHOD(TestLexer) {
            const std::string query = "assign a; Select a pattern a(_, _\"x\"_) such that Modifies(a, \"x\")";

            std::vector<std::string> expectedResult;
            expectedResult.push_back("assign");
            expectedResult.push_back("a");
            expectedResult.push_back(";");
            expectedResult.push_back("Select");
            expectedResult.push_back("a");
            expectedResult.push_back("pattern");
            expectedResult.push_back("a");
            expectedResult.push_back("(");
            expectedResult.push_back("_");
            expectedResult.push_back(",");
            expectedResult.push_back("_");
            expectedResult.push_back("\"x\"");
            expectedResult.push_back("_");
            expectedResult.push_back(")");
            expectedResult.push_back("such that");
            expectedResult.push_back("Modifies");
            expectedResult.push_back("(");
            expectedResult.push_back("a");
            expectedResult.push_back(",");
            expectedResult.push_back("\"x\"");
            expectedResult.push_back(")");

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

            std::vector<std::string> expectedResult;
            expectedResult.push_back("assign");
            expectedResult.push_back("a");
            expectedResult.push_back(";");
            expectedResult.push_back("Select");
            expectedResult.push_back("a");
            expectedResult.push_back("pattern");
            expectedResult.push_back("a");
            expectedResult.push_back("(");
            expectedResult.push_back("_");
            expectedResult.push_back(",");
            expectedResult.push_back("_");
            expectedResult.push_back("\"x\"");
            expectedResult.push_back("_");
            expectedResult.push_back(")");
            expectedResult.push_back("such that");
            expectedResult.push_back("Modifies");
            expectedResult.push_back("(");
            expectedResult.push_back("a");
            expectedResult.push_back(",");
            expectedResult.push_back("\"x\"");
            expectedResult.push_back(")");

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

        TEST_METHOD(TestLexerException) {
            const std::string query = "assign a$; Select a pattern a(_, _\"x\"_) such that Modifies(a, \"x\")";
            QueryLexer lexer = QueryLexer(query);

            bool exceptionThrown = false;
            try
            {
                lexer.lex();
            }
            catch (QueryLexerException&)
            {
                exceptionThrown = true;
            }


            Assert::IsTrue(exceptionThrown);
        }
    };
}