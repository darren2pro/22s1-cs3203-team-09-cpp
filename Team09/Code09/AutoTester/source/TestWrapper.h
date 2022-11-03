#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>

// include your other headers here
#include "AbstractWrapper.h"
#include "SPAManager/SPAManager.h"

class TestWrapper : public AbstractWrapper {
private:
    SPAManager* spaManager;

    std::string readFileIntoString(const std::string& path) {
        ifstream input_file(path);
        if (!input_file.is_open()) {
            cerr << "Could not open the file - '"
                 << path << "'" << endl;
            exit(EXIT_FAILURE);
        }
        return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    }

public:
    // default constructor
    TestWrapper();

    // destructor
    ~TestWrapper();

    // method for parsing the SIMPLE source
    virtual void parse(std::string filename);

    // method for evaluating a query
    virtual void evaluate(std::string query, std::list<std::string> &results);
};

#endif
