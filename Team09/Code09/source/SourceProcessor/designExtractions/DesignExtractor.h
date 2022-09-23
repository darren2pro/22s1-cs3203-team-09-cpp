#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../../ProgramKnowledgeBase/PKBStorage.h"

class DesignExtractor {	
public:
    std::shared_ptr<PKB::PKBStorage> pkbFacade;
	
    DesignExtractor(std::shared_ptr<PKB::PKBStorage> pkb) : pkbFacade(pkb) {};

    ~DesignExtractor();

    void extractDesignAbstractions(const std::shared_ptr<ProgramNode> astRoot);
};
