#include "TestWrapper.h"
#include "SourceProcessor/Parser.h"
#include <sstream>
#include <string>
#include <fstream>
#include <memory>
#include <unordered_set>

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
    std::string str((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());
    SimpleParser simpleParser = SimpleParser(str);
    simpleParser.parse();
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string> &results) {
    // call your evaluator to evaluate the query here
    // ...code to evaluate query...

    // store the answers to the query in the results list (it is initially empty)
    // each result must be a string.
    std::cout << "TestWrapper::evaluate called" << std::endl;
    results.push_back("sampleDarrenResult");
}
