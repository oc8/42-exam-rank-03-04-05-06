#pragma once

#include <iostream>
#include <map>

#include "ASpell.hpp"

using std::string;
using std::cout;
using std::endl;
using std::map;

class Warlock {
private:
	Warlock() {}
	Warlock(Warlock const &other): name(other.name), title(other.title) {}
	Warlock &operator=(const Warlock &other) {title = other.title; name = other.name; return *this;}
	string name;
	string title;
	map<string, ASpell *> spells;

public:
	Warlock(string name, string title): name(name), title(title) {cout << name << ": This looks like another boring day." << endl;}
	virtual ~Warlock() {cout << name << ": My job here is done!" << endl;}
	string getName() const {return name;}
	string getTitle() const {return title;}
	void setTitle(const string &str) {title = str;}
	void introduce() const {cout << name << ": I am " << name << ", " << title << "!" << endl;}
	void learnSpell(ASpell *aspell) {
		spells[aspell->getName()] = aspell;
	}
	void forgetSpell(string spell) {
		spells.erase(spell);
	}
	void launchSpell(string spell, ATarget &target) {
		if (spells[spell])
			spells[spell]->launch(target);
	}
};
