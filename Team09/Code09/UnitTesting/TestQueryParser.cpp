#include "stdafx.h"
#include "targetver.h"
#include "CppUnitTest.h"
#include <string>
#include <vector>
#include "../SPA/Relation.h"
#include "../SPA/QueryParser.h"
#include "../SPA/QueryLexer.h"
#include "../SPA/QPSValidatorException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestQueryParser) {

    public:

        TEST_METHOD(TestParser) {
            const std::string query = "assign a; Select a pattern a(_, _\"x\"_) such that Modifies(a, \"x\")";
            QueryLexer lexer = QueryLexer(query);
            std::vector<std::string> tokens = lexer.lex();
            QueryParser parser = QueryParser(tokens);

            Query expectedResult = Query();

            expectedResult.declarations = std::vector<std::string>({ "a" });
            expectedResult.target = "a";
            expectedResult.relations = Relation(Relation::Types::Modifies, "a", "\"x\"");
            expectedResult.patterns = std::vector<std::string>({ "a", "_", "_\"x\"_" });
            expectedResult.results = std::vector<std::string>();

            Query result = parser.parse();

            // declarations
            Assert::AreEqual(expectedResult.declarations.size(), result.declarations.size());
            for (int i = 0; i < expectedResult.declarations.size(); i++) {
                Assert::AreEqual(expectedResult.declarations[i], result.declarations[i]);
            }

            // target
            Assert::AreEqual(expectedResult.target, result.target);

            // realtion
            Assert::IsTrue(expectedResult.relations.TYPE == result.relations.TYPE);
            Assert::AreEqual(expectedResult.relations.LEFT_ARG, result.relations.LEFT_ARG);
            Assert::AreEqual(expectedResult.relations.RIGHT_ARG, result.relations.RIGHT_ARG);

            // pattern
            Assert::AreEqual(expectedResult.patterns.size(), result.patterns.size());
            for (int i = 0; i < expectedResult.patterns.size(); i++) {
                Assert::AreEqual(expectedResult.patterns[i], result.patterns[i]);
            }

            // result
            Assert::AreEqual(expectedResult.results.size(), result.results.size());
            for (int i = 0; i < expectedResult.results.size(); i++) {
                Assert::AreEqual(expectedResult.results[i], result.results[i]);
            }
        }

        TEST_METHOD(TestParserException) {
            const std::string query = "assign a Select a pattern a(_, _\"x\"_) such that Modifies(a, \"x\")";       // missing ';'
            QueryLexer lexer = QueryLexer(query);
            std::vector<std::string> tokens = lexer.lex();
            QueryParser parser = QueryParser(tokens);

            bool exceptionThrown = false;
            try
            {
                parser.parse();
            }
            catch (QueryParserException&)
            {
                exceptionThrown = true;
            }


            Assert::IsTrue(exceptionThrown);
        }
    };
}