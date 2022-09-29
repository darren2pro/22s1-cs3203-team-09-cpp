#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include <vector>
#include <regex>
#include "QueryProcessingSubsystem/Relation.h"
#include "QueryProcessingSubsystem/Validator/QueryLexer.h"
#include "QueryProcessingSubsystem/Validator/QueryParser.h"
#include "QueryProcessingSubsystem/Validator/QueryParser.cpp"
#include "QueryProcessingSubsystem/Validator/SyntaxException.h"
#include "QueryProcessingSubsystem/Validator/SemanticException.h"
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

        //TEST_METHOD(TestParser1) {
        //    const std::string query = "stmt sss; Select sss ";
        //    QueryLexer lexer = QueryLexer(query);
        //    std::vector<std::string> tokens = lexer.lex();
        //    QueryParser parser = QueryParser(tokens);

        //    Query expectedResult = Query();

        //    expectedResult.declarations = std::vector<Declaration>();
        //    expectedResult.declarations.push_back(Declaration(Declaration::DesignEntity::Statement, "sss"));
        //    expectedResult.target = Declaration(Declaration::DesignEntity::Statement, "sss");
        //    expectedResult.relations = Relation();
        //    expectedResult.patterns = Pattern();
        //    expectedResult.results = std::vector<std::string>();

        //    Query* result = parser.parse();

        //    // declarations
        //    Assert::AreEqual(expectedResult.declarations.size(), result->declarations.size());
        //    for (int i = 0; i < expectedResult.declarations.size(); i++) {
        //        Assert::IsTrue(expectedResult.declarations[i].TYPE == result->declarations[i].TYPE);
        //        Assert::AreEqual(expectedResult.declarations[i].name, result->declarations[i].name);
        //    }

        //    //// target
        //    Assert::IsTrue(expectedResult.target.TYPE == result->target.TYPE);
        //    Assert::AreEqual(expectedResult.target.name, result->target.name);

        //    //// realtion
        //    Assert::IsTrue(expectedResult.relations.TYPE == result->relations.TYPE);
        //    Assert::AreEqual(expectedResult.relations.LEFT_ARG, result->relations.LEFT_ARG);
        //    Assert::AreEqual(expectedResult.relations.RIGHT_ARG, result->relations.RIGHT_ARG);

        //    //// pattern
        //    Assert::AreEqual(expectedResult.patterns.LEFT_ARG, result->patterns.LEFT_ARG);
        //    Assert::AreEqual(expectedResult.patterns.RIGHT_ARG, result->patterns.RIGHT_ARG);
        //    Assert::AreEqual(expectedResult.patterns.synonym, result->patterns.synonym);

        //    //// result
        //    Assert::AreEqual(expectedResult.results.size(), result->results.size());
        //    for (int i = 0; i < expectedResult.results.size(); i++) {
        //        Assert::AreEqual(expectedResult.results[i], result->results[i]);
        //    }
        //}
        //TEST_METHOD(TestParser) {
        //    const std::string query = "assign a; Select a pattern a(_, _\"x\"_) such that Modifies(a, \"x\")";
        //    QueryLexer lexer = QueryLexer(query);
        //    std::vector<std::string> tokens = lexer.lex();
        //    QueryParser parser = QueryParser(tokens);

        //    Query expectedResult = Query();

        //    expectedResult.declarations = std::vector<Declaration>();
        //    expectedResult.declarations.push_back(Declaration(Declaration::DesignEntity::Assignment, "a"));
        //    expectedResult.target = Declaration(Declaration::DesignEntity::Assignment, "a");
        //    expectedResult.relations = Relation(Relation::Types::Modifies, "a", "\"x\"");
        //    expectedResult.patterns = Pattern("a", "_", "_\"x\"_");
        //    expectedResult.results = std::vector<std::string>();

        //    Query* result = parser.parse();

        //    // declarations
        //    Assert::AreEqual(expectedResult.declarations.size(), result->declarations.size());
        //    for (int i = 0; i < expectedResult.declarations.size(); i++) {
        //        Assert::IsTrue(expectedResult.declarations[i].TYPE == result->declarations[i].TYPE);
        //        Assert::AreEqual(expectedResult.declarations[i].name, result->declarations[i].name);
        //    }

        //    //// target
        //    Assert::IsTrue(expectedResult.target.TYPE == result->target.TYPE);
        //    Assert::AreEqual(expectedResult.target.name, result->target.name);

        //    //// realtion
        //    Assert::IsTrue(expectedResult.relations.TYPE == result->relations.TYPE);
        //    Assert::AreEqual(expectedResult.relations.LEFT_ARG, result->relations.LEFT_ARG);
        //    Assert::AreEqual(expectedResult.relations.RIGHT_ARG, result->relations.RIGHT_ARG);

        //    //// pattern
        //    Assert::AreEqual(expectedResult.patterns.LEFT_ARG, result->patterns.LEFT_ARG);
        //    Assert::AreEqual(expectedResult.patterns.RIGHT_ARG, result->patterns.RIGHT_ARG);
        //    Assert::AreEqual(expectedResult.patterns.synonym, result->patterns.synonym);

        //    //// result
        //    Assert::AreEqual(expectedResult.results.size(), result->results.size());
        //    for (int i = 0; i < expectedResult.results.size(); i++) {
        //        Assert::AreEqual(expectedResult.results[i], result->results[i]);
        //    }
        //}

        //TEST_METHOD(TestParserCorrectMultipleDeclarations) {
        //    const std::string query = "assign a, a1; variable v, V; stmt s, s1, x; constant Select; Select Select such that Modifies(a, \"x\")";
        //    QueryLexer lexer = QueryLexer(query);
        //    std::vector<std::string> tokens = lexer.lex();
        //    QueryParser parser = QueryParser(tokens);

        //    Query expectedResult = Query();

        //    expectedResult.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a"), 
        //                                Declaration(Declaration::DesignEntity::Assignment, "a1"),  Declaration(Declaration::DesignEntity::Variable, "v"),
        //                                Declaration(Declaration::DesignEntity::Variable, "V"),  Declaration(Declaration::DesignEntity::Statement, "s"),
        //                                Declaration(Declaration::DesignEntity::Statement, "s1"),  Declaration(Declaration::DesignEntity::Statement, "x"),
        //                                Declaration(Declaration::DesignEntity::Constant, "Select")});
        //    expectedResult.target = Declaration(Declaration::DesignEntity::Constant, "Select");
        //    expectedResult.relations = Relation(Relation::Types::Modifies, "a", "\"x\"");
        //    expectedResult.patterns = Pattern();
        //    expectedResult.results = std::vector<std::string>();

        //    Query* result = parser.parse();

        //    // declarations
        //    Assert::AreEqual(expectedResult.declarations.size(), result->declarations.size());
        //    for (int i = 0; i < expectedResult.declarations.size(); i++) {
        //        Assert::IsTrue(expectedResult.declarations[i].TYPE == result->declarations[i].TYPE);
        //        Assert::AreEqual(expectedResult.declarations[i].name, result->declarations[i].name);
        //    }

        //    //// target
        //    Assert::IsTrue(expectedResult.target.TYPE == result->target.TYPE);
        //    Assert::AreEqual(expectedResult.target.name, result->target.name);

        //    //// realtion
        //    Assert::IsTrue(expectedResult.relations.TYPE == result->relations.TYPE);
        //    Assert::AreEqual(expectedResult.relations.LEFT_ARG, result->relations.LEFT_ARG);
        //    Assert::AreEqual(expectedResult.relations.RIGHT_ARG, result->relations.RIGHT_ARG);

        //    //// pattern
        //    Assert::AreEqual(expectedResult.patterns.LEFT_ARG, result->patterns.LEFT_ARG);
        //    Assert::AreEqual(expectedResult.patterns.RIGHT_ARG, result->patterns.RIGHT_ARG);
        //    Assert::AreEqual(expectedResult.patterns.synonym, result->patterns.synonym);

        //    //// result
        //    Assert::AreEqual(expectedResult.results.size(), result->results.size());
        //    for (int i = 0; i < expectedResult.results.size(); i++) {
        //        Assert::AreEqual(expectedResult.results[i], result->results[i]);
        //    }
        //}

        //TEST_METHOD(TestParserValidFollowsQuery) {
        //    std::string query = "assign a; Select a such that Follows(a, _)";
        //    QueryLexer lexer = QueryLexer(query);
        //    std::vector<std::string> tokens = lexer.lex();
        //    QueryParser parser = QueryParser(tokens);

        //    Query expectedResult = Query();

        //    expectedResult.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
        //    expectedResult.target = Declaration(Declaration::DesignEntity::Assignment, "a");
        //    expectedResult.relations = Relation(Relation::Types::Follows, "a", "_");
        //    expectedResult.patterns = Pattern();
        //    expectedResult.results = std::vector<std::string>();

        //    Query* result = parser.parse();

        //    // declarations
        //    Assert::AreEqual(expectedResult.declarations.size(), result->declarations.size());
        //    for (int i = 0; i < expectedResult.declarations.size(); i++) {
        //        Assert::IsTrue(expectedResult.declarations[i].TYPE == result->declarations[i].TYPE);
        //        Assert::AreEqual(expectedResult.declarations[i].name, result->declarations[i].name);
        //    }

        //    //// target
        //    Assert::IsTrue(expectedResult.target.TYPE == result->target.TYPE);
        //    Assert::AreEqual(expectedResult.target.name, result->target.name);

        //    //// realtion
        //    Assert::IsTrue(expectedResult.relations.TYPE == result->relations.TYPE);
        //    Assert::AreEqual(expectedResult.relations.LEFT_ARG, result->relations.LEFT_ARG);
        //    Assert::AreEqual(expectedResult.relations.RIGHT_ARG, result->relations.RIGHT_ARG);

        //    //// pattern
        //    Assert::AreEqual(expectedResult.patterns.LEFT_ARG, result->patterns.LEFT_ARG);
        //    Assert::AreEqual(expectedResult.patterns.RIGHT_ARG, result->patterns.RIGHT_ARG);
        //    Assert::AreEqual(expectedResult.patterns.synonym, result->patterns.synonym);

        //    //// result
        //    Assert::AreEqual(expectedResult.results.size(), result->results.size());
        //    for (int i = 0; i < expectedResult.results.size(); i++) {
        //        Assert::AreEqual(expectedResult.results[i], result->results[i]);
        //    }
        //}
        //        
        //TEST_METHOD(TestParserValidUsesQuery) {
        //    std::string query = "print a; variable v; Select a such that Uses(a, v)";
        //    QueryLexer lexer = QueryLexer(query);
        //    std::vector<std::string> tokens = lexer.lex();
        //    QueryParser parser = QueryParser(tokens);

        //    Query expectedResult = Query();

        //    expectedResult.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Print, "a"),  
        //                                                             Declaration(Declaration::DesignEntity::Variable, "v") });
        //    expectedResult.target = Declaration(Declaration::DesignEntity::Print, "a");
        //    expectedResult.relations = Relation(Relation::Types::Uses, "a", "v");
        //    expectedResult.patterns = Pattern();
        //    expectedResult.results = std::vector<std::string>();

        //    Query* result = parser.parse();

        //    // declarations
        //    Assert::AreEqual(expectedResult.declarations.size(), result->declarations.size());
        //    for (int i = 0; i < expectedResult.declarations.size(); i++) {
        //        Assert::IsTrue(expectedResult.declarations[i].TYPE == result->declarations[i].TYPE);
        //        Assert::AreEqual(expectedResult.declarations[i].name, result->declarations[i].name);
        //    }

        //    //// target
        //    Assert::IsTrue(expectedResult.target.TYPE == result->target.TYPE);
        //    Assert::AreEqual(expectedResult.target.name, result->target.name);

        //    //// realtion
        //    Assert::IsTrue(expectedResult.relations.TYPE == result->relations.TYPE);
        //    Assert::AreEqual(expectedResult.relations.LEFT_ARG, result->relations.LEFT_ARG);
        //    Assert::AreEqual(expectedResult.relations.RIGHT_ARG, result->relations.RIGHT_ARG);

        //    //// pattern
        //    Assert::AreEqual(expectedResult.patterns.LEFT_ARG, result->patterns.LEFT_ARG);
        //    Assert::AreEqual(expectedResult.patterns.RIGHT_ARG, result->patterns.RIGHT_ARG);
        //    Assert::AreEqual(expectedResult.patterns.synonym, result->patterns.synonym);

        //    //// result
        //    Assert::AreEqual(expectedResult.results.size(), result->results.size());
        //    for (int i = 0; i < expectedResult.results.size(); i++) {
        //        Assert::AreEqual(expectedResult.results[i], result->results[i]);
        //    }
        //}
        //
        //TEST_METHOD(TestParserValidModifiesQuery) {
        //    std::string query = "read a; Select a such that Modifies(a, _)";
        //    QueryLexer lexer = QueryLexer(query);
        //    std::vector<std::string> tokens = lexer.lex();
        //    QueryParser parser = QueryParser(tokens);

        //    Query expectedResult = Query();

        //    expectedResult.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Read, "a") });
        //    expectedResult.target = Declaration(Declaration::DesignEntity::Read, "a");
        //    expectedResult.relations = Relation(Relation::Types::Modifies, "a", "_");
        //    expectedResult.patterns = Pattern();
        //    expectedResult.results = std::vector<std::string>();

        //    Query* result = parser.parse();

        //    // declarations
        //    Assert::AreEqual(expectedResult.declarations.size(), result->declarations.size());
        //    for (int i = 0; i < expectedResult.declarations.size(); i++) {
        //        Assert::IsTrue(expectedResult.declarations[i].TYPE == result->declarations[i].TYPE);
        //        Assert::AreEqual(expectedResult.declarations[i].name, result->declarations[i].name);
        //    }

        //    //// target
        //    Assert::IsTrue(expectedResult.target.TYPE == result->target.TYPE);
        //    Assert::AreEqual(expectedResult.target.name, result->target.name);

        //    //// realtion
        //    Assert::IsTrue(expectedResult.relations.TYPE == result->relations.TYPE);
        //    Assert::AreEqual(expectedResult.relations.LEFT_ARG, result->relations.LEFT_ARG);
        //    Assert::AreEqual(expectedResult.relations.RIGHT_ARG, result->relations.RIGHT_ARG);

        //    //// pattern
        //    Assert::AreEqual(expectedResult.patterns.LEFT_ARG, result->patterns.LEFT_ARG);
        //    Assert::AreEqual(expectedResult.patterns.RIGHT_ARG, result->patterns.RIGHT_ARG);
        //    Assert::AreEqual(expectedResult.patterns.synonym, result->patterns.synonym);

        //    //// result
        //    Assert::AreEqual(expectedResult.results.size(), result->results.size());
        //    for (int i = 0; i < expectedResult.results.size(); i++) {
        //        Assert::AreEqual(expectedResult.results[i], result->results[i]);
        //    }
        //}
        //
        //TEST_METHOD(TestParserValidParentQuery) {
        //    std::string query = "assign a; Select a such that Parent(_, _)";
        //    QueryLexer lexer = QueryLexer(query);
        //    std::vector<std::string> tokens = lexer.lex();
        //    QueryParser parser = QueryParser(tokens);

        //    Query expectedResult = Query();

        //    expectedResult.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
        //    expectedResult.target = Declaration(Declaration::DesignEntity::Assignment, "a");
        //    expectedResult.relations = Relation(Relation::Types::Parent, "_", "_");
        //    expectedResult.patterns = Pattern();
        //    expectedResult.results = std::vector<std::string>();

        //    Query* result = parser.parse();

        //    // declarations
        //    Assert::AreEqual(expectedResult.declarations.size(), result->declarations.size());
        //    for (int i = 0; i < expectedResult.declarations.size(); i++) {
        //        Assert::IsTrue(expectedResult.declarations[i].TYPE == result->declarations[i].TYPE);
        //        Assert::AreEqual(expectedResult.declarations[i].name, result->declarations[i].name);
        //    }

        //    //// target
        //    Assert::IsTrue(expectedResult.target.TYPE == result->target.TYPE);
        //    Assert::AreEqual(expectedResult.target.name, result->target.name);

        //    //// realtion
        //    Assert::IsTrue(expectedResult.relations.TYPE == result->relations.TYPE);
        //    Assert::AreEqual(expectedResult.relations.LEFT_ARG, result->relations.LEFT_ARG);
        //    Assert::AreEqual(expectedResult.relations.RIGHT_ARG, result->relations.RIGHT_ARG);

        //    //// pattern
        //    Assert::AreEqual(expectedResult.patterns.LEFT_ARG, result->patterns.LEFT_ARG);
        //    Assert::AreEqual(expectedResult.patterns.RIGHT_ARG, result->patterns.RIGHT_ARG);
        //    Assert::AreEqual(expectedResult.patterns.synonym, result->patterns.synonym);

        //    //// result
        //    Assert::AreEqual(expectedResult.results.size(), result->results.size());
        //    for (int i = 0; i < expectedResult.results.size(); i++) {
        //        Assert::AreEqual(expectedResult.results[i], result->results[i]);
        //    }
        //}

        TEST_METHOD(TestParserDeclarationSyntaxError) {
            // 1
            const std::string query = "assign a, variable v; Select a such that Modifies(a, \"x\")";
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
            
            // 2
            const std::string query2 = "assign a; v; Select a such that Modifies(a, \"x\")";
            QueryLexer lexer2 = QueryLexer(query2);
            std::vector<std::string> tokens2 = lexer2.lex();
            QueryParser parser2 = QueryParser(tokens2);

            exceptionThrown = false;
            try
            {
                parser2.parse();
            }
            catch (SyntaxError&)
            {
                exceptionThrown = true;
            }

            Assert::IsTrue(exceptionThrown);            
            
            // 3
            const std::string query3 = "assign a Select a such that Modifies(a, \"x\")";
            QueryLexer lexer3 = QueryLexer(query3);
            std::vector<std::string> tokens3 = lexer3.lex();
            QueryParser parser3 = QueryParser(tokens3);

            exceptionThrown = false;
            try
            {
                parser3.parse();
            }
            catch (SyntaxError&)
            {
                exceptionThrown = true;
            }

            Assert::IsTrue(exceptionThrown);

        }        
              
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

        TEST_METHOD(TestParserSelectVariableNotDeclared) {
            const std::string query = "assign a; Select a1 such that Modifies(a, \"x\")";
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
        
        TEST_METHOD(TestParserSelectSyntaxError) {
            // 1
            std::string query = "assign a; select a such that Modifies(a, \"x\")";
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
            

            // 2
            query = "assign a; a such that Modifies(a, \"x\")";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;

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
        
        TEST_METHOD(TestParserSuchThatSyntaxError) {
            // 1
            std::string query = "assign a; Select a Such that Modifies(a, \"x\")";
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
            

            // 2
            query = "assign a; Select a suchthat Modifies(a, \"x\")";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;

            try
            {
                parser.parse();
            }
            catch (SyntaxError&)
            {
                exceptionThrown = true;
            }

            Assert::IsTrue(exceptionThrown);           
            
            // 3
            query = "assign a; Select a such that Modifies( , \"x\")";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;

            try
            {
                parser.parse();
            }
            catch (SyntaxError&)
            {
                exceptionThrown = true;
            }

            Assert::IsTrue(exceptionThrown);
            
            // 4
            query = "assign a; a such that pattern(a, \"x\")";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;

            try
            {
                parser.parse();
            }
            catch (SyntaxError&)
            {
                exceptionThrown = true;
            }

            Assert::IsTrue(exceptionThrown);
            
            // 5
            query = "assign a; Select a such that Modifies a, \"x\")";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;

            try
            {
                parser.parse();
            }
            catch (SyntaxError&)
            {
                exceptionThrown = true;
            }

            Assert::IsTrue(exceptionThrown);
            
            // 6
            query = "assign a; Select a such that Modifies(a \"x\")";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;

            try
            {
                parser.parse();
            }
            catch (SyntaxError&)
            {
                exceptionThrown = true;
            }

            Assert::IsTrue(exceptionThrown);
            
            // 7
            query = "assign a; Select a such that Modifies(a, \"x\"";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;

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
        
        TEST_METHOD(TestParserUsesModifiesSemanticError) {
            // first arg underscore
            std::string query = "assign a; Select a such that Modifies(_, \"x\")";
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
            
            query = "assign a; Select a such that Uses(_, \"x\")";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;
            try
            {
                parser.parse();
            }
            catch (SemanticError&)
            {
                exceptionThrown = true;
            }
            Assert::IsTrue(exceptionThrown);            
            

            // First arg invalid type
            query = "print a; Select a such that Modifies(a, \"x\")";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;

            try
            {
                parser.parse();
            }
            catch (SemanticError&)
            {
                exceptionThrown = true;
            }

            Assert::IsTrue(exceptionThrown);                 
            
            query = "read a; Select a such that Uses(a, \"x\")";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;

            try
            {
                parser.parse();
            }
            catch (SemanticError&)
            {
                exceptionThrown = true;
            }

            Assert::IsTrue(exceptionThrown);               
            

            // Second arg invalid type
            query = "assign a; stmt s; Select a such that Modifies(a, s)";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;

            try
            {
                parser.parse();
            }
            catch (SemanticError&)
            {
                exceptionThrown = true;
            }

            Assert::IsTrue(exceptionThrown);                 
            
            query = "assign a; stmt s; Select a such that Uses(a, s)";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;

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
        
        TEST_METHOD(TestParserPatternSyntaxError) {
            // 1
            std::string query = "assign a; Select a pattern (_, _)";
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
            

            // 2
            query = "assign a; Select a pattern a _, \"x\")";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;

            try
            {
                parser.parse();
            }
            catch (SyntaxError&)
            {
                exceptionThrown = true;
            }

            Assert::IsTrue(exceptionThrown);           
            
            // 3
            query = "assign a; Select a pattern a(_ \"x\")";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;

            try
            {
                parser.parse();
            }
            catch (SyntaxError&)
            {
                exceptionThrown = true;
            }

            Assert::IsTrue(exceptionThrown);
            
            // 4
            query = "assign a; Select a pattern a(_, )";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;

            try
            {
                parser.parse();
            }
            catch (SyntaxError&)
            {
                exceptionThrown = true;
            }

            Assert::IsTrue(exceptionThrown);
            
            // 5
            query = "assign a; Select a pattern a(_, \"x\"";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;

            try
            {
                parser.parse();
            }
            catch (SyntaxError&)
            {
                exceptionThrown = true;
            }

            Assert::IsTrue(exceptionThrown);
            
            // 6
            query = "assign a; Select a patterna(_, \"x\")";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;

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
        
        TEST_METHOD(TestParserPatternSemanticError) {
            // syn-assign not declared
            std::string query = "assign a; Select a pattern a1(_, \"x\")";
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
            
            // syn-assign not not an assignment
            query = "stmt a; Select a pattern a(_, \"x\")";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;
            try
            {
                parser.parse();
            }
            catch (SemanticError&)
            {
                exceptionThrown = true;
            }
            Assert::IsTrue(exceptionThrown);            
            

            // First arg invalid type
            query = "assign a; stmt s; Select a pattern a(s, \"x\")";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;

            try
            {
                parser.parse();
            }
            catch (SemanticError&)
            {
                exceptionThrown = true;
            }

            Assert::IsTrue(exceptionThrown);                          
            

            // First arg not declared
            query = "assign a; Select a pattern a(v, \"x\")";
            lexer = QueryLexer(query);
            tokens = lexer.lex();
            parser = QueryParser(tokens);

            exceptionThrown = false;

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
    };
}