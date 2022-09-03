#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PKB.h"
#include <SourceProcessor/astBuilder/TNode.h>

class RelationshipExtraction {
private:
	PKB pkbFacade;

public:
	RelationshipExtraction(PKB pkb);
	~RelationshipExtraction();
	void extractRls(PROGRAM_NODE_PTR astRoot);

};