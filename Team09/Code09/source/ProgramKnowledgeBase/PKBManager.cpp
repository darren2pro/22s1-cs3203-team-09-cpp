#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PKB.h"

PKBManager::PKBManager() {}

PKBManager::~PKBManager() {}

//getter for pkbStorage. Used by SimpleParser. DesignExtractor extractor(pkb.getPKBStorage).
std::shared_ptr<PKBStorage> PKBManager::getPKBStorage() {
    return pkbStorage;
}

