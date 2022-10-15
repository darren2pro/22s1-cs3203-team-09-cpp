#pragma once

#include "../QueryProcessingSubsystem/Executor/QueryExecutor.h"
#include "../ProgramKnowledgeBase/PKBStorage.h"
#include <unordered_set>

using namespace std;

class SPAManager {
private:
    PKBStorage* pkb;
    QueryExecutor* qm;

public:
    SPAManager();
    ~SPAManager();
    void loadSimpleSource(string fileName);
    void loadSimpleSourceFromProgram(string program);
    unordered_set<string> query(string& pqlQuery);
};
