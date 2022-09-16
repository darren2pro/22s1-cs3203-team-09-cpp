#pragma once

#include <iostream>
#include <string>
#include <vector>
#include"TNode/TNode.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"

using namespace PKB;

class DesignExtractor {	
public:
    std::shared_ptr<PKBStorage> pkbFacade;

    DesignExtractor(PKBStorage* pkb) : pkbFacade(pkb) {};

    ~DesignExtractor();

    void extractDesignAbstractions(const std::shared_ptr<ProgramNode> astRoot);
 };
