#pragma once

#include <iostream>

#include "ASpell.hpp"

using std::cout;
using std::endl;
using std::string;

class Fireball : public ASpell
{
private:
public:
	Fireball() : ASpell("Fireball", "burnt to a crisp") {}
	virtual ~Fireball() {}
	virtual Fireball *clone() const { return new Fireball; }
};
