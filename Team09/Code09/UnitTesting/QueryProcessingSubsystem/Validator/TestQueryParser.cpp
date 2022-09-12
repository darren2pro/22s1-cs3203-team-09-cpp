#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include <vector>
#include "QueryProcessingSubsystem/Relation.h"
#include "QueryProcessingSubsystem/Validator/QueryLexer.h"
#include "QueryProcessingSubsystem/Validator/QueryParser.h"
#include "QueryProcessingSubsystem/Validator/QPSValidatorException.h"
#include "QueryProcessingSubsystem/Pattern.h"
#include "QueryProcessingSubsystem/Declaration.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestQueryParser) {

    public:

        //TEST_METHOD(TestParser) {
        //    const std::string query = "assign a; Select a pattern a(_, _\"x\"_) such that Modifies(a, \"x\")";
        //    QueryLexer lexer = QueryLexer(query);
        //    std::vector<std::string> tokens = lexer.lex();
        //    QueryParser parser = QueryParser(tokens);

        //    Query expectedResult = Query();

        //    expectedResult.declarations = std::vector<Declaration>();
        //    expectedResult.declarations.push_back(Declaration(Declaration::DesignEntity::Assignment, "a"));
        //    expectedResult.target = "a";
        //    expectedResult.relations = Relation(Relation::Types::Modifies, "a", "\"x\"");
        //    expectedResult.patterns = Pattern("a", "_", "_\"x\"_");
        //    expectedResult.results = std::vector<std::string>();

        //    Query* result = parser.parse();

        //    // declarations
        //    Assert::AreEqual(expectedResult.declarations.size(), result->declarations.size());
        //    for (int i = 0; i < expectedResult.declarations.size(); i++) {
        //        Assert::AreEqual(expectedResult.declarations[i].TYPE, result->declarations[i].TYPE);
        //        Assert::AreEqual(expectedResult.declarations[i].name, result->declarations[i].name);
        //    }

        //    // target
        //    Assert::AreEqual(expectedResult.target, result->target);

        //    // realtion
        //    Assert::IsTrue(expectedResult.relations.TYPE == result->relations.TYPE);
        //    Assert::AreEqual(expectedResult.relations.LEFT_ARG, result->relations.LEFT_ARG);
        //    Assert::AreEqual(expectedResult.relations.RIGHT_ARG, result->relations.RIGHT_ARG);

        //    // pattern
        //    Assert::AreEqual(expectedResult.patterns.LEFT_ARG, result->patterns.LEFT_ARG);
        //    Assert::AreEqual(expectedResult.patterns.RIGHT_ARG, result->patterns.RIGHT_ARG);
        //    Assert::AreEqual(expectedResult.patterns.synonym, result->patterns.synonym);

        //    // result
        //    Assert::AreEqual(expectedResult.results.size(), result->results.size());
        //    for (int i = 0; i < expectedResult.results.size(); i++) {
        //        Assert::AreEqual(expectedResult.results[i], result->results[i]);
        //    }
        //}

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
            catch (SyntaxError&)
            {
                exceptionThrown = true;
            }


            Assert::IsTrue(exceptionThrown);
        }
    };
}