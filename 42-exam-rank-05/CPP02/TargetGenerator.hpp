#pragma once

#include <iostream>
#include <map>

using std::string;
using std::cout;
using std::endl;
using std::map;

#include "ATarget.hpp"

class TargetGenerator {
private:
	typedef map<string, ATarget *>::iterator map_it;
	TargetGenerator(const TargetGenerator &other);
	TargetGenerator &operator=(const TargetGenerator &other);
	map<string, ATarget *> spells;

public:
	TargetGenerator() {}
	virtual ~TargetGenerator() {
		for (map_it it = spells.begin(); it != spells.end(); ++it)
			delete it->second;
	}
	void learnTargetType(ATarget *aspell) {
		spells[aspell->getType()] = aspell->clone();
	}
	void forgetTargetType(const string &spell) {
		delete spells.find(spell)->second;
		spells.erase(spell);
	}
	ATarget *createTarget(const string &spell) {
		if (spells.find(spell) != spells.end())
			return spells[spell];
		return NULL;
	}
};

