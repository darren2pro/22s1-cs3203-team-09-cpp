#pragma once
#include <string>
#include <vector>
#include "AttrReference.h"

class With {
public:

	AttrReference ref1;
	AttrReference ref2;

	With(AttrReference ref1, AttrReference ref2) : ref1(ref1), ref2(ref2) {}

	bool operator==(const With& w) const {
		return ref1 == w.ref1 && ref2 == w.ref2;
	}
};