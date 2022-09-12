#include "TestWrapper.h"
#include "SourceProcessor/Parser.h"
#include <sstream>
#include <string>
#include <fstream>
#include <memory>
#include <unordered_set>
#include "../source/QueryProcessingSubsystem/Executor/QueryExecutor.h"
#include "../source/QueryProcessingSubsystem/Validator/QueryBuilder.h"
#include "../source/QueryProcessingSubsystem/Validator/QPSValidatorException.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;

AbstractWrapper* WrapperFactory::createWrapper() {
    if (wrapper == 0) wrapper = new TestWrapper;
    return wrapper;
}

// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
    // create any objects here as instance variables of this class
    // as well as any initialization required for your spa program
    std::cout << "TestWrapper constructor called" << std::endl;
}

TestWrapper::~TestWrapper() {
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
    // read the SIMPLE source file as string and call your simpleParser
    std::ifstream file(filename);
    SimpleParser simpleParser = SimpleParser(&file);
    this->pkb = simpleParser.parse();
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string> &results) {
    Query* queryAdt = QueryBuilder().buildQuery(query);
    QueryExecutor qe(this->pkb);
    std::unordered_set<std::string> queryResults = qe.processQuery(queryAdt);
    for (std::string result : queryResults) {
        results.push_back(result);
    }
}
