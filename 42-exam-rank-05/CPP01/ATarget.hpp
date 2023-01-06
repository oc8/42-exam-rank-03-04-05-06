#pragma once

#include <iostream>

class ASpell;

using std::string;
using std::cout;
using std::endl;

class ATarget {
private:
	string type;

public:
	ATarget() {}
	ATarget(const ATarget &other) {*this = other;}
	ATarget &operator=(const ATarget &other) {
		type = other.type;
		return *this;
	}
	ATarget(string type): type(type) {}
	virtual ~ATarget() {}
	string getType() const {return type;}
	virtual ATarget *clone() const = 0;
	void getHitBySpell(const ASpell &other) const;
};

#include "ASpell.hpp"
