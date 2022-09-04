#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../astBuilder/TNode.h"
#include "../../PKB.h"
// #include "PKB.h"

class DesignExtractor {
private:
    PKB pkbFacade;

public:
    explicit DesignExtractor(PKB pkb);

    ~DesignExtractor();

    void extractDesignAbstractions(TNode::PROGRAM_NODE_PTR astRoot);
};
