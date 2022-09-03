#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PKB.h"
#include "RelationshipExtraction.h"
#include "TNode.h"

RelationshipExtraction::RelationshipExtraction(PKB pkb) {
	pkbFacade = pkb;
}

RelationshipExtraction::~RelationshipExtraction() {}

void RelationshipExtraction::extractRls(TNode subtreeRoot) {


}