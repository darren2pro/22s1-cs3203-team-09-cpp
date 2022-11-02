#include "TestWrapper.h"
#include "SourceProcessor/Parser.h"
#include <sstream>
#include <string>
#include <fstream>
#include <memory>
#include <unordered_set>
#include "../source/QueryProcessingSubsystem/Executor/QueryExecutor.h"
#include "../source/QueryProcessingSubsystem/Validator/QueryBuilder.h"
#include "../source/QueryProcessingSubsystem/Validator/SyntaxException.h"
#include "../source/QueryProcessingSubsystem/Validator/SemanticException.h"
#include "SourceProcessor/exceptions/SimpleInvalidSyntaxException.h"

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
    spaManager = new SPAManager();
}

TestWrapper::~TestWrapper() {
    delete spaManager;
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
    try {
        string program = readFileIntoString(filename);
        spaManager->loadSimpleSourceFromProgram(program);
	}
	catch (SimpleInvalidSyntaxException& e) {
        std::cout << e.what() << std::endl;
        std::cout << "Invalid syntax. Please input another file." << std::endl;
        exit(1);
	}   
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string> &results) {
   try {
	    unordered_set<string> queryResults = spaManager->query(query);
        for (std::string result : queryResults) {
            results.push_back(result);
        }
    }
    catch (SyntaxError& syntaxException) {
        results.push_back("SyntaxError");
        std::cout << syntaxException.what() << std::endl;
		std::cout << "Invalid syntax. Please input another query." << std::endl;
    }
    catch (SemanticError& semanticException) {
        results.push_back("SemanticError");
		std::cout << semanticException.what() << std::endl;
		std::cout << "Semantic error. Please input another query." << std::endl;
    }
    catch (SimpleInvalidSyntaxException& simpleInvalidSyntaxFromParser) {
        results.push_back("SyntaxError");
        std::cout << simpleInvalidSyntaxFromParser.what() << std::endl;
        std::cout << "Invalid syntax for this query's arithmetic expression. Please input another query." << std::endl;
    }
}
