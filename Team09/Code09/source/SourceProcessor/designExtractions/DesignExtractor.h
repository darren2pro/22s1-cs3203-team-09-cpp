#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../../ProgramKnowledgeBase/PKBStorage.h"

class DesignExtractor {	
public:
    PKB::PKBStorage* pkbFacade;
	
    DesignExtractor(PKB::PKBStorage* pkb) : pkbFacade(pkb) {};

    ~DesignExtractor();

    void extractDesignAbstractions(const std::shared_ptr<ProgramNode> astRoot);
};
