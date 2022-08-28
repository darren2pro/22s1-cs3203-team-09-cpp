#include <string>
#include <vector>
#include "../../../SPA/Query.h"

class QueryValidator {
public:
	QueryValidator(Query query);
	bool validate();
};
