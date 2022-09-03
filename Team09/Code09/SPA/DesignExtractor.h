#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PKB.h"
#include "Tnode.h"

class DesignExtractor {
private:
	PKB pkbFacade;

public: 
	DesignExtractor(PKB pkb);
	~DesignExtractor();
	void extractDesignAbstractions(PROGRAM_NODE_PTR astRoot);

};
