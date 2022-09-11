#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PKBStorage.h"

namespace PKB {

class PKBManager {
private:
	std::shared_ptr<PKBStorage> pkbStorage = std::make_shared<PKBStorage>();
public:
	PKBManager();
	~PKBManager();

	std::shared_ptr<PKBStorage> PKBManager::getPKBStorage();
};

}





