#include "TestWrapper.h"
#include "SourceProcessor/Parser.h"
#include <sstream>
#include <string>
#include <fstream>
#include <memory>
#include <unordered_set>
#include "../source/QueryProcessingSubsystem/Executor/QueryExecutor.h"
#include "../source/QueryProcessingSubsystem/Validator/QueryBuilder.h"

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
    std::cout << "parse called" << std::endl;
    std::ifstream file(filename);
    std::string str((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());
    SimpleParser simpleParser = SimpleParser(str);
    this->pkb = simpleParser.parse();
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string> &results) {
    // call your evaluator to evaluate the query here
    // ...code to evaluate query...	
    std::cout << "evaluate called" << std::endl;
    auto queryAdt = QueryBuilder().buildQuery(query);
    auto qe = QueryExecutor(this->pkb);
    auto queryResults = qe.processQuery(queryAdt);
    for (auto result : *queryResults) {
        results.push_back(result);
    }
}
