#include "stdafx.h"
#include "CppUnitTest.h"
#include <SourceProcessor/Parser.h>
#include <QueryProcessingSubsystem/Query.h>
#include <QueryProcessingSubsystem/Validator/QueryBuilder.h>
#include <QueryProcessingSubsystem/Validator/SyntaxException.h>
#include <QueryProcessingSubsystem/Validator/SemanticException.h>
#include <QueryProcessingSubsystem/Executor/QueryExecutor.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting {
    TEST_CLASS(TestExecute) {
            public:
            TEST_METHOD(TestExecuteSampleQuery) {
                string program = "procedure q {\n"
                                 "  if (x==1) then {\n"
                                 "    z = x + 1; }\n"
                                 "  else {\n"
                                 "    x = z + x; } }\n";
                std::istringstream iss(program);
                SimpleParser parser(&iss);
                PKBManager pkb = parser.parse();
                string query1 = "while w;\n"
                                "Select w such that Parent(w, 7)";
                Query* q1 = QueryBuilder().buildQuery(query1);
                QueryExecutor executor(pkb);
                unordered_set<string> results1 = executor.processQuery(q1);
                
            }
    };
}
