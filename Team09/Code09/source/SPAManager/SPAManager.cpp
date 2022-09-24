#include "SPAManager.h"
#include "../TNode/TNode.h"
#include "../QueryProcessingSubsystem/Validator/QueryBuilder.h"
#include "../SourceProcessor/designExtractions/DesignExtractor.h"
#include "../SourceProcessor/SimpleInterface.h"

using namespace std;

void SPAManager::loadSimpleSource(string fileName) {
    if (pkb != nullptr) delete pkb;
    if (qm != nullptr) delete qm;
    AST ast = SimpleInterface::getAstFromFile(fileName);
    pkb = new PKBManager();
    qm = new QueryExecutor(pkb);

    //! Now populate the pkb with the necessary information we get from the ast
    DesignExtractor extractor(pkb->getPKBStorage());
    extractor.extractDesignAbstractions(ast);
}

void SPAManager::loadSimpleSourceFromProgram(string program) {
    if (pkb != nullptr) delete pkb;
    if (qm != nullptr) delete qm;
    AST ast = SimpleInterface::getAstFromProgram(program);
    pkb = new PKBManager();
    qm = new QueryExecutor(pkb);

    //! Now populate the pkb with the necessary information we get from the ast
    DesignExtractor extractor(pkb->getPKBStorage());
    extractor.extractDesignAbstractions(ast);
}

unordered_set<string> SPAManager::query(string& pqlQuery) {
    QueryBuilder qb;
    Query* queryAdt = qb.buildQuery(pqlQuery);
    unordered_set<string> queryResults = qm->processQuery(queryAdt);
    return queryResults;
}

SPAManager::~SPAManager() {
    delete pkb;
    delete qm;
}
