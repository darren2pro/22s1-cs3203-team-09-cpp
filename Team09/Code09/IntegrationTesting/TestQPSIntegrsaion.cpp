#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryProcessingSubsystem/Validator/QueryBuilder.h"
#include "QueryProcessingSubsystem/Executor/QueryExecutor.h"
#include "QueryProcessingSubsystem/Declaration.h"
#include "QueryProcessingSubsystem/Result.h"
#include "QueryProcessingSubsystem/Relation.h"
#include "QueryProcessingSubsystem/Pattern.h"
#include "QueryProcessingSubsystem/With.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

/*
 * Integration tests between QPS Validator and Executor.
 */
namespace IntegrationTesting {
    TEST_CLASS(TestQPSIntegration) {
    public:
        PKBStorage* pkb = new PKBStorage();     // an empty pkb to instantiate the query executor for the sake of testing
        QueryExecutor* qm = new QueryExecutor(pkb);
        QueryBuilder qb;
              
        TEST_METHOD(TestBasicQuery) {
            std::string query = "print a; Select a";
            Query* queryAdt = qb.buildQuery(query);
            qm->processQuery(queryAdt, false);

            std::vector<Relation> relations = std::vector<Relation>();
            std::vector<Pattern> patterns = std::vector<Pattern>();
            std::vector<With> withs = std::vector<With>();
            Result target = Result(Result::Types::Tuple, std::vector<std::variant<Declaration, AttrReference>>({ Declaration(Declaration::DesignEntity::Print, "a") }));
            std::vector<Declaration> declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Print, "a") });

            Assert::IsTrue(qm->getRelations() == relations);
            Assert::IsTrue(qm->getPatterns() == patterns);
            Assert::IsTrue(qm->getWiths() == withs);
            Assert::IsTrue(qm->getTarget() == target);
            Assert::IsTrue(qm->getDeclarations() == declarations);
        }        
        
        TEST_METHOD(TestQueryOneSuchThatClause) {
            std::string query = "assign a; Select a such that Follows (_,_)";
            Query* queryAdt = qb.buildQuery(query);
            std::unordered_set<std::string> results = qm->processQuery(queryAdt, false);

            std::vector<Relation> relations = std::vector<Relation>({ Relation(Relation::Types::Follows, Reference("_"), Reference("_")) });
            std::vector<Pattern> patterns = std::vector<Pattern>();
            std::vector<With> withs = std::vector<With>();
            Result target = Result(Result::Types::Tuple, std::vector<std::variant<Declaration, AttrReference>>({ Declaration(Declaration::DesignEntity::Assignment, "a") }));
            std::vector<Declaration> declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });

            Assert::IsTrue(qm->getRelations() == relations);
            Assert::IsTrue(qm->getPatterns() == patterns);
            Assert::IsTrue(qm->getWiths() == withs);
            Assert::IsTrue(qm->getTarget() == target);
            Assert::IsTrue(qm->getDeclarations() == declarations);
        }          

        TEST_METHOD(TestQueryOnePatternClause) {
            std::string query = "assign a; Select a pattern a (_,_)";
            Query* queryAdt = qb.buildQuery(query);
            std::unordered_set<std::string> results = qm->processQuery(queryAdt, false);

            std::vector<Relation> relations = std::vector<Relation>();
            std::vector<Pattern> patterns = std::vector<Pattern>({Pattern(Pattern::Types::Assign, Declaration(Declaration::DesignEntity::Assignment, "a"),
                                                                           Reference("_"), Expression("_"))});
            std::vector<With> withs = std::vector<With>();
            Result target = Result(Result::Types::Tuple, std::vector<std::variant<Declaration, AttrReference>>({ Declaration(Declaration::DesignEntity::Assignment, "a") }));
            std::vector<Declaration> declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });

            Assert::IsTrue(qm->getRelations() == relations);
            Assert::IsTrue(qm->getPatterns() == patterns);
            Assert::IsTrue(qm->getWiths() == withs);
            Assert::IsTrue(qm->getTarget() == target);
            Assert::IsTrue(qm->getDeclarations() == declarations);
        }      

        TEST_METHOD(TestQueryOneWithClause) {
            std::string query = "assign a; Select a with 1 = 1";
            Query* queryAdt = qb.buildQuery(query);
            std::unordered_set<std::string> results = qm->processQuery(queryAdt, false);

            std::vector<Relation> relations = std::vector<Relation>();
            std::vector<Pattern> patterns = std::vector<Pattern>();
            std::vector<With> withs = std::vector<With>({With(AttrReference("1"), AttrReference("1"))});
            Result target = Result(Result::Types::Tuple, std::vector<std::variant<Declaration, AttrReference>>({ Declaration(Declaration::DesignEntity::Assignment, "a") }));
            std::vector<Declaration> declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });

            Assert::IsTrue(qm->getRelations() == relations);
            Assert::IsTrue(qm->getPatterns() == patterns);
            Assert::IsTrue(qm->getWiths() == withs);
            Assert::IsTrue(qm->getTarget() == target);
            Assert::IsTrue(qm->getDeclarations() == declarations);
        }        
        
        TEST_METHOD(TestQueryMultipleClauses) {
            std::string query = "assign a; if ifs; Select a such that Follows(_, _) and Next(_, _) pattern a(_,_) and ifs(_,_,_) with  a.stmt# = 5 and ifs.stmt# = 8";
            Query* queryAdt = qb.buildQuery(query);
            std::unordered_set<std::string> results = qm->processQuery(queryAdt, false);

            std::vector<Relation> relations = std::vector<Relation>({ Relation(Relation::Types::Follows, Reference("_"), Reference("_")) ,
                                                                      Relation(Relation::Types::Next, Reference("_"), Reference("_")) });
            std::vector<Pattern> patterns = std::vector<Pattern>({Pattern(Pattern::Types::Assign, Declaration(Declaration::DesignEntity::Assignment, "a"), Reference("_"), Expression("_")),
                                                                  Pattern(Pattern::Types::If, Declaration(Declaration::DesignEntity::If, "ifs"), Reference("_")) });
            std::vector<With> withs = std::vector<With>({ With(AttrReference(Declaration(Declaration::DesignEntity::Assignment, "a"), AttrReference::Attribute::StmtNum),AttrReference("5")),
                                                          With(AttrReference(Declaration(Declaration::DesignEntity::If, "ifs"), AttrReference::Attribute::StmtNum),AttrReference("8")) });
            Result target = Result(Result::Types::Tuple, std::vector<std::variant<Declaration, AttrReference>>({ Declaration(Declaration::DesignEntity::Assignment, "a") }));
            std::vector<Declaration> declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a"), Declaration(Declaration::DesignEntity::If, "ifs") });

            Assert::IsTrue(qm->getRelations() == relations);
            Assert::IsTrue(qm->getPatterns() == patterns);
            Assert::IsTrue(qm->getWiths() == withs);
            Assert::IsTrue(qm->getTarget() == target);
            Assert::IsTrue(qm->getDeclarations() == declarations);
        }        
        
        TEST_METHOD(TestBooleanQuery) {
            std::string query = "assign a; Select BOOLEAN";
            Query* queryAdt = qb.buildQuery(query);
            qm->processQuery(queryAdt, false);

            std::vector<Relation> relations = std::vector<Relation>();
            std::vector<Pattern> patterns = std::vector<Pattern>();
            std::vector<With> withs = std::vector<With>();
            Result target = Result(Result::Types::Boolean);
            std::vector<Declaration> declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Assignment, "a") });

            Assert::IsTrue(qm->getRelations() == relations);
            Assert::IsTrue(qm->getPatterns() == patterns);
            Assert::IsTrue(qm->getWiths() == withs);
            Assert::IsTrue(qm->getTarget() == target);
            Assert::IsTrue(qm->getDeclarations() == declarations);
        }        
        
        TEST_METHOD(TestTupleQuery) {
            std::string query = "print a1, a2; Select <a1, a2>";
            Query* queryAdt = qb.buildQuery(query);
            qm->processQuery(queryAdt, false);

            std::vector<Relation> relations = std::vector<Relation>();
            std::vector<Pattern> patterns = std::vector<Pattern>();
            std::vector<With> withs = std::vector<With>();
            Result target = Result(Result::Types::Tuple, std::vector<std::variant<Declaration, AttrReference>>({ Declaration(Declaration::DesignEntity::Print, "a1"), Declaration(Declaration::DesignEntity::Print, "a2") }));
            std::vector<Declaration> declarations = std::vector<Declaration>({ Declaration(Declaration::DesignEntity::Print, "a1"), Declaration(Declaration::DesignEntity::Print, "a2") });

            Assert::IsTrue(qm->getRelations() == relations);
            Assert::IsTrue(qm->getPatterns() == patterns);
            Assert::IsTrue(qm->getWiths() == withs);
            Assert::IsTrue(qm->getTarget() == target);
            Assert::IsTrue(qm->getDeclarations() == declarations);
        }

    };
}