#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PKB.h"
#include "Tnode.h"

class DesignExtractor {
private:
    PKB pkbFacade;

public:
    explicit DesignExtractor(PKB pkb);

    ~DesignExtractor();

    void extractDesignAbstractions(TNode::PROGRAM_NODE_PTR astRoot);
};
