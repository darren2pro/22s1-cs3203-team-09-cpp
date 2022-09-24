#pragma once

#include "../QueryProcessingSubsystem/Executor/QueryExecutor.h"
#include "../ProgramKnowledgeBase/PKBManager.h"
#include <unordered_set>

using namespace std;

class SPAManager {
private:
    PKBManager* pkb;
    QueryExecutor* qm;

public:
    void loadSimpleSource(string fileName);
    void loadSimpleSourceFromProgram(string program);
    unordered_set<string> query(string& pqlQuery);
    ~SPAManager();
};
