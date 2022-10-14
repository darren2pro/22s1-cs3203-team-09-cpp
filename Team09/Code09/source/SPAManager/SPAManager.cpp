#include "SPAManager.h"
#include "../TNode/TNode.h"
#include "../QueryProcessingSubsystem/Validator/QueryBuilder.h"
#include "../SourceProcessor/designExtractions/DesignExtractor.h"
#include "../SourceProcessor/SimpleInterface.h"

using namespace std;

SPAManager::SPAManager() {
    pkb = nullptr;
    qm = nullptr;
}

SPAManager::~SPAManager() {
    delete pkb;
    delete qm;
}

void SPAManager::loadSimpleSource(string fileName) {
    if (pkb != nullptr) {
        delete pkb;
    }
    if (qm != nullptr) {
        delete qm;
    }
    AST ast = SimpleInterface::getAstFromFile(fileName);
    pkb = new PKBStorage();
    qm = new QueryExecutor(pkb);

    //! Now populate the pkb with the necessary information we get from the ast
    DesignExtractor extractor(pkb);
    extractor.extractDesignAbstractions(ast);
}

void SPAManager::loadSimpleSourceFromProgram(string program) {
    if (pkb != nullptr) {
        delete pkb;
    }
    if (qm != nullptr) {
        delete qm;
    }
    AST ast = SimpleInterface::getAstFromProgram(program);
    pkb = new PKBStorage();
    qm = new QueryExecutor(pkb);

    //! Now populate the pkb with the necessary information we get from the ast
    DesignExtractor extractor(pkb);
    extractor.extractDesignAbstractions(ast);
}

unordered_set<string> SPAManager::query(string& pqlQuery) {
    QueryBuilder qb;
    Query* queryAdt = qb.buildQuery(pqlQuery);
    unordered_set<string> queryResults = qm->processQuery(queryAdt);
    return queryResults;
}
