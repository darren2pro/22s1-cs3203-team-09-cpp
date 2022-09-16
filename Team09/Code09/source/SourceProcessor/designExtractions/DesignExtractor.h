#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../../ProgramKnowledgeBase/PKBStorage.h"

class DesignExtractor {	
public:
    PKBStorage* pkbFacade;
	
    DesignExtractor(PKBStorage* pkb) : pkbFacade(pkb) {};

    ~DesignExtractor();

    // void extractDesignAbstractions(TNode::PROGRAM_NODE_PTR astRoot);
};
