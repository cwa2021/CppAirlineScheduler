#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;

#include "crew.h"
#include "crew_collection.h"
#include "helper.h"

int CrewCollection::nextId() {
  return ++idCounter;
}

bool CrewCollection::add() {
  Crew newCrew;
  if (newCrew.interactiveEdit()) return add(newCrew);
  return false;
}

bool CrewCollection::add(Crew &c) {
  c.valid();
  crew.push_back(c);
  c.saved = true;
  return true;
}

void CrewCollection::load() {
  vector<string> lines = Helper::readFile(FILENAME);
  transform(lines.begin(), lines.end(), back_inserter(crew), &Crew::fromLine);
  auto it = crew.rbegin();
  if (it != crew.rend()) idCounter = it->getId();
}


void CrewCollection::save() const {
  vector<string> lines;
  transform(crew.begin(), crew.end(), back_inserter(lines), mem_fn(&Crew::toLine));
  Helper::writeFile(FILENAME, lines);
}

void CrewCollection::print() const {
  cout << *this << endl;
}

bool CrewCollection::isEnd(typename vector<Crew>::const_iterator it) const {
  return it == crew.end();
}


ostream &operator<<(ostream &out, const CrewCollection &cc) {
  for (const Crew &c : cc.crew) out << c << endl;
  return out;
}

// finds crew
typename vector<Crew>::iterator CrewCollection::find(int val) {
  return std::find(crew.begin(), crew.end(), val);
}

typename vector<Crew>::const_iterator CrewCollection::find(int val) const {
  return std::find(crew.begin(), crew.end(), val);
}

CrewCollection crewCollection;
