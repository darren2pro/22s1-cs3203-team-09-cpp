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
        
        TEST_METHOD(TestQueryOneClause) {
            // such that
            std::string query = "assign a; Select a such that Follows (_,_)";
            Query* queryAdt = qb.buildQuery(query);
            std::unordered_set<std::string> results =  qm->processQuery(queryAdt, false);

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

            // pattern

            // with
        }        
        
        TEST_METHOD(TestQueryMultipleClause) {
            // multiple such that

            // multiple pattern

            // multiple with

            // mix

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

        }

    };
}