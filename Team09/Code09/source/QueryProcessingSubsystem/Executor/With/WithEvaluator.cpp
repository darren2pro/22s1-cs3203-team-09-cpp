#include <string>
#include <vector>
#include "WithEvaluator.h"
#include "../QueryExecutor.h"
#include "../../Utils.h"

bool WithEvaluator::evaluate() {
	// Check the left and right argument. If they are synonyms, must
	// get their entire variable set from pkb and populate it first.

	bool isLeftInt = leftArg.isInteger();
	bool isLeftIdent = leftArg.isIdent();
	bool isLeftAttrRef = leftArg.isSynonym();

	bool isRightInt = rightArg.isInteger();
	bool isRightIdent = rightArg.isIdent();
	bool isRightAttrRef = rightArg.isSynonym();

	// Pre-fill for the 2 attribute references
	if (isLeftAttrRef) {
		Declaration synonym = leftArg.declaration;
		leftSynonym = synonym.name;
		QueryExecutor::insertSynonymSetIntoRDB(synonym, rdb, pkb);
	}

	if (isRightAttrRef) {
		Declaration synonym = rightArg.declaration;
		leftSynonym = synonym.name;
		QueryExecutor::insertSynonymSetIntoRDB(synonym, rdb, pkb);
	}

	// NOTE FOR ATTRIBUTE REFERENCE
	// only exceptions: call/read/print.procName -> stmt# to procname. The rest of attrRef is just itself, no change.

	// Int Int
	if (isLeftInt && isRightInt) {
		return leftArg == rightArg;
	}
	// Ident Ident
	else if (isLeftIdent && isRightIdent) {
		return leftArg == rightArg;
	}
	// AttrRef AttrRef
	else if (isLeftAttrRef && isRightAttrRef) {
		// More complicated. Fetch both lists and store the correct results
		// p.procName = c.procName -> Store p values and all c values belonging to that procedure
		return leftArg == rightArg;
	}
	// Int Ident
	else if ((isLeftInt && isRightIdent) || (isRightInt && isLeftIdent)) {
		// with 5 = "cenX"
		// Idea: fetch all lines for ident, then if lines contain 5, set cenX table to be only 5.
		return true;
	}
	// Int AttrRef
	else if ((isLeftInt && isRightAttrRef) || (isRightAttrRef && isLeftInt)) {
		// With s.stmt# = 5
		// Idea: fetch all lines for s, then if lines contain 5, set s table to be only 5.
		return true;
	}
	// Ident AttrRef
	else if ((isLeftIdent && isRightAttrRef) || (isRightAttrRef && isLeftIdent)) {
		// With "proc1" = c.procName
		// Idea: fetch all lines for c. If their procName = proc1, set the c table to be only those lines.
		return true;
	}
	else {
		return true;
	}
}
