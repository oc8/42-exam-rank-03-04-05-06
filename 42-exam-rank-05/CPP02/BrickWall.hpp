#pragma once

#include <iostream>

#include "ASpell.hpp"

using std::string;
using std::cout;
using std::endl;

class BrickWall: public ATarget {
private:

public:
	BrickWall(): ATarget("Inconspicuous Red-brick Wall") {}
	virtual ~BrickWall() {}
	virtual BrickWall *clone() const {return new BrickWall;}
};
