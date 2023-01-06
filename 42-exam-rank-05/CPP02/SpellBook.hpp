#pragma once

#include <iostream>
#include <map>

using std::string;
using std::cout;
using std::endl;
using std::map;

#include "ASpell.hpp"

class SpellBook {
private:
	typedef map<string, ASpell *>::iterator map_it;
	SpellBook(const SpellBook &other);
	SpellBook &operator=(const SpellBook &other);
	map<string, ASpell *> spells;

public:
	SpellBook() {}
	virtual ~SpellBook() {
		for (map_it it = spells.begin(); it != spells.end(); ++it)
			delete it->second;
	}
	void learnSpell(ASpell *aspell) {
		spells[aspell->getName()] = aspell->clone();
	}
	void forgetSpell(const string &spell) {
		delete spells.find(spell)->second;
		spells.erase(spell);
	}
	ASpell *createSpell(const string &spell) {
		if (spells.find(spell) != spells.end())
			return spells[spell];
		return NULL;
	}
};
