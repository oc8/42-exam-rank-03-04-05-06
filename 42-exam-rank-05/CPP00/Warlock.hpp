#pragma once

#include <iostream>

using std::string;
using std::cout;
using std::endl;

class Warlock {
private:
	Warlock() {}
	Warlock(Warlock const &other): name(other.name), title(other.title) {}
	Warlock &operator=(const Warlock &other) {title = other.title; name = other.name; return *this;}
	string name;
	string title;

public:
	Warlock(string name, string title): name(name), title(title) {cout << name << ": This looks like another boring day." << endl;}
	virtual ~Warlock() {cout << name << ": My job here is done!" << endl;}
	string getName() const {return name;}
	string getTitle() const {return title;}
	void setTitle(const string &str) {title = str;}
	void introduce() const {cout << name << ": I am " << name << ", " << title << "!" << endl;}
};
