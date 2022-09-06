#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../astBuilder/TNode.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"

using namespace PKB;

class DesignExtractor {	
public:
    PKBStorage* pkbFacade;
	
    DesignExtractor(PKBStorage* pkb) : pkbFacade(pkb) {};

    ~DesignExtractor();

    void extractDesignAbstractions(TNode::PROGRAM_NODE_PTR astRoot);
};
