#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../../ProgramKnowledgeBase/PKBStorage.h"

class DesignExtractor {	
public:
    std::shared_ptr<PKBStorage> pkbFacade;
	
    DesignExtractor(std::shared_ptr<PKBStorage> pkb) : pkbFacade(pkb) {};

    ~DesignExtractor();

    void extractDesignAbstractions(const std::shared_ptr<ProgramNode> astRoot);
};
