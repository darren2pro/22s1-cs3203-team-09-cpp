#include "SimpleInvalidSyntaxException.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

SimpleInvalidSyntaxException::SimpleInvalidSyntaxException(const string& msg)
        : runtime_error(msg) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::string timestamp = std::ctime(&in_time_t);
    ofstream logFile(timestamp + ".txt");
    logFile << msg;
    logFile.close();
}
