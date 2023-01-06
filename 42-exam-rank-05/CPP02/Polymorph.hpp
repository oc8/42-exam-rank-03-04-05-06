#pragma once

#include <iostream>

#include "ASpell.hpp"

using std::string;
using std::cout;
using std::endl;

class Polymorph: public ASpell {
private:

public:
	Polymorph(): ASpell("Polymorph", "turned into a critter") {}
	virtual ~Polymorph() {}
	virtual Polymorph *clone() const {return new Polymorph;}
};

