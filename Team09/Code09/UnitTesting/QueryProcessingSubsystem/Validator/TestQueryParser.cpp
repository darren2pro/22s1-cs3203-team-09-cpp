#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include <vector>
#include <regex>
#include "QueryProcessingSubsystem/Relation.h"
#include "QueryProcessingSubsystem/Validator/QueryLexer.h"
#include "QueryProcessingSubsystem/Validator/QueryParser.h"
#include "QueryProcessingSubsystem/Validator/QueryParser.cpp"
#include "QueryProcessingSubsystem/Validator/QPSValidatorException.h"
#include "QueryProcessingSubsystem/Pattern.h"
#include "QueryProcessingSubsystem/Declaration.h"

#include <iostream>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestQueryParser) {

    public:
        TEST_METHOD(TestParserRegex) {
            // check that the regular expressions are correct

            // integer_re
            Assert::IsTrue(std::regex_match("0", parserre::integer_re));
            Assert::IsTrue(std::regex_match("10000", parserre::integer_re));
            Assert::IsTrue(std::regex_match("4329", parserre::integer_re));
            Assert::IsTrue(std::regex_match("502", parserre::integer_re));

            Assert::IsFalse(std::regex_match("01", parserre::integer_re));
            Assert::IsFalse(std::regex_match("000", parserre::integer_re));
            Assert::IsFalse(std::regex_match("1a", parserre::integer_re));
            Assert::IsFalse(std::regex_match("a", parserre::integer_re));
            Assert::IsFalse(std::regex_match("", parserre::integer_re));


            // synonym_re
            Assert::IsTrue(std::regex_match("x", parserre::synonym_re));
            Assert::IsTrue(std::regex_match("X", parserre::synonym_re));
            Assert::IsTrue(std::regex_match("a0A3", parserre::synonym_re));

            Assert::IsFalse(std::regex_match("1", parserre::synonym_re));
            Assert::IsFalse(std::regex_match("1B", parserre::synonym_re));
            Assert::IsFalse(std::regex_match("x_x", parserre::synonym_re));
            Assert::IsFalse(std::regex_match("", parserre::synonym_re));


            // design_enteties_re
            Assert::IsTrue(std::regex_match("stmt", parserre::design_enteties_re));
            Assert::IsTrue(std::regex_match("read", parserre::design_enteties_re));
            Assert::IsTrue(std::regex_match("print", parserre::design_enteties_re));
            Assert::IsTrue(std::regex_match("while", parserre::design_enteties_re));
            Assert::IsTrue(std::regex_match("if", parserre::design_enteties_re));
            Assert::IsTrue(std::regex_match("assign", parserre::design_enteties_re));
            Assert::IsTrue(std::regex_match("variable", parserre::design_enteties_re));
            Assert::IsTrue(std::regex_match("constant", parserre::design_enteties_re));
            Assert::IsTrue(std::regex_match("procedure", parserre::design_enteties_re));
            Assert::IsTrue(std::regex_match("call", parserre::design_enteties_re));

            Assert::IsFalse(std::regex_match("STMT", parserre::design_enteties_re));
            Assert::IsFalse(std::regex_match("Read", parserre::design_enteties_re));
            Assert::IsFalse(std::regex_match("print|while", parserre::design_enteties_re));
            Assert::IsFalse(std::regex_match("xyz", parserre::design_enteties_re));
            Assert::IsFalse(std::regex_match("", parserre::design_enteties_re));


            // relation_re
            Assert::IsTrue(std::regex_match("Follows", parserre::relation_re));
            Assert::IsTrue(std::regex_match("Follows*", parserre::relation_re));
            Assert::IsTrue(std::regex_match("Parent", parserre::relation_re));
            Assert::IsTrue(std::regex_match("Parent*", parserre::relation_re));
            Assert::IsTrue(std::regex_match("Uses", parserre::relation_re));
            Assert::IsTrue(std::regex_match("Modifies", parserre::relation_re));

            Assert::IsFalse(std::regex_match("modifies", parserre::relation_re));
            Assert::IsFalse(std::regex_match("USES", parserre::relation_re));
            Assert::IsFalse(std::regex_match("xyz", parserre::relation_re));
            Assert::IsFalse(std::regex_match("", parserre::relation_re));


            // stmtRef_re
            Assert::IsTrue(std::regex_match("_", parserre::stmtRef_re));
            Assert::IsTrue(std::regex_match("0", parserre::stmtRef_re));
            Assert::IsTrue(std::regex_match("x", parserre::stmtRef_re));
            Assert::IsTrue(std::regex_match("Follows", parserre::stmtRef_re));
            Assert::IsTrue(std::regex_match("Select", parserre::stmtRef_re));

            Assert::IsFalse(std::regex_match("011", parserre::stmtRef_re));
            Assert::IsFalse(std::regex_match("\"x\"", parserre::stmtRef_re));
            Assert::IsFalse(std::regex_match("\"0\"", parserre::stmtRef_re));
            Assert::IsFalse(std::regex_match("", parserre::stmtRef_re));


            // entRef_re
            Assert::IsTrue(std::regex_match("_", parserre::entRef_re));
            Assert::IsTrue(std::regex_match("x", parserre::entRef_re));
            Assert::IsTrue(std::regex_match("\"x\"", parserre::entRef_re));
            Assert::IsTrue(std::regex_match("\"y1\"", parserre::entRef_re));

            Assert::IsFalse(std::regex_match("0", parserre::entRef_re));
            Assert::IsFalse(std::regex_match("\"0\"", parserre::entRef_re));
            Assert::IsFalse(std::regex_match("", parserre::entRef_re));


            // expressionSpec_re
            Assert::IsTrue(std::regex_match("_", parserre::expressionSpec_re));
            Assert::IsTrue(std::regex_match("\"x\"", parserre::expressionSpec_re));
            Assert::IsTrue(std::regex_match("\"0\"", parserre::expressionSpec_re));

            Assert::IsFalse(std::regex_match("x", parserre::expressionSpec_re));
            Assert::IsFalse(std::regex_match("0", parserre::expressionSpec_re));
            Assert::IsFalse(std::regex_match("", parserre::expressionSpec_re));
        }

        TEST_METHOD(TestParser) {
            const std::string query = "assign a; Select a pattern a(_, _\"x\"_) such that Modifies(a, \"x\")";
            QueryLexer lexer = QueryLexer(query);
            std::vector<std::string> tokens = lexer.lex();
            QueryParser parser = QueryParser(tokens);

            Query expectedResult = Query();

            expectedResult.declarations = std::vector<Declaration>();
            expectedResult.declarations.push_back(Declaration(Declaration::DesignEntity::Assignment, "a"));
            expectedResult.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult.relations = Relation(Relation::Types::Modifies, "a", "\"x\"");
            expectedResult.patterns = Pattern("a", "_", "_\"x\"_");
            expectedResult.results = std::vector<std::string>();

            Query* result = parser.parse();

            // declarations
            Assert::AreEqual(expectedResult.declarations.size(), result->declarations.size());
            for (int i = 0; i < expectedResult.declarations.size(); i++) {
                Assert::IsTrue(expectedResult.declarations[i].TYPE == result->declarations[i].TYPE);
                Assert::AreEqual(expectedResult.declarations[i].name, result->declarations[i].name);
            }

            //// target
            Assert::IsTrue(expectedResult.target.TYPE == result->target.TYPE);
            Assert::AreEqual(expectedResult.target.name, result->target.name);

            //// realtion
            Assert::IsTrue(expectedResult.relations.TYPE == result->relations.TYPE);
            Assert::AreEqual(expectedResult.relations.LEFT_ARG, result->relations.LEFT_ARG);
            Assert::AreEqual(expectedResult.relations.RIGHT_ARG, result->relations.RIGHT_ARG);

            //// pattern
            Assert::AreEqual(expectedResult.patterns.LEFT_ARG, result->patterns.LEFT_ARG);
            Assert::AreEqual(expectedResult.patterns.RIGHT_ARG, result->patterns.RIGHT_ARG);
            Assert::AreEqual(expectedResult.patterns.synonym, result->patterns.synonym);

            //// result
            Assert::AreEqual(expectedResult.results.size(), result->results.size());
            for (int i = 0; i < expectedResult.results.size(); i++) {
                Assert::AreEqual(expectedResult.results[i], result->results[i]);
            }
        }

        TEST_METHOD(TestParserCorrectMultipleDeclarations) {
            const std::string query = "assign a, a1; variable v, V; stmt s, s1, x; constant Select; Select Select such that Modifies(a, \"x\")";
            QueryLexer lexer = QueryLexer(query);
            std::vector<std::string> tokens = lexer.lex();
            QueryParser parser = QueryParser(tokens);

            Query expectedResult = Query();

            expectedResult.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a"), 
                                        Declaration(Declaration::DesignEntity::Assignment, "a1"),  Declaration(Declaration::DesignEntity::Variable, "v"),
                                        Declaration(Declaration::DesignEntity::Variable, "V"),  Declaration(Declaration::DesignEntity::Statement, "s"),
                                        Declaration(Declaration::DesignEntity::Statement, "s1"),  Declaration(Declaration::DesignEntity::Statement, "x"),
                                        Declaration(Declaration::DesignEntity::Constant, "Select")});
            expectedResult.target = Declaration(Declaration::DesignEntity::Constant, "Select");
            expectedResult.relations = Relation(Relation::Types::Modifies, "a", "\"x\"");
            expectedResult.patterns = Pattern();
            expectedResult.results = std::vector<std::string>();

            Query* result = parser.parse();

            // declarations
            Assert::AreEqual(expectedResult.declarations.size(), result->declarations.size());
            for (int i = 0; i < expectedResult.declarations.size(); i++) {
                Assert::IsTrue(expectedResult.declarations[i].TYPE == result->declarations[i].TYPE);
                Assert::AreEqual(expectedResult.declarations[i].name, result->declarations[i].name);
            }

            //// target
            Assert::IsTrue(expectedResult.target.TYPE == result->target.TYPE);
            Assert::AreEqual(expectedResult.target.name, result->target.name);

            //// realtion
            Assert::IsTrue(expectedResult.relations.TYPE == result->relations.TYPE);
            Assert::AreEqual(expectedResult.relations.LEFT_ARG, result->relations.LEFT_ARG);
            Assert::AreEqual(expectedResult.relations.RIGHT_ARG, result->relations.RIGHT_ARG);

            //// pattern
            Assert::AreEqual(expectedResult.patterns.LEFT_ARG, result->patterns.LEFT_ARG);
            Assert::AreEqual(expectedResult.patterns.RIGHT_ARG, result->patterns.RIGHT_ARG);
            Assert::AreEqual(expectedResult.patterns.synonym, result->patterns.synonym);

            //// result
            Assert::AreEqual(expectedResult.results.size(), result->results.size());
            for (int i = 0; i < expectedResult.results.size(); i++) {
                Assert::AreEqual(expectedResult.results[i], result->results[i]);
            }
        }

        //TEST_METHOD(TestParserDeclarationSyntaxError) {
        //    // 1
        //    const std::string query = "assign a, variable v; Select a such that Modifies(a, \"x\")";
        //    QueryLexer lexer = QueryLexer(query);
        //    std::vector<std::string> tokens = lexer.lex();
        //    QueryParser parser = QueryParser(tokens);

        //    bool semanticexceptionThrown = false;
        //    bool syntaxexceptionThrown = false;
        //    try
        //    {
        //        parser.parse();
        //    }
        //    catch (SemanticError&)
        //    {
        //        semanticexceptionThrown = true;

        //    } catch (SyntaxError&)
        //    {
        //        syntaxexceptionThrown = true;

        //    }

        //    Assert::IsFalse(semanticexceptionThrown);
        //    Assert::IsTrue(syntaxexceptionThrown);
            
            //// 2
            //const std::string query2 = "assign a; v; Select a such that Modifies(a, \"x\")";
            //QueryLexer lexer2 = QueryLexer(query2);
            //std::vector<std::string> tokens2 = lexer2.lex();
            //QueryParser parser2 = QueryParser(tokens2);

            //exceptionThrown = false;
            //try
            //{
            //    parser2.parse();
            //}
            //catch (SyntaxError&)
            //{
            //    exceptionThrown = true;
            //}

            //Assert::IsTrue(exceptionThrown);            
            //
            //// 3
            //const std::string query3 = "assign a Select a such that Modifies(a, \"x\")";
            //QueryLexer lexer3 = QueryLexer(query3);
            //std::vector<std::string> tokens3 = lexer3.lex();
            //QueryParser parser3 = QueryParser(tokens3);

            //exceptionThrown = false;
            //try
            //{
            //    parser3.parse();
            //}
            //catch (SyntaxError&)
            //{
            //    exceptionThrown = true;
            //}

            //Assert::IsTrue(exceptionThrown);

        //}        
                
        TEST_METHOD(TestParserDuplicateSynonymError) {
            const std::string query = "assign a; variable a; Select a such that Modifies(a, \"x\")";
            QueryLexer lexer = QueryLexer(query);
            std::vector<std::string> tokens = lexer.lex();
            QueryParser parser = QueryParser(tokens);

            bool exceptionThrown = false;
            try
            {
                parser.parse();
            }
            catch (SemanticError&)
            {
                exceptionThrown = true;
            }


            Assert::IsTrue(exceptionThrown);

        }


        //TEST_METHOD(TestParserException) {
        //    const std::string query = "assign a Select a pattern a(_, _\"x\"_) such that Modifies(a, \"x\")";       // missing ';'
        //    QueryLexer lexer = QueryLexer(query);
        //    std::vector<std::string> tokens = lexer.lex();
        //    QueryParser parser = QueryParser(tokens);

        //    bool exceptionThrown = false;
        //    try
        //    {
        //        parser.parse();
        //    }
        //    catch (SyntaxError&)
        //    {
        //        exceptionThrown = true;
        //    }


        //    Assert::IsTrue(exceptionThrown);
        //}
    };
}