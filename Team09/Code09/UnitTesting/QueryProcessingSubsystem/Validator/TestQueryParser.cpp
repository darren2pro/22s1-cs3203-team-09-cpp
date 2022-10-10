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
#include "QueryProcessingSubsystem/Reference.h"
#include "QueryProcessingSubsystem/Expression.h"

#include <iostream>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestQueryParser) {

    public:
        TEST_METHOD(Testparsergex) {
            // check that the regular expressions are correct

            // integer_re
            Assert::IsTrue(std::regex_match("0", parser::integer_re));
            Assert::IsTrue(std::regex_match("10000", parser::integer_re));
            Assert::IsTrue(std::regex_match("4329", parser::integer_re));
            Assert::IsTrue(std::regex_match("502", parser::integer_re));

            Assert::IsFalse(std::regex_match("01", parser::integer_re));
            Assert::IsFalse(std::regex_match("000", parser::integer_re));
            Assert::IsFalse(std::regex_match("1a", parser::integer_re));
            Assert::IsFalse(std::regex_match("a", parser::integer_re));
            Assert::IsFalse(std::regex_match("", parser::integer_re));


            // synonym_re
            Assert::IsTrue(std::regex_match("x", parser::synonym_re));
            Assert::IsTrue(std::regex_match("X", parser::synonym_re));
            Assert::IsTrue(std::regex_match("a0A3", parser::synonym_re));

            Assert::IsFalse(std::regex_match("1", parser::synonym_re));
            Assert::IsFalse(std::regex_match("1B", parser::synonym_re));
            Assert::IsFalse(std::regex_match("x_x", parser::synonym_re));
            Assert::IsFalse(std::regex_match("", parser::synonym_re));


            // design_enteties_re
            Assert::IsTrue(std::regex_match("stmt", parser::design_enteties_re));
            Assert::IsTrue(std::regex_match("read", parser::design_enteties_re));
            Assert::IsTrue(std::regex_match("print", parser::design_enteties_re));
            Assert::IsTrue(std::regex_match("while", parser::design_enteties_re));
            Assert::IsTrue(std::regex_match("if", parser::design_enteties_re));
            Assert::IsTrue(std::regex_match("assign", parser::design_enteties_re));
            Assert::IsTrue(std::regex_match("variable", parser::design_enteties_re));
            Assert::IsTrue(std::regex_match("constant", parser::design_enteties_re));
            Assert::IsTrue(std::regex_match("procedure", parser::design_enteties_re));
            Assert::IsTrue(std::regex_match("call", parser::design_enteties_re));

            Assert::IsFalse(std::regex_match("STMT", parser::design_enteties_re));
            Assert::IsFalse(std::regex_match("Read", parser::design_enteties_re));
            Assert::IsFalse(std::regex_match("print|while", parser::design_enteties_re));
            Assert::IsFalse(std::regex_match("xyz", parser::design_enteties_re));
            Assert::IsFalse(std::regex_match("", parser::design_enteties_re));


            // relation_re
            Assert::IsTrue(std::regex_match("Follows", parser::relation_re));
            Assert::IsTrue(std::regex_match("Parent*", parser::relation_re));
            Assert::IsTrue(std::regex_match("Parent", parser::relation_re));
            Assert::IsTrue(std::regex_match("Parent*", parser::relation_re));
            Assert::IsTrue(std::regex_match("Uses", parser::relation_re));
            Assert::IsTrue(std::regex_match("Modifies", parser::relation_re));

            Assert::IsFalse(std::regex_match("modifies", parser::relation_re));
            Assert::IsFalse(std::regex_match("USES", parser::relation_re));
            Assert::IsFalse(std::regex_match("xyz", parser::relation_re));
            Assert::IsFalse(std::regex_match("", parser::relation_re));


            // stmtRef_re
            Assert::IsTrue(std::regex_match("_", parser::stmtRef_re));
            Assert::IsTrue(std::regex_match("0", parser::stmtRef_re));
            Assert::IsTrue(std::regex_match("x", parser::stmtRef_re));
            Assert::IsTrue(std::regex_match("Follows", parser::stmtRef_re));
            Assert::IsTrue(std::regex_match("Select", parser::stmtRef_re));

            Assert::IsFalse(std::regex_match("011", parser::stmtRef_re));
            Assert::IsFalse(std::regex_match("\"x\"", parser::stmtRef_re));
            Assert::IsFalse(std::regex_match("\"0\"", parser::stmtRef_re));
            Assert::IsFalse(std::regex_match("", parser::stmtRef_re));


            // entRef_re
            Assert::IsTrue(std::regex_match("_", parser::entRef_re));
            Assert::IsTrue(std::regex_match("x", parser::entRef_re));
            Assert::IsTrue(std::regex_match("\"x\"", parser::entRef_re));
            Assert::IsTrue(std::regex_match("\"y1\"", parser::entRef_re));

            Assert::IsFalse(std::regex_match("0", parser::entRef_re));
            Assert::IsFalse(std::regex_match("\"0\"", parser::entRef_re));
            Assert::IsFalse(std::regex_match("", parser::entRef_re));


            // expressionSpec_re
            Assert::IsTrue(std::regex_match("_", parser::expressionSpec_re));
            Assert::IsTrue(std::regex_match("\"x\"", parser::expressionSpec_re));
            Assert::IsTrue(std::regex_match("\"0\"", parser::expressionSpec_re));

            Assert::IsFalse(std::regex_match("x", parser::expressionSpec_re));
            Assert::IsFalse(std::regex_match("0", parser::expressionSpec_re));
            Assert::IsFalse(std::regex_match("", parser::expressionSpec_re));
        }

        TEST_METHOD(TestParserBasicQuery) {
            const std::string query = "stmt sss; Select sss";
            QueryLexer lexer = QueryLexer(query);
            std::vector<std::string> tokens = lexer.lex();
            QueryParser parser = QueryParser(tokens);

            Query expectedResult = Query();

            expectedResult.declarations = std::vector<Declaration>();
            expectedResult.declarations.push_back(Declaration(Declaration::DesignEntity::Statement, "sss"));
            expectedResult.target = Declaration(Declaration::DesignEntity::Statement, "sss");
            expectedResult.relations = std::vector<Relation>();
            expectedResult.patterns = std::vector<Pattern>();
            expectedResult.results = std::vector<std::string>();

            Query* result = parser.parse();

            Assert::IsTrue( *result == expectedResult);
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
            expectedResult.relations = std::vector<Relation>({ Relation(Relation::Types::ModifiesS, Reference(Declaration(Declaration::DesignEntity::Assignment, "a")), Reference("\"x\"")) });
            expectedResult.patterns = std::vector<Pattern>({ Pattern(Pattern::Types::Assign, Declaration(Declaration::DesignEntity::Assignment, "a"), Reference("_"), Expression("_\"x\"_")) });
            expectedResult.results = std::vector<std::string>();

            Query* result = parser.parse();

            Assert::IsTrue(*result == expectedResult);
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
            expectedResult.relations = std::vector<Relation>({ Relation(Relation::Types::ModifiesS, Reference(Declaration(Declaration::DesignEntity::Assignment, "a")), Reference("\"x\"")) });
            expectedResult.patterns = std::vector<Pattern>();
            expectedResult.results = std::vector<std::string>();

            Query* result = parser.parse();

            Assert::IsTrue(*result == expectedResult);
        }

        TEST_METHOD(TestParserValidPAttern) {
            //    a(_, _)
            const std::string query1 = "assign a; Select a pattern a(_, _)";
            QueryLexer lexer1 = QueryLexer(query1);
            std::vector<std::string> tokens1 = lexer1.lex();
            QueryParser parser1 = QueryParser(tokens1);

            Query expectedResult1 = Query();

            expectedResult1.declarations = std::vector<Declaration>();
            expectedResult1.declarations.push_back(Declaration(Declaration::DesignEntity::Assignment, "a"));
            expectedResult1.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult1.relations = std::vector<Relation>();
            expectedResult1.patterns = std::vector<Pattern>({ Pattern(Pattern::Types::Assign, Declaration(Declaration::DesignEntity::Assignment, "a"), Reference("_"), Expression("_")) });
            expectedResult1.results = std::vector<std::string>();

            Query* result1 = parser1.parse();

            Assert::IsTrue(*result1 == expectedResult1);


            //    a(_, _"x+1"_)
            const std::string query2 = "assign a; Select a pattern a(_, _\"x+1\"_)";
            QueryLexer lexer2 = QueryLexer(query2);
            std::vector<std::string> tokens2 = lexer2.lex();
            QueryParser parser2 = QueryParser(tokens2);

            Query expectedResult2 = Query();

            expectedResult2.declarations = std::vector<Declaration>();
            expectedResult2.declarations.push_back(Declaration(Declaration::DesignEntity::Assignment, "a"));
            expectedResult2.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult2.relations = std::vector<Relation>();
            expectedResult2.patterns = std::vector<Pattern>({ Pattern(Pattern::Types::Assign, Declaration(Declaration::DesignEntity::Assignment, "a"), Reference("_"), Expression("_\"x+1\"_")) });
            expectedResult2.results = std::vector<std::string>();

            Query* result2 = parser2.parse();

            Assert::IsTrue(*result2 == expectedResult2);

            //    a(_, "x+1")
            const std::string query3 = "assign a; Select a pattern a(_, \"x+1\")";
            QueryLexer lexer3 = QueryLexer(query3);
            std::vector<std::string> tokens3 = lexer3.lex();
            QueryParser parser3 = QueryParser(tokens3);

            Query expectedResult3 = Query();

            expectedResult3.declarations = std::vector<Declaration>();
            expectedResult3.declarations.push_back(Declaration(Declaration::DesignEntity::Assignment, "a"));
            expectedResult3.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult2.relations = std::vector<Relation>();
            expectedResult3.patterns = std::vector<Pattern>({ Pattern(Pattern::Types::Assign, Declaration(Declaration::DesignEntity::Assignment, "a"), Reference("_"), Expression("\"x+1\"")) });
            expectedResult3.results = std::vector<std::string>();

            Query* result3 = parser3.parse();

            Assert::IsTrue(*result3 == expectedResult3);

            //    a(v, _)
            const std::string query4 = "assign a; variable v; Select a pattern a(v, _)";
            QueryLexer lexer4 = QueryLexer(query4);
            std::vector<std::string> tokens4 = lexer4.lex();
            QueryParser parser4 = QueryParser(tokens4);

            Query expectedResult4 = Query();

            expectedResult4.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a"), Declaration(Declaration::DesignEntity::Variable, "v") });
            expectedResult4.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult4.relations = std::vector<Relation>();
            expectedResult4.patterns = std::vector<Pattern>({ Pattern(Pattern::Types::Assign, Declaration(Declaration::DesignEntity::Assignment, "a"), Reference(Declaration(Declaration::DesignEntity::Variable, "v")), Expression("_")) });
            expectedResult4.results = std::vector<std::string>();

            Query* result4 = parser4.parse();

            Assert::IsTrue(*result4 == expectedResult4);

            //    a(v, _"x+1"_)
            const std::string query5 = "assign a; variable v; Select a pattern a(v, _\"x+1\"_)";
            QueryLexer lexer5 = QueryLexer(query5);
            std::vector<std::string> tokens5 = lexer5.lex();
            QueryParser parser5 = QueryParser(tokens5);

            Query expectedResult5 = Query();

            expectedResult5.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a"), Declaration(Declaration::DesignEntity::Variable, "v") });
            expectedResult5.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult5.relations = std::vector<Relation>();
            expectedResult5.patterns = std::vector<Pattern>({ Pattern(Pattern::Types::Assign, Declaration(Declaration::DesignEntity::Assignment, "a"), Reference(Declaration(Declaration::DesignEntity::Variable, "v")), Expression("_\"x+1\"_")) });
            expectedResult5.results = std::vector<std::string>();

            Query* result5 = parser5.parse();

            Assert::IsTrue(*result5 == expectedResult5);

            //    a(v, "x+1")
            const std::string query6 = "assign a; variable v; Select a pattern a(v, \"x+1\")";
            QueryLexer lexer6 = QueryLexer(query6);
            std::vector<std::string> tokens6 = lexer6.lex();
            QueryParser parser6 = QueryParser(tokens6);

            Query expectedResult6 = Query();

            expectedResult6.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a"), Declaration(Declaration::DesignEntity::Variable, "v") });
            expectedResult6.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult6.relations = std::vector<Relation>();
            expectedResult6.patterns = std::vector<Pattern>({ Pattern(Pattern::Types::Assign, Declaration(Declaration::DesignEntity::Assignment, "a"), Reference(Declaration(Declaration::DesignEntity::Variable, "v")), Expression("\"x+1\"")) });
            expectedResult6.results = std::vector<std::string>();

            Query* result6 = parser6.parse();

            Assert::IsTrue(*result6 == expectedResult6);

            //    a("x", _)
            const std::string query7 = "assign a; Select a pattern a(\"x\", _)";
            QueryLexer lexer7 = QueryLexer(query7);
            std::vector<std::string> tokens7 = lexer7.lex();
            QueryParser parser7 = QueryParser(tokens7);

            Query expectedResult7 = Query();

            expectedResult7.declarations = std::vector<Declaration>();
            expectedResult7.declarations.push_back(Declaration(Declaration::DesignEntity::Assignment, "a"));
            expectedResult7.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult7.relations = std::vector<Relation>();
            expectedResult7.patterns = std::vector<Pattern>({ Pattern(Pattern::Types::Assign, Declaration(Declaration::DesignEntity::Assignment, "a"), Reference("\"x\""), Expression("_")) });
            expectedResult7.results = std::vector<std::string>();

            Query* result7 = parser7.parse();

            Assert::IsTrue(*result7 == expectedResult7);

            //    a("x", _"x+1"_)
            const std::string query8 = "assign a; Select a pattern a(\"x\", _\"x+1\"_)";
            QueryLexer lexer8 = QueryLexer(query8);
            std::vector<std::string> tokens8 = lexer8.lex();
            QueryParser parser8 = QueryParser(tokens8);

            Query expectedResult8 = Query();

            expectedResult8.declarations = std::vector<Declaration>();
            expectedResult8.declarations.push_back(Declaration(Declaration::DesignEntity::Assignment, "a"));
            expectedResult8.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult8.relations = std::vector<Relation>();
            expectedResult8.patterns = std::vector<Pattern>({ Pattern(Pattern::Types::Assign, Declaration(Declaration::DesignEntity::Assignment, "a"), Reference("\"x\""), Expression("_\"x+1\"_")) });
            expectedResult8.results = std::vector<std::string>();

            Query* result8 = parser8.parse();

            Assert::IsTrue(*result8 == expectedResult8);

            //    a("x", "x+1")
            const std::string query9 = "assign a; Select a pattern a(\"x\", \"x+1\")";
            QueryLexer lexer9 = QueryLexer(query9);
            std::vector<std::string> tokens9 = lexer9.lex();
            QueryParser parser9 = QueryParser(tokens9);

            Query expectedResult9 = Query();

            expectedResult9.declarations = std::vector<Declaration>();
            expectedResult9.declarations.push_back(Declaration(Declaration::DesignEntity::Assignment, "a"));
            expectedResult9.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult9.relations = std::vector<Relation>();
            expectedResult9.patterns = std::vector<Pattern>({ Pattern(Pattern::Types::Assign, Declaration(Declaration::DesignEntity::Assignment, "a"), Reference("\"x\""), Expression("\"x+1\"")) });
            expectedResult9.results = std::vector<std::string>();

            Query* result9 = parser9.parse();

            Assert::IsTrue(*result9 == expectedResult9);

        }

        TEST_METHOD(TestParserValidFollowsQueries) {
            // Follows(synonym, synonym)
            std::string query1 = "assign a1, a2; Select a1 such that Follows(a1, a2)";
            QueryLexer lexer1 = QueryLexer(query1);
            std::vector<std::string> tokens1 = lexer1.lex();
            QueryParser parser1 = QueryParser(tokens1);

            Query expectedResult1 = Query();

            expectedResult1.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a1"),  Declaration(Declaration::DesignEntity::Assignment, "a2") });
            expectedResult1.target = Declaration(Declaration::DesignEntity::Assignment, "a1");
            expectedResult1.relations = std::vector<Relation>({ Relation(Relation::Types::Follows, Reference(Declaration(Declaration::DesignEntity::Assignment, "a1")), Reference(Declaration(Declaration::DesignEntity::Assignment, "a2"))) });
            expectedResult1.patterns = std::vector<Pattern>();
            expectedResult1.results = std::vector<std::string>();

            Query* result1 = parser1.parse();

            Assert::IsTrue(*result1 == expectedResult1);

            // Follows(synonym, _)
            std::string query2 = "assign a; Select a such that Follows(a, _)";
            QueryLexer lexer2 = QueryLexer(query2);
            std::vector<std::string> tokens2 = lexer2.lex();
            QueryParser parser2 = QueryParser(tokens2);

            Query expectedResult2 = Query();

            expectedResult2.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult2.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult2.relations = std::vector<Relation>({ Relation(Relation::Types::Follows, Reference(Declaration(Declaration::DesignEntity::Assignment, "a")), Reference("_")) });
            expectedResult2.patterns = std::vector<Pattern>();
            expectedResult2.results = std::vector<std::string>();

            Query* result2 = parser2.parse();

            Assert::IsTrue(*result2 == expectedResult2);

            // Follows(synonym, INTEGER)
            std::string query3 = "assign a; Select a such that Follows(a, 3)";
            QueryLexer lexer3 = QueryLexer(query3);
            std::vector<std::string> tokens3 = lexer3.lex();
            QueryParser parser3 = QueryParser(tokens3);

            Query expectedResult3 = Query();

            expectedResult3.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult3.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult3.relations = std::vector<Relation>({Relation(Relation::Types::Follows, Reference(Declaration(Declaration::DesignEntity::Assignment, "a")), Reference("3")) });
            expectedResult3.patterns = std::vector<Pattern>();
            expectedResult3.results = std::vector<std::string>();

            Query* result3 = parser3.parse();

            Assert::IsTrue(*result3 == expectedResult3);

            // Follows(_, synonym)
            std::string query4 = "assign a; Select a such that Follows(_, a)";
            QueryLexer lexer4 = QueryLexer(query4);
            std::vector<std::string> tokens4 = lexer4.lex();
            QueryParser parser4 = QueryParser(tokens4);

            Query expectedResult4 = Query();

            expectedResult4.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult4.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult4.relations = std::vector<Relation>({Relation(Relation::Types::Follows, Reference("_"), Reference(Declaration(Declaration::DesignEntity::Assignment, "a"))) });
            expectedResult4.patterns = std::vector<Pattern>();
            expectedResult4.results = std::vector<std::string>();

            Query* result4 = parser4.parse();

            Assert::IsTrue(*result4 == expectedResult4);

            // Follows(_, _)
            std::string query5 = "assign a; Select a such that Follows(_, _)";
            QueryLexer lexer5 = QueryLexer(query5);
            std::vector<std::string> tokens5 = lexer5.lex();
            QueryParser parser5 = QueryParser(tokens5);

            Query expectedResult5 = Query();

            expectedResult5.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult5.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult5.relations = std::vector<Relation>({Relation(Relation::Types::Follows, Reference("_"), Reference("_")) });
            expectedResult5.patterns = std::vector<Pattern>();
            expectedResult5.results = std::vector<std::string>();

            Query* result5 = parser5.parse();

            Assert::IsTrue(*result5 == expectedResult5);

            // Follows(_, INTEGER)
            std::string query6 = "assign a; Select a such that Follows(_, 3)";
            QueryLexer lexer6 = QueryLexer(query6);
            std::vector<std::string> tokens6 = lexer6.lex();
            QueryParser parser6 = QueryParser(tokens6);

            Query expectedResult6 = Query();

            expectedResult6.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult6.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult6.relations = std::vector<Relation>({Relation(Relation::Types::Follows, Reference("_"), Reference("3")) });
            expectedResult6.patterns = std::vector<Pattern>();
            expectedResult6.results = std::vector<std::string>();

            Query* result6 = parser6.parse();

            Assert::IsTrue(*result6 == expectedResult6);

            // Follows(INTEGER, synonym)
            std::string query7 = "assign a; Select a such that Follows(2, a)";
            QueryLexer lexer7 = QueryLexer(query7);
            std::vector<std::string> tokens7 = lexer7.lex();
            QueryParser parser7 = QueryParser(tokens7);

            Query expectedResult7 = Query();

            expectedResult7.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult7.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult7.relations = std::vector<Relation>({Relation(Relation::Types::Follows, Reference("2"), Reference(Declaration(Declaration::DesignEntity::Assignment, "a"))) });
            expectedResult7.patterns = std::vector<Pattern>();
            expectedResult7.results = std::vector<std::string>();

            Query* result7 = parser7.parse();

            Assert::IsTrue(*result7 == expectedResult7);

            // Follows(INTEGER, _)
            std::string query8 = "assign a; Select a such that Follows(2, _)";
            QueryLexer lexer8 = QueryLexer(query8);
            std::vector<std::string> tokens8 = lexer8.lex();
            QueryParser parser8 = QueryParser(tokens8);

            Query expectedResult8 = Query();

            expectedResult8.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult8.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult8.relations = std::vector<Relation>({Relation(Relation::Types::Follows, Reference("2"), Reference("_")) });
            expectedResult8.patterns = std::vector<Pattern>();
            expectedResult8.results = std::vector<std::string>();

            Query* result8 = parser8.parse();

            Assert::IsTrue(*result8 == expectedResult8);

            // Follows(INTEGER, INTEGER)
            std::string query9 = "assign a; Select a such that Follows(2, 3)";
            QueryLexer lexer9 = QueryLexer(query9);
            std::vector<std::string> tokens9 = lexer9.lex();
            QueryParser parser9 = QueryParser(tokens9);

            Query expectedResult9 = Query();

            expectedResult9.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult9.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult9.relations = std::vector<Relation>({ Relation(Relation::Types::Follows, Reference("2"), Reference("3")) });
            expectedResult9.patterns = std::vector<Pattern>();
            expectedResult9.results = std::vector<std::string>();

            Query* result9 = parser9.parse();

            Assert::IsTrue(*result9 == expectedResult9);

        }

        TEST_METHOD(TestParserValidFollowsTQueries) {
            // Follows*(synonym, synonym)
            std::string query1 = "assign a1, a2; Select a1 such that Follows*(a1, a2)";
            QueryLexer lexer1 = QueryLexer(query1);
            std::vector<std::string> tokens1 = lexer1.lex();
            QueryParser parser1 = QueryParser(tokens1);

            Query expectedResult1 = Query();

            expectedResult1.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a1"),  Declaration(Declaration::DesignEntity::Assignment, "a2") });
            expectedResult1.target = Declaration(Declaration::DesignEntity::Assignment, "a1");
            expectedResult1.relations = std::vector<Relation>({Relation(Relation::Types::FollowsT, Reference(Declaration(Declaration::DesignEntity::Assignment, "a1")), Reference(Declaration(Declaration::DesignEntity::Assignment, "a2"))) });
            expectedResult1.patterns = std::vector<Pattern>();
            expectedResult1.results = std::vector<std::string>();

            Query* result1 = parser1.parse();

            Assert::IsTrue(*result1 == expectedResult1);

            // Follows*(synonym, _)
            std::string query2 = "assign a; Select a such that Follows*(a, _)";
            QueryLexer lexer2 = QueryLexer(query2);
            std::vector<std::string> tokens2 = lexer2.lex();
            QueryParser parser2 = QueryParser(tokens2);

            Query expectedResult2 = Query();

            expectedResult2.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult2.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult2.relations = std::vector<Relation>({ Relation(Relation::Types::FollowsT, Reference(Declaration(Declaration::DesignEntity::Assignment, "a")), Reference("_")) });
            expectedResult2.patterns = std::vector<Pattern>();
            expectedResult2.results = std::vector<std::string>();

            Query* result2 = parser2.parse();

            Assert::IsTrue(*result2 == expectedResult2);

            // Follows*(synonym, INTEGER)
            std::string query3 = "assign a; Select a such that Follows*(a, 3)";
            QueryLexer lexer3 = QueryLexer(query3);
            std::vector<std::string> tokens3 = lexer3.lex();
            QueryParser parser3 = QueryParser(tokens3);

            Query expectedResult3 = Query();

            expectedResult3.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult3.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult3.relations = std::vector<Relation>({ Relation(Relation::Types::FollowsT, Reference(Declaration(Declaration::DesignEntity::Assignment, "a")), Reference("3")) });
            expectedResult3.patterns = std::vector<Pattern>();
            expectedResult3.results = std::vector<std::string>();

            Query* result3 = parser3.parse();

            Assert::IsTrue(*result3 == expectedResult3);

            // Follows*(_, synonym)
            std::string query4 = "assign a; Select a such that Follows*(_, a)";
            QueryLexer lexer4 = QueryLexer(query4);
            std::vector<std::string> tokens4 = lexer4.lex();
            QueryParser parser4 = QueryParser(tokens4);

            Query expectedResult4 = Query();

            expectedResult4.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult4.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult4.relations = std::vector<Relation>({ Relation(Relation::Types::FollowsT, Reference("_"), Reference(Declaration(Declaration::DesignEntity::Assignment, "a"))) });
            expectedResult4.patterns = std::vector<Pattern>();
            expectedResult4.results = std::vector<std::string>();

            Query* result4 = parser4.parse();

            Assert::IsTrue(*result4 == expectedResult4);

            // Follows*(_, _)
            std::string query5 = "assign a; Select a such that Follows*(_, _)";
            QueryLexer lexer5 = QueryLexer(query5);
            std::vector<std::string> tokens5 = lexer5.lex();
            QueryParser parser5 = QueryParser(tokens5);

            Query expectedResult5 = Query();

            expectedResult5.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult5.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult5.relations = std::vector<Relation>({ Relation(Relation::Types::FollowsT, Reference("_"), Reference("_")) });
            expectedResult5.patterns = std::vector<Pattern>();
            expectedResult5.results = std::vector<std::string>();

            Query* result5 = parser5.parse();

            Assert::IsTrue(*result5 == expectedResult5);

            // Follows*(_, INTEGER)
            std::string query6 = "assign a; Select a such that Follows*(_, 3)";
            QueryLexer lexer6 = QueryLexer(query6);
            std::vector<std::string> tokens6 = lexer6.lex();
            QueryParser parser6 = QueryParser(tokens6);

            Query expectedResult6 = Query();

            expectedResult6.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult6.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult6.relations = std::vector<Relation>({ Relation(Relation::Types::FollowsT, Reference("_"), Reference("3")) });
            expectedResult6.patterns = std::vector<Pattern>();
            expectedResult6.results = std::vector<std::string>();

            Query* result6 = parser6.parse();

            Assert::IsTrue(*result6 == expectedResult6);

            // Follows*(INTEGER, synonym)
            std::string query7 = "assign a; Select a such that Follows*(2, a)";
            QueryLexer lexer7 = QueryLexer(query7);
            std::vector<std::string> tokens7 = lexer7.lex();
            QueryParser parser7 = QueryParser(tokens7);

            Query expectedResult7 = Query();

            expectedResult7.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult7.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult7.relations = std::vector<Relation>({ Relation(Relation::Types::FollowsT, Reference("2"), Reference(Declaration(Declaration::DesignEntity::Assignment, "a"))) });
            expectedResult7.patterns = std::vector<Pattern>();
            expectedResult7.results = std::vector<std::string>();

            Query* result7 = parser7.parse();

            Assert::IsTrue(*result7 == expectedResult7);

            // Follows*(INTEGER, _)
            std::string query8 = "assign a; Select a such that Follows*(2, _)";
            QueryLexer lexer8 = QueryLexer(query8);
            std::vector<std::string> tokens8 = lexer8.lex();
            QueryParser parser8 = QueryParser(tokens8);

            Query expectedResult8 = Query();

            expectedResult8.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult8.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult8.relations = std::vector<Relation>({ Relation(Relation::Types::FollowsT, Reference("2"), Reference("_")) });
            expectedResult8.patterns = std::vector<Pattern>();
            expectedResult8.results = std::vector<std::string>();

            Query* result8 = parser8.parse();

            Assert::IsTrue(*result8 == expectedResult8);

            // Follows*(INTEGER, INTEGER)
            std::string query9 = "assign a; Select a such that Follows*(2, 3)";
            QueryLexer lexer9 = QueryLexer(query9);
            std::vector<std::string> tokens9 = lexer9.lex();
            QueryParser parser9 = QueryParser(tokens9);

            Query expectedResult9 = Query();

            expectedResult9.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult9.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult9.relations = std::vector<Relation>({ Relation(Relation::Types::FollowsT, Reference("2"), Reference("3")) });
            expectedResult9.patterns = std::vector<Pattern>();
            expectedResult9.results = std::vector<std::string>();

            Query* result9 = parser9.parse();

            Assert::IsTrue(*result9 == expectedResult9);
        }

        TEST_METHOD(TestParserValidUsesSQueries) {
            std::string query = "print a; variable v; Select a such that Uses(a, v)";
            QueryLexer lexer = QueryLexer(query);
            std::vector<std::string> tokens = lexer.lex();
            QueryParser parser = QueryParser(tokens);

            Query expectedResult = Query();

            expectedResult.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Print, "a"),
                                                                     Declaration(Declaration::DesignEntity::Variable, "v") });
            expectedResult.target = Declaration(Declaration::DesignEntity::Print, "a");
            expectedResult.relations = std::vector<Relation>({Relation(Relation::Types::UsesS, Reference(Declaration(Declaration::DesignEntity::Print, "a")),
                                                Reference(Declaration(Declaration::DesignEntity::Variable, "v"))) });
            expectedResult.patterns = std::vector<Pattern>();
            expectedResult.results = std::vector<std::string>();

            Query* result = parser.parse();

            Assert::IsTrue(*result == expectedResult);

            // UsesS(synonym, synonym)
            std::string query1 = "stmt s; variable v; Select s such that Uses(s, v)";
            QueryLexer lexer1 = QueryLexer(query1);
            std::vector<std::string> tokens1 = lexer1.lex();
            QueryParser parser1 = QueryParser(tokens1);

            Query expectedResult1 = Query();

            expectedResult1.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Statement, "s"),  Declaration(Declaration::DesignEntity::Variable, "v") });
            expectedResult1.target = Declaration(Declaration::DesignEntity::Statement, "s");
            expectedResult1.relations = std::vector<Relation>({ Relation(Relation::Types::UsesS, Reference(Declaration(Declaration::DesignEntity::Statement, "s")), Reference(Declaration(Declaration::DesignEntity::Variable, "v"))) });
            expectedResult1.patterns = std::vector<Pattern>();
            expectedResult1.results = std::vector<std::string>();

            Query* result1 = parser1.parse();

            Assert::IsTrue(*result1 == expectedResult1);

            // UsesS(synonym, _)
            std::string query2 = "assign a; Select a such that Uses(a, _)";
            QueryLexer lexer2 = QueryLexer(query2);
            std::vector<std::string> tokens2 = lexer2.lex();
            QueryParser parser2 = QueryParser(tokens2);

            Query expectedResult2 = Query();

            expectedResult2.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult2.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult2.relations = std::vector<Relation>({ Relation(Relation::Types::UsesS, Reference(Declaration(Declaration::DesignEntity::Assignment, "a")), Reference("_")) });
            expectedResult2.patterns = std::vector<Pattern>();
            expectedResult2.results = std::vector<std::string>();

            Query* result2 = parser2.parse();

            Assert::IsTrue(*result2 == expectedResult2);

            // UsesS(synonym, "IDENT")
            std::string query3 = "assign a; Select a such that Uses(a, \"x\")";
            QueryLexer lexer3 = QueryLexer(query3);
            std::vector<std::string> tokens3 = lexer3.lex();
            QueryParser parser3 = QueryParser(tokens3);

            Query expectedResult3 = Query();

            expectedResult3.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult3.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult3.relations = std::vector<Relation>({ Relation(Relation::Types::UsesS, Reference(Declaration(Declaration::DesignEntity::Assignment, "a")), Reference("\"x\"")) });
            expectedResult3.patterns = std::vector<Pattern>();
            expectedResult3.results = std::vector<std::string>();

            Query* result3 = parser3.parse();

            Assert::IsTrue(*result3 == expectedResult3);

            // UsesS(INTEGER, synonym)
            std::string query7 = "variable v; Select v such that Uses(2, v)";
            QueryLexer lexer7 = QueryLexer(query7);
            std::vector<std::string> tokens7 = lexer7.lex();
            QueryParser parser7 = QueryParser(tokens7);

            Query expectedResult7 = Query();

            expectedResult7.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Variable, "v") });
            expectedResult7.target = Declaration(Declaration::DesignEntity::Variable, "v");
            expectedResult7.relations = std::vector<Relation>({ Relation(Relation::Types::UsesS, Reference("2"), Reference(Declaration(Declaration::DesignEntity::Variable, "v"))) });
            expectedResult7.patterns = std::vector<Pattern>();
            expectedResult7.results = std::vector<std::string>();

            Query* result7 = parser7.parse();

            Assert::IsTrue(*result7 == expectedResult7);

            // UsesS(INTEGER, _)
            std::string query8 = "assign a; Select a such that Uses(2, _)";
            QueryLexer lexer8 = QueryLexer(query8);
            std::vector<std::string> tokens8 = lexer8.lex();
            QueryParser parser8 = QueryParser(tokens8);

            Query expectedResult8 = Query();

            expectedResult8.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult8.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult8.relations = std::vector<Relation>({ Relation(Relation::Types::UsesS, Reference("2"), Reference("_")) });
            expectedResult8.patterns = std::vector<Pattern>();
            expectedResult8.results = std::vector<std::string>();

            Query* result8 = parser8.parse();

            Assert::IsTrue(*result8 == expectedResult8);

            // UsesS(INTEGER, "IDENT")
            std::string query9 = "assign a; Select a such that Uses(2, \"x\")";
            QueryLexer lexer9 = QueryLexer(query9);
            std::vector<std::string> tokens9 = lexer9.lex();
            QueryParser parser9 = QueryParser(tokens9);

            Query expectedResult9 = Query();

            expectedResult9.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult9.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult9.relations = std::vector<Relation>({ Relation(Relation::Types::UsesS, Reference("2"), Reference("\"x\"")) });
            expectedResult9.patterns = std::vector<Pattern>();
            expectedResult9.results = std::vector<std::string>();

            Query* result9 = parser9.parse();

            Assert::IsTrue(*result9 == expectedResult9);
        }

        TEST_METHOD(TestParserValidUsesPQueries) {
            // UsesP(synonym, synonym)
            std::string query1 = "procedure p; variable v; Select p such that Uses(p, v)";
            QueryLexer lexer1 = QueryLexer(query1);
            std::vector<std::string> tokens1 = lexer1.lex();
            QueryParser parser1 = QueryParser(tokens1);

            Query expectedResult1 = Query();

            expectedResult1.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p"),  Declaration(Declaration::DesignEntity::Variable, "v") });
            expectedResult1.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult1.relations = std::vector<Relation>({Relation(Relation::Types::UsesP, Reference(Declaration(Declaration::DesignEntity::Procedure, "p")), Reference(Declaration(Declaration::DesignEntity::Variable, "v"))) });
            expectedResult1.patterns = std::vector<Pattern>();
            expectedResult1.results = std::vector<std::string>();

            Query* result1 = parser1.parse();

            Assert::IsTrue(*result1 == expectedResult1);

            // UsesP(synonym, _)
            std::string query2 = "procedure p; Select p such that Uses(p, _)";
            QueryLexer lexer2 = QueryLexer(query2);
            std::vector<std::string> tokens2 = lexer2.lex();
            QueryParser parser2 = QueryParser(tokens2);

            Query expectedResult2 = Query();

            expectedResult2.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult2.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult2.relations = std::vector<Relation>({ Relation(Relation::Types::UsesP, Reference(Declaration(Declaration::DesignEntity::Procedure, "p")), Reference("_")) });
            expectedResult2.patterns = std::vector<Pattern>();
            expectedResult2.results = std::vector<std::string>();

            Query* result2 = parser2.parse();

            Assert::IsTrue(*result2 == expectedResult2);

            // UsesP(synonym, "IDENT")
            std::string query3 = "procedure p; Select p such that Uses(p, \"x\")";
            QueryLexer lexer3 = QueryLexer(query3);
            std::vector<std::string> tokens3 = lexer3.lex();
            QueryParser parser3 = QueryParser(tokens3);

            Query expectedResult3 = Query();

            expectedResult3.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult3.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult3.relations = std::vector<Relation>({ Relation(Relation::Types::UsesP, Reference(Declaration(Declaration::DesignEntity::Procedure, "p")), Reference("\"x\"")) });
            expectedResult3.patterns = std::vector<Pattern>();
            expectedResult3.results = std::vector<std::string>();

            Query* result3 = parser3.parse();

            Assert::IsTrue(*result3 == expectedResult3);

            // UsesP("IDENT", synonym)
            std::string query7 = "variable v; Select v such that Uses(\"p\", v)";
            QueryLexer lexer7 = QueryLexer(query7);
            std::vector<std::string> tokens7 = lexer7.lex();
            QueryParser parser7 = QueryParser(tokens7);

            Query expectedResult7 = Query();

            expectedResult7.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Variable, "v") });
            expectedResult7.target = Declaration(Declaration::DesignEntity::Variable, "v");
            expectedResult7.relations = std::vector<Relation>({ Relation(Relation::Types::UsesP, Reference("\"p\""), Reference(Declaration(Declaration::DesignEntity::Variable, "v"))) });
            expectedResult7.patterns = std::vector<Pattern>();
            expectedResult7.results = std::vector<std::string>();

            Query* result7 = parser7.parse();

            Assert::IsTrue(*result7 == expectedResult7);

            // UsesP("IDENT", _)
            std::string query8 = "assign a; Select a such that Uses(\"p\", _)";
            QueryLexer lexer8 = QueryLexer(query8);
            std::vector<std::string> tokens8 = lexer8.lex();
            QueryParser parser8 = QueryParser(tokens8);

            Query expectedResult8 = Query();

            expectedResult8.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult8.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult8.relations = std::vector<Relation>({ Relation(Relation::Types::UsesP, Reference("\"p\""), Reference("_")) });
            expectedResult8.patterns = std::vector<Pattern>();
            expectedResult8.results = std::vector<std::string>();

            Query* result8 = parser8.parse();

            Assert::IsTrue(*result8 == expectedResult8);

            // UsesP("IDENT", "IDENT")
            std::string query9 = "assign a; Select a such that Uses(\"p\", \"x\")";
            QueryLexer lexer9 = QueryLexer(query9);
            std::vector<std::string> tokens9 = lexer9.lex();
            QueryParser parser9 = QueryParser(tokens9);

            Query expectedResult9 = Query();

            expectedResult9.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult9.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult9.relations = std::vector<Relation>({ Relation(Relation::Types::UsesP, Reference("\"p\""), Reference("\"x\"")) });
            expectedResult9.patterns = std::vector<Pattern>();
            expectedResult9.results = std::vector<std::string>();

            Query* result9 = parser9.parse();

            Assert::IsTrue(*result9 == expectedResult9);
        }

        TEST_METHOD(TestParserValidModifiesSQueries) {
            std::string query = "read a; Select a such that Modifies(a, _)";
            QueryLexer lexer = QueryLexer(query);
            std::vector<std::string> tokens = lexer.lex();
            QueryParser parser = QueryParser(tokens);

            Query expectedResult = Query();

            expectedResult.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Read, "a") });
            expectedResult.target = Declaration(Declaration::DesignEntity::Read, "a");
            expectedResult.relations = std::vector<Relation>({Relation(Relation::Types::ModifiesS, Reference(Declaration(Declaration::DesignEntity::Read, "a")), Reference("_")) });
            expectedResult.patterns = std::vector<Pattern>();
            expectedResult.results = std::vector<std::string>();

            Query* result = parser.parse();

            Assert::IsTrue(*result == expectedResult);

            // ModifiesS(synonym, synonym)
            std::string query1 = "stmt s; variable v; Select s such that Modifies(s, v)";
            QueryLexer lexer1 = QueryLexer(query1);
            std::vector<std::string> tokens1 = lexer1.lex();
            QueryParser parser1 = QueryParser(tokens1);

            Query expectedResult1 = Query();

            expectedResult1.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Statement, "s"),  Declaration(Declaration::DesignEntity::Variable, "v") });
            expectedResult1.target = Declaration(Declaration::DesignEntity::Statement, "s");
            expectedResult1.relations = std::vector<Relation>({ Relation(Relation::Types::ModifiesS, Reference(Declaration(Declaration::DesignEntity::Statement, "s")), Reference(Declaration(Declaration::DesignEntity::Variable, "v"))) });
            expectedResult1.patterns = std::vector<Pattern>();
            expectedResult1.results = std::vector<std::string>();

            Query* result1 = parser1.parse();

            Assert::IsTrue(*result1 == expectedResult1);

            // ModifiesS(synonym, _)
            std::string query2 = "assign a; Select a such that Modifies(a, _)";
            QueryLexer lexer2 = QueryLexer(query2);
            std::vector<std::string> tokens2 = lexer2.lex();
            QueryParser parser2 = QueryParser(tokens2);

            Query expectedResult2 = Query();

            expectedResult2.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult2.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult2.relations = std::vector<Relation>({ Relation(Relation::Types::ModifiesS, Reference(Declaration(Declaration::DesignEntity::Assignment, "a")), Reference("_")) });
            expectedResult2.patterns = std::vector<Pattern>();
            expectedResult2.results = std::vector<std::string>();

            Query* result2 = parser2.parse();

            Assert::IsTrue(*result2 == expectedResult2);

            // ModifiesS(synonym, "IDENT")
            std::string query3 = "assign a; Select a such that Modifies(a, \"x\")";
            QueryLexer lexer3 = QueryLexer(query3);
            std::vector<std::string> tokens3 = lexer3.lex();
            QueryParser parser3 = QueryParser(tokens3);

            Query expectedResult3 = Query();

            expectedResult3.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult3.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult3.relations = std::vector<Relation>({ Relation(Relation::Types::ModifiesS, Reference(Declaration(Declaration::DesignEntity::Assignment, "a")), Reference("\"x\"")) });
            expectedResult3.patterns = std::vector<Pattern>();
            expectedResult3.results = std::vector<std::string>();

            Query* result3 = parser3.parse();

            Assert::IsTrue(*result3 == expectedResult3);

            // ModifiesS(INTEGER, synonym)
            std::string query7 = "variable v; Select v such that Modifies(2, v)";
            QueryLexer lexer7 = QueryLexer(query7);
            std::vector<std::string> tokens7 = lexer7.lex();
            QueryParser parser7 = QueryParser(tokens7);

            Query expectedResult7 = Query();

            expectedResult7.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Variable, "v") });
            expectedResult7.target = Declaration(Declaration::DesignEntity::Variable, "v");
            expectedResult7.relations = std::vector<Relation>({ Relation(Relation::Types::ModifiesS, Reference("2"), Reference(Declaration(Declaration::DesignEntity::Variable, "v"))) });
            expectedResult7.patterns = std::vector<Pattern>();
            expectedResult7.results = std::vector<std::string>();

            Query* result7 = parser7.parse();

            Assert::IsTrue(*result7 == expectedResult7);

            // ModifiesS(INTEGER, _)
            std::string query8 = "assign a; Select a such that Modifies(2, _)";
            QueryLexer lexer8 = QueryLexer(query8);
            std::vector<std::string> tokens8 = lexer8.lex();
            QueryParser parser8 = QueryParser(tokens8);

            Query expectedResult8 = Query();

            expectedResult8.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult8.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult8.relations = std::vector<Relation>({ Relation(Relation::Types::ModifiesS, Reference("2"), Reference("_")) });
            expectedResult8.patterns = std::vector<Pattern>();
            expectedResult8.results = std::vector<std::string>();

            Query* result8 = parser8.parse();

            Assert::IsTrue(*result8 == expectedResult8);

            // ModifiesS(INTEGER, "IDENT")
            std::string query9 = "assign a; Select a such that Modifies(2, \"x\")";
            QueryLexer lexer9 = QueryLexer(query9);
            std::vector<std::string> tokens9 = lexer9.lex();
            QueryParser parser9 = QueryParser(tokens9);

            Query expectedResult9 = Query();

            expectedResult9.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult9.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult9.relations = std::vector<Relation>({ Relation(Relation::Types::ModifiesS, Reference("2"), Reference("\"x\"")) });
            expectedResult9.patterns = std::vector<Pattern>();
            expectedResult9.results = std::vector<std::string>();

            Query* result9 = parser9.parse();

            Assert::IsTrue(*result9 == expectedResult9);
        }

        TEST_METHOD(TestParserValidModifiesPQueries) {
            // ModifiesP(synonym, synonym)
            std::string query1 = "procedure p; variable v; Select p such that Modifies(p, v)";
            QueryLexer lexer1 = QueryLexer(query1);
            std::vector<std::string> tokens1 = lexer1.lex();
            QueryParser parser1 = QueryParser(tokens1);

            Query expectedResult1 = Query();

            expectedResult1.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p"),  Declaration(Declaration::DesignEntity::Variable, "v") });
            expectedResult1.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult1.relations = std::vector<Relation>({Relation(Relation::Types::ModifiesP, Reference(Declaration(Declaration::DesignEntity::Procedure, "p")), Reference(Declaration(Declaration::DesignEntity::Variable, "v"))) });
            expectedResult1.patterns = std::vector<Pattern>();
            expectedResult1.results = std::vector<std::string>();

            Query* result1 = parser1.parse();

            Assert::IsTrue(*result1 == expectedResult1);

            // ModifiesP(synonym, _)
            std::string query2 = "procedure p; Select p such that Modifies(p, _)";
            QueryLexer lexer2 = QueryLexer(query2);
            std::vector<std::string> tokens2 = lexer2.lex();
            QueryParser parser2 = QueryParser(tokens2);

            Query expectedResult2 = Query();

            expectedResult2.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult2.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult2.relations = std::vector<Relation>({Relation(Relation::Types::ModifiesP, Reference(Declaration(Declaration::DesignEntity::Procedure, "p")), Reference("_")) });
            expectedResult2.patterns = std::vector<Pattern>();
            expectedResult2.results = std::vector<std::string>();

            Query* result2 = parser2.parse();

            Assert::IsTrue(*result2 == expectedResult2);

            // ModifiesP(synonym, "IDENT")
            std::string query3 = "procedure p; Select p such that Modifies(p, \"x\")";
            QueryLexer lexer3 = QueryLexer(query3);
            std::vector<std::string> tokens3 = lexer3.lex();
            QueryParser parser3 = QueryParser(tokens3);

            Query expectedResult3 = Query();

            expectedResult3.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult3.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult3.relations = std::vector<Relation>({Relation(Relation::Types::ModifiesP, Reference(Declaration(Declaration::DesignEntity::Procedure, "p")), Reference("\"x\"")) });
            expectedResult3.patterns = std::vector<Pattern>();
            expectedResult3.results = std::vector<std::string>();

            Query* result3 = parser3.parse();

            Assert::IsTrue(*result3 == expectedResult3);

            // ModifiesP("IDENT", synonym)
            std::string query7 = "variable v; Select v such that Modifies(\"p\", v)";
            QueryLexer lexer7 = QueryLexer(query7);
            std::vector<std::string> tokens7 = lexer7.lex();
            QueryParser parser7 = QueryParser(tokens7);

            Query expectedResult7 = Query();

            expectedResult7.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Variable, "v") });
            expectedResult7.target = Declaration(Declaration::DesignEntity::Variable, "v");
            expectedResult7.relations = std::vector<Relation>({Relation(Relation::Types::ModifiesP, Reference("\"p\""), Reference(Declaration(Declaration::DesignEntity::Variable, "v"))) });
            expectedResult7.patterns = std::vector<Pattern>();
            expectedResult7.results = std::vector<std::string>();

            Query* result7 = parser7.parse();

            Assert::IsTrue(*result7 == expectedResult7);

            // ModifiesP("IDENT", _)
            std::string query8 = "assign a; Select a such that Modifies(\"p\", _)";
            QueryLexer lexer8 = QueryLexer(query8);
            std::vector<std::string> tokens8 = lexer8.lex();
            QueryParser parser8 = QueryParser(tokens8);

            Query expectedResult8 = Query();

            expectedResult8.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult8.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult8.relations = std::vector<Relation>({Relation(Relation::Types::ModifiesP, Reference("\"p\""), Reference("_")) });
            expectedResult8.patterns = std::vector<Pattern>();
            expectedResult8.results = std::vector<std::string>();

            Query* result8 = parser8.parse();

            Assert::IsTrue(*result8 == expectedResult8);

            // ModifiesP("IDENT", "IDENT")
            std::string query9 = "assign a; Select a such that Modifies(\"p\", \"x\")";
            QueryLexer lexer9 = QueryLexer(query9);
            std::vector<std::string> tokens9 = lexer9.lex();
            QueryParser parser9 = QueryParser(tokens9);

            Query expectedResult9 = Query();

            expectedResult9.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult9.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult9.relations = std::vector<Relation>({Relation(Relation::Types::ModifiesP, Reference("\"p\""), Reference("\"x\"")) });
            expectedResult9.patterns = std::vector<Pattern>();
            expectedResult9.results = std::vector<std::string>();

            Query* result9 = parser9.parse();

            Assert::IsTrue(*result9 == expectedResult9);
        }

        TEST_METHOD(TestParserValidParentQueries) {
            // Parent(synonym, synonym)
            std::string query1 = "assign a1, a2; Select a1 such that Parent(a1, a2)";
            QueryLexer lexer1 = QueryLexer(query1);
            std::vector<std::string> tokens1 = lexer1.lex();
            QueryParser parser1 = QueryParser(tokens1);

            Query expectedResult1 = Query();

            expectedResult1.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a1"),  Declaration(Declaration::DesignEntity::Assignment, "a2") });
            expectedResult1.target = Declaration(Declaration::DesignEntity::Assignment, "a1");
            expectedResult1.relations = std::vector<Relation>({Relation(Relation::Types::Parent, Reference(Declaration(Declaration::DesignEntity::Assignment, "a1")), Reference(Declaration(Declaration::DesignEntity::Assignment, "a2"))) });
            expectedResult1.patterns = std::vector<Pattern>();
            expectedResult1.results = std::vector<std::string>();

            Query* result1 = parser1.parse();

            Assert::IsTrue(*result1 == expectedResult1);

            // Parent(synonym, _)
            std::string query2 = "assign a; Select a such that Parent(a, _)";
            QueryLexer lexer2 = QueryLexer(query2);
            std::vector<std::string> tokens2 = lexer2.lex();
            QueryParser parser2 = QueryParser(tokens2);

            Query expectedResult2 = Query();

            expectedResult2.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult2.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult2.relations = std::vector<Relation>({Relation(Relation::Types::Parent, Reference(Declaration(Declaration::DesignEntity::Assignment, "a")), Reference("_")) });
            expectedResult2.patterns = std::vector<Pattern>();
            expectedResult2.results = std::vector<std::string>();

            Query* result2 = parser2.parse();

            Assert::IsTrue(*result2 == expectedResult2);

            // Parent(synonym, INTEGER)
            std::string query3 = "assign a; Select a such that Parent(a, 3)";
            QueryLexer lexer3 = QueryLexer(query3);
            std::vector<std::string> tokens3 = lexer3.lex();
            QueryParser parser3 = QueryParser(tokens3);

            Query expectedResult3 = Query();

            expectedResult3.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult3.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult3.relations = std::vector<Relation>({Relation(Relation::Types::Parent, Reference(Declaration(Declaration::DesignEntity::Assignment, "a")), Reference("3")) });
            expectedResult3.patterns = std::vector<Pattern>();
            expectedResult3.results = std::vector<std::string>();

            Query* result3 = parser3.parse();

            Assert::IsTrue(*result3 == expectedResult3);

            // Parent(_, synonym)
            std::string query4 = "assign a; Select a such that Parent(_, a)";
            QueryLexer lexer4 = QueryLexer(query4);
            std::vector<std::string> tokens4 = lexer4.lex();
            QueryParser parser4 = QueryParser(tokens4);

            Query expectedResult4 = Query();

            expectedResult4.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult4.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult4.relations = std::vector<Relation>({Relation(Relation::Types::Parent, Reference("_"), Reference(Declaration(Declaration::DesignEntity::Assignment, "a"))) });
            expectedResult4.patterns = std::vector<Pattern>();
            expectedResult4.results = std::vector<std::string>();

            Query* result4 = parser4.parse();

            Assert::IsTrue(*result4 == expectedResult4);

            // Parent(_, _)
            std::string query5 = "assign a; Select a such that Parent(_, _)";
            QueryLexer lexer5 = QueryLexer(query5);
            std::vector<std::string> tokens5 = lexer5.lex();
            QueryParser parser5 = QueryParser(tokens5);

            Query expectedResult5 = Query();

            expectedResult5.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult5.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult5.relations = std::vector<Relation>({Relation(Relation::Types::Parent, Reference("_"), Reference("_")) });
            expectedResult5.patterns = std::vector<Pattern>();
            expectedResult5.results = std::vector<std::string>();

            Query* result5 = parser5.parse();

            Assert::IsTrue(*result5 == expectedResult5);

            // Parent(_, INTEGER)
            std::string query6 = "assign a; Select a such that Parent(_, 3)";
            QueryLexer lexer6 = QueryLexer(query6);
            std::vector<std::string> tokens6 = lexer6.lex();
            QueryParser parser6 = QueryParser(tokens6);

            Query expectedResult6 = Query();

            expectedResult6.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult6.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult6.relations = std::vector<Relation>({Relation(Relation::Types::Parent, Reference("_"), Reference("3")) });
            expectedResult6.patterns = std::vector<Pattern>();
            expectedResult6.results = std::vector<std::string>();

            Query* result6 = parser6.parse();

            Assert::IsTrue(*result6 == expectedResult6);

            // Parent(INTEGER, synonym)
            std::string query7 = "assign a; Select a such that Parent(2, a)";
            QueryLexer lexer7 = QueryLexer(query7);
            std::vector<std::string> tokens7 = lexer7.lex();
            QueryParser parser7 = QueryParser(tokens7);

            Query expectedResult7 = Query();

            expectedResult7.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult7.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult7.relations = std::vector<Relation>({Relation(Relation::Types::Parent, Reference("2"), Reference(Declaration(Declaration::DesignEntity::Assignment, "a"))) });
            expectedResult7.patterns = std::vector<Pattern>();
            expectedResult7.results = std::vector<std::string>();

            Query* result7 = parser7.parse();

            Assert::IsTrue(*result7 == expectedResult7);

            // Parent(INTEGER, _)
            std::string query8 = "assign a; Select a such that Parent(2, _)";
            QueryLexer lexer8 = QueryLexer(query8);
            std::vector<std::string> tokens8 = lexer8.lex();
            QueryParser parser8 = QueryParser(tokens8);

            Query expectedResult8 = Query();

            expectedResult8.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult8.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult8.relations = std::vector<Relation>({Relation(Relation::Types::Parent, Reference("2"), Reference("_")) });
            expectedResult8.patterns = std::vector<Pattern>();
            expectedResult8.results = std::vector<std::string>();

            Query* result8 = parser8.parse();

            Assert::IsTrue(*result8 == expectedResult8);

            // Parent(INTEGER, INTEGER)
            std::string query9 = "assign a; Select a such that Parent(2, 3)";
            QueryLexer lexer9 = QueryLexer(query9);
            std::vector<std::string> tokens9 = lexer9.lex();
            QueryParser parser9 = QueryParser(tokens9);

            Query expectedResult9 = Query();

            expectedResult9.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult9.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult9.relations = std::vector<Relation>({Relation(Relation::Types::Parent, Reference("2"), Reference("3")) });
            expectedResult9.patterns = std::vector<Pattern>();
            expectedResult9.results = std::vector<std::string>();

            Query* result9 = parser9.parse();

            Assert::IsTrue(*result9 == expectedResult9);
        }

        TEST_METHOD(TestParserValidParentTQueries) {
            // Parent*(synonym, synonym)
            std::string query1 = "assign a1, a2; Select a1 such that Parent*(a1, a2)";
            QueryLexer lexer1 = QueryLexer(query1);
            std::vector<std::string> tokens1 = lexer1.lex();
            QueryParser parser1 = QueryParser(tokens1);

            Query expectedResult1 = Query();

            expectedResult1.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a1"),  Declaration(Declaration::DesignEntity::Assignment, "a2") });
            expectedResult1.target = Declaration(Declaration::DesignEntity::Assignment, "a1");
            expectedResult1.relations = std::vector<Relation>({Relation(Relation::Types::ParentT, Reference(Declaration(Declaration::DesignEntity::Assignment, "a1")), Reference(Declaration(Declaration::DesignEntity::Assignment, "a2"))) });
            expectedResult1.patterns = std::vector<Pattern>();
            expectedResult1.results = std::vector<std::string>();

            Query* result1 = parser1.parse();

            Assert::IsTrue(*result1 == expectedResult1);

            // Parent*(synonym, _)
            std::string query2 = "assign a; Select a such that Parent*(a, _)";
            QueryLexer lexer2 = QueryLexer(query2);
            std::vector<std::string> tokens2 = lexer2.lex();
            QueryParser parser2 = QueryParser(tokens2);

            Query expectedResult2 = Query();

            expectedResult2.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult2.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult2.relations = std::vector<Relation>({Relation(Relation::Types::ParentT, Reference(Declaration(Declaration::DesignEntity::Assignment, "a")), Reference("_")) });
            expectedResult2.patterns = std::vector<Pattern>();
            expectedResult2.results = std::vector<std::string>();

            Query* result2 = parser2.parse();

            Assert::IsTrue(*result2 == expectedResult2);

            // Parent*(synonym, INTEGER)
            std::string query3 = "assign a; Select a such that Parent*(a, 3)";
            QueryLexer lexer3 = QueryLexer(query3);
            std::vector<std::string> tokens3 = lexer3.lex();
            QueryParser parser3 = QueryParser(tokens3);

            Query expectedResult3 = Query();

            expectedResult3.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult3.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult3.relations = std::vector<Relation>({Relation(Relation::Types::ParentT, Reference(Declaration(Declaration::DesignEntity::Assignment, "a")), Reference("3")) });
            expectedResult3.patterns = std::vector<Pattern>();
            expectedResult3.results = std::vector<std::string>();

            Query* result3 = parser3.parse();

            Assert::IsTrue(*result3 == expectedResult3);

            // Parent*(_, synonym)
            std::string query4 = "assign a; Select a such that Parent*(_, a)";
            QueryLexer lexer4 = QueryLexer(query4);
            std::vector<std::string> tokens4 = lexer4.lex();
            QueryParser parser4 = QueryParser(tokens4);

            Query expectedResult4 = Query();

            expectedResult4.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult4.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult4.relations = std::vector<Relation>({Relation(Relation::Types::ParentT, Reference("_"), Reference(Declaration(Declaration::DesignEntity::Assignment, "a"))) });
            expectedResult4.patterns = std::vector<Pattern>();
            expectedResult4.results = std::vector<std::string>();

            Query* result4 = parser4.parse();

            Assert::IsTrue(*result4 == expectedResult4);

            // Parent*(_, _)
            std::string query5 = "assign a; Select a such that Parent*(_, _)";
            QueryLexer lexer5 = QueryLexer(query5);
            std::vector<std::string> tokens5 = lexer5.lex();
            QueryParser parser5 = QueryParser(tokens5);

            Query expectedResult5 = Query();

            expectedResult5.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult5.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult5.relations = std::vector<Relation>({Relation(Relation::Types::ParentT, Reference("_"), Reference("_")) });
            expectedResult5.patterns = std::vector<Pattern>();
            expectedResult5.results = std::vector<std::string>();

            Query* result5 = parser5.parse();

            Assert::IsTrue(*result5 == expectedResult5);

            // Parent*(_, INTEGER)
            std::string query6 = "assign a; Select a such that Parent*(_, 3)";
            QueryLexer lexer6 = QueryLexer(query6);
            std::vector<std::string> tokens6 = lexer6.lex();
            QueryParser parser6 = QueryParser(tokens6);

            Query expectedResult6 = Query();

            expectedResult6.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult6.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult6.relations = std::vector<Relation>({Relation(Relation::Types::ParentT, Reference("_"), Reference("3")) });
            expectedResult6.patterns = std::vector<Pattern>();
            expectedResult6.results = std::vector<std::string>();

            Query* result6 = parser6.parse();

            Assert::IsTrue(*result6 == expectedResult6);

            // Parent*(INTEGER, synonym)
            std::string query7 = "assign a; Select a such that Parent*(2, a)";
            QueryLexer lexer7 = QueryLexer(query7);
            std::vector<std::string> tokens7 = lexer7.lex();
            QueryParser parser7 = QueryParser(tokens7);

            Query expectedResult7 = Query();

            expectedResult7.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult7.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult7.relations = std::vector<Relation>({Relation(Relation::Types::ParentT, Reference("2"), Reference(Declaration(Declaration::DesignEntity::Assignment, "a"))) });
            expectedResult7.patterns = std::vector<Pattern>();
            expectedResult7.results = std::vector<std::string>();

            Query* result7 = parser7.parse();

            Assert::IsTrue(*result7 == expectedResult7);

            // Parent*(INTEGER, _)
            std::string query8 = "assign a; Select a such that Parent*(2, _)";
            QueryLexer lexer8 = QueryLexer(query8);
            std::vector<std::string> tokens8 = lexer8.lex();
            QueryParser parser8 = QueryParser(tokens8);

            Query expectedResult8 = Query();

            expectedResult8.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult8.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult8.relations = std::vector<Relation>({Relation(Relation::Types::ParentT, Reference("2"), Reference("_")) });
            expectedResult8.patterns = std::vector<Pattern>();
            expectedResult8.results = std::vector<std::string>();

            Query* result8 = parser8.parse();

            Assert::IsTrue(*result8 == expectedResult8);

            // Parent*(INTEGER, INTEGER)
            std::string query9 = "assign a; Select a such that Parent*(2, 3)";
            QueryLexer lexer9 = QueryLexer(query9);
            std::vector<std::string> tokens9 = lexer9.lex();
            QueryParser parser9 = QueryParser(tokens9);

            Query expectedResult9 = Query();

            expectedResult9.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });
            expectedResult9.target = Declaration(Declaration::DesignEntity::Assignment, "a");
            expectedResult9.relations = std::vector<Relation>({ Relation(Relation::Types::ParentT, Reference("2"), Reference("3")) });
            expectedResult9.patterns = std::vector<Pattern>();
            expectedResult9.results = std::vector<std::string>();

            Query* result9 = parser9.parse();

            Assert::IsTrue(*result9 == expectedResult9);
        }

        TEST_METHOD(TestParserValidCallsQueries) {
            // Calls(synonym, synonym)
            std::string query1 = "procedure p1, p2; Select p1 such that Calls(p1, p2)";
            QueryLexer lexer1 = QueryLexer(query1);
            std::vector<std::string> tokens1 = lexer1.lex();
            QueryParser parser1 = QueryParser(tokens1);

            Query expectedResult1 = Query();

            expectedResult1.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p1"),  Declaration(Declaration::DesignEntity::Procedure, "p2") });
            expectedResult1.target = Declaration(Declaration::DesignEntity::Procedure, "p1");
            expectedResult1.relations = std::vector<Relation>({Relation(Relation::Types::Calls, Reference(Declaration(Declaration::DesignEntity::Procedure, "p1")), Reference(Declaration(Declaration::DesignEntity::Procedure, "p2")))});
            expectedResult1.patterns = std::vector<Pattern>();
            expectedResult1.results = std::vector<std::string>();

            Query* result1 = parser1.parse();

            Assert::IsTrue(*result1 == expectedResult1);

            // Calls(synonym, _)
            std::string query2 = "procedure p; Select p such that Calls(p, _)";
            QueryLexer lexer2 = QueryLexer(query2);
            std::vector<std::string> tokens2 = lexer2.lex();
            QueryParser parser2 = QueryParser(tokens2);

            Query expectedResult2 = Query();

            expectedResult2.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult2.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult2.relations = std::vector<Relation>({Relation(Relation::Types::Calls, Reference(Declaration(Declaration::DesignEntity::Procedure, "p")), Reference("_"))});
            expectedResult2.patterns = std::vector<Pattern>();
            expectedResult2.results = std::vector<std::string>();

            Query* result2 = parser2.parse();

            Assert::IsTrue(*result2 == expectedResult2);

            // Calls(synonym, "IDENT")
            std::string query3 = "procedure p; Select p such that Calls(p, \"procA\")";
            QueryLexer lexer3 = QueryLexer(query3);
            std::vector<std::string> tokens3 = lexer3.lex();
            QueryParser parser3 = QueryParser(tokens3);

            Query expectedResult3 = Query();

            expectedResult3.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult3.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult3.relations = std::vector<Relation>({Relation(Relation::Types::Calls, Reference(Declaration(Declaration::DesignEntity::Procedure, "p")), Reference("\"procA\""))});
            expectedResult3.patterns = std::vector<Pattern>();
            expectedResult3.results = std::vector<std::string>();

            Query* result3 = parser3.parse();

            Assert::IsTrue(*result3 == expectedResult3);

            // Calls(_, synonym)
            std::string query4 = "procedure p; Select p such that Calls(_, p)";
            QueryLexer lexer4 = QueryLexer(query4);
            std::vector<std::string> tokens4 = lexer4.lex();
            QueryParser parser4 = QueryParser(tokens4);

            Query expectedResult4 = Query();

            expectedResult4.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult4.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult4.relations = std::vector<Relation>({Relation(Relation::Types::Calls, Reference("_"), Reference(Declaration(Declaration::DesignEntity::Procedure, "p")))});
            expectedResult4.patterns = std::vector<Pattern>();
            expectedResult4.results = std::vector<std::string>();

            Query* result4 = parser4.parse();

            Assert::IsTrue(*result4 == expectedResult4);

            // Calls(_, _)
            std::string query5 = "procedure p; Select p such that Calls(_, _)";
            QueryLexer lexer5 = QueryLexer(query5);
            std::vector<std::string> tokens5 = lexer5.lex();
            QueryParser parser5 = QueryParser(tokens5);

            Query expectedResult5 = Query();

            expectedResult5.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult5.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult5.relations = std::vector<Relation>({Relation(Relation::Types::Calls, Reference("_"), Reference("_"))});
            expectedResult5.patterns = std::vector<Pattern>();
            expectedResult5.results = std::vector<std::string>();

            Query* result5 = parser5.parse();

            Assert::IsTrue(*result5 == expectedResult5);

            // Calls(_, "IDENT")
            std::string query6 = "procedure p; Select p such that Calls(_, \"procA\")";
            QueryLexer lexer6 = QueryLexer(query6);
            std::vector<std::string> tokens6 = lexer6.lex();
            QueryParser parser6 = QueryParser(tokens6);

            Query expectedResult6 = Query();

            expectedResult6.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult6.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult6.relations = std::vector<Relation>({Relation(Relation::Types::Calls, Reference("_"), Reference("\"procA\""))});
            expectedResult6.patterns = std::vector<Pattern>();
            expectedResult6.results = std::vector<std::string>();

            Query* result6 = parser6.parse();

            Assert::IsTrue(*result6 == expectedResult6);

            // Calls("IDENT", synonym)
            std::string query7 = "procedure p; Select p such that Calls(\"procA\", p)";
            QueryLexer lexer7 = QueryLexer(query7);
            std::vector<std::string> tokens7 = lexer7.lex();
            QueryParser parser7 = QueryParser(tokens7);

            Query expectedResult7 = Query();

            expectedResult7.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult7.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult7.relations = std::vector<Relation>({Relation(Relation::Types::Calls, Reference("\"procA\""), Reference(Declaration(Declaration::DesignEntity::Procedure, "p")))});
            expectedResult7.patterns = std::vector<Pattern>();
            expectedResult7.results = std::vector<std::string>();

            Query* result7 = parser7.parse();

            Assert::IsTrue(*result7 == expectedResult7);

            // Calls("IDENT", _)
            std::string query8 = "procedure p; Select p such that Calls(\"procA\", _)";
            QueryLexer lexer8 = QueryLexer(query8);
            std::vector<std::string> tokens8 = lexer8.lex();
            QueryParser parser8 = QueryParser(tokens8);

            Query expectedResult8 = Query();

            expectedResult8.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult8.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult8.relations = std::vector<Relation>({Relation(Relation::Types::Calls, Reference("\"procA\""), Reference("_"))});
            expectedResult8.patterns = std::vector<Pattern>();
            expectedResult8.results = std::vector<std::string>();

            Query* result8 = parser8.parse();

            Assert::IsTrue(*result8 == expectedResult8);

            // Calls("IDENT", "IDENT")
            std::string query9 = "procedure p; Select p such that Calls(\"procA\", \"procB\")";
            QueryLexer lexer9 = QueryLexer(query9);
            std::vector<std::string> tokens9 = lexer9.lex();
            QueryParser parser9 = QueryParser(tokens9);

            Query expectedResult9 = Query();

            expectedResult9.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult9.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult9.relations = std::vector<Relation>({Relation(Relation::Types::Calls, Reference("\"procA\""), Reference("\"procB\""))});
            expectedResult9.patterns = std::vector<Pattern>();
            expectedResult9.results = std::vector<std::string>();

            Query* result9 = parser9.parse();

            Assert::IsTrue(*result9 == expectedResult9);

        }

        TEST_METHOD(TestParserValidCallsTQueries) {
            // CallsT(synonym, synonym)
            std::string query1 = "procedure p1, p2; Select p1 such that Calls*(p1, p2)";
            QueryLexer lexer1 = QueryLexer(query1);
            std::vector<std::string> tokens1 = lexer1.lex();
            QueryParser parser1 = QueryParser(tokens1);

            Query expectedResult1 = Query();

            expectedResult1.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p1"),  Declaration(Declaration::DesignEntity::Procedure, "p2") });
            expectedResult1.target = Declaration(Declaration::DesignEntity::Procedure, "p1");
            expectedResult1.relations = std::vector<Relation>({Relation(Relation::Types::CallsT, Reference(Declaration(Declaration::DesignEntity::Procedure, "p1")), Reference(Declaration(Declaration::DesignEntity::Procedure, "p2"))) });
            expectedResult1.patterns = std::vector<Pattern>();
            expectedResult1.results = std::vector<std::string>();

            Query* result1 = parser1.parse();

            Assert::IsTrue(*result1 == expectedResult1);

            // CallsT(synonym, _)
            std::string query2 = "procedure p; Select p such that Calls*(p, _)";
            QueryLexer lexer2 = QueryLexer(query2);
            std::vector<std::string> tokens2 = lexer2.lex();
            QueryParser parser2 = QueryParser(tokens2);

            Query expectedResult2 = Query();

            expectedResult2.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult2.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult2.relations = std::vector<Relation>({ Relation(Relation::Types::CallsT, Reference(Declaration(Declaration::DesignEntity::Procedure, "p")), Reference("_")) });
            expectedResult2.patterns = std::vector<Pattern>();
            expectedResult2.results = std::vector<std::string>();

            Query* result2 = parser2.parse();

            Assert::IsTrue(*result2 == expectedResult2);

            // CallsT(synonym, "IDENT")
            std::string query3 = "procedure p; Select p such that Calls*(p, \"procA\")";
            QueryLexer lexer3 = QueryLexer(query3);
            std::vector<std::string> tokens3 = lexer3.lex();
            QueryParser parser3 = QueryParser(tokens3);

            Query expectedResult3 = Query();

            expectedResult3.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult3.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult3.relations = std::vector<Relation>({ Relation(Relation::Types::CallsT, Reference(Declaration(Declaration::DesignEntity::Procedure, "p")), Reference("\"procA\"")) });
            expectedResult3.patterns = std::vector<Pattern>();
            expectedResult3.results = std::vector<std::string>();

            Query* result3 = parser3.parse();

            Assert::IsTrue(*result3 == expectedResult3);

            // CallsT(_, synonym)
            std::string query4 = "procedure p; Select p such that Calls*(_, p)";
            QueryLexer lexer4 = QueryLexer(query4);
            std::vector<std::string> tokens4 = lexer4.lex();
            QueryParser parser4 = QueryParser(tokens4);

            Query expectedResult4 = Query();

            expectedResult4.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult4.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult4.relations = std::vector<Relation>({ Relation(Relation::Types::CallsT, Reference("_"), Reference(Declaration(Declaration::DesignEntity::Procedure, "p"))) });
            expectedResult4.patterns = std::vector<Pattern>();
            expectedResult4.results = std::vector<std::string>();

            Query* result4 = parser4.parse();

            Assert::IsTrue(*result4 == expectedResult4);

            // CallsT(_, _)
            std::string query5 = "procedure p; Select p such that Calls*(_, _)";
            QueryLexer lexer5 = QueryLexer(query5);
            std::vector<std::string> tokens5 = lexer5.lex();
            QueryParser parser5 = QueryParser(tokens5);

            Query expectedResult5 = Query();

            expectedResult5.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult5.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult5.relations = std::vector<Relation>({ Relation(Relation::Types::CallsT, Reference("_"), Reference("_")) });
            expectedResult5.patterns = std::vector<Pattern>();
            expectedResult5.results = std::vector<std::string>();

            Query* result5 = parser5.parse();

            Assert::IsTrue(*result5 == expectedResult5);

            // CallsT(_, "IDENT")
            std::string query6 = "procedure p; Select p such that Calls*(_, \"procA\")";
            QueryLexer lexer6 = QueryLexer(query6);
            std::vector<std::string> tokens6 = lexer6.lex();
            QueryParser parser6 = QueryParser(tokens6);

            Query expectedResult6 = Query();

            expectedResult6.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult6.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult6.relations = std::vector<Relation>({ Relation(Relation::Types::CallsT, Reference("_"), Reference("\"procA\"")) });
            expectedResult6.patterns = std::vector<Pattern>();
            expectedResult6.results = std::vector<std::string>();

            Query* result6 = parser6.parse();

            Assert::IsTrue(*result6 == expectedResult6);

            // CallsT("IDENT", synonym)
            std::string query7 = "procedure p; Select p such that Calls*(\"procA\", p)";
            QueryLexer lexer7 = QueryLexer(query7);
            std::vector<std::string> tokens7 = lexer7.lex();
            QueryParser parser7 = QueryParser(tokens7);

            Query expectedResult7 = Query();

            expectedResult7.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult7.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult7.relations = std::vector<Relation>({ Relation(Relation::Types::CallsT, Reference("\"procA\""), Reference(Declaration(Declaration::DesignEntity::Procedure, "p"))) });
            expectedResult7.patterns = std::vector<Pattern>();
            expectedResult7.results = std::vector<std::string>();

            Query* result7 = parser7.parse();

            Assert::IsTrue(*result7 == expectedResult7);

            // CallsT("IDENT", _)
            std::string query8 = "procedure p; Select p such that Calls*(\"procA\", _)";
            QueryLexer lexer8 = QueryLexer(query8);
            std::vector<std::string> tokens8 = lexer8.lex();
            QueryParser parser8 = QueryParser(tokens8);

            Query expectedResult8 = Query();

            expectedResult8.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult8.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult8.relations = std::vector<Relation>({ Relation(Relation::Types::CallsT, Reference("\"procA\""), Reference("_")) });
            expectedResult8.patterns = std::vector<Pattern>();
            expectedResult8.results = std::vector<std::string>();

            Query* result8 = parser8.parse();

            Assert::IsTrue(*result8 == expectedResult8);

            // CallsT("IDENT", "IDENT")
            std::string query9 = "procedure p; Select p such that Calls*(\"procA\", \"procB\")";
            QueryLexer lexer9 = QueryLexer(query9);
            std::vector<std::string> tokens9 = lexer9.lex();
            QueryParser parser9 = QueryParser(tokens9);

            Query expectedResult9 = Query();

            expectedResult9.declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Procedure, "p") });
            expectedResult9.target = Declaration(Declaration::DesignEntity::Procedure, "p");
            expectedResult9.relations = std::vector<Relation>({ Relation(Relation::Types::CallsT, Reference("\"procA\""), Reference("\"procB\"")) });
            expectedResult9.patterns = std::vector<Pattern>();
            expectedResult9.results = std::vector<std::string>();

            Query* result9 = parser9.parse();

            Assert::IsTrue(*result9 == expectedResult9);
        }

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