#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;
using namespace std::placeholders;

#include "crew.h"
#include "crew_collection.h"
#include "pilot.h"
#include "co_pilot.h"
#include "cabin_crew.h"
#include "helper.h"

CrewCollection::~CrewCollection() {
  for(Crew *c : crew) delete c;
}

int CrewCollection::nextId() {
  return ++idCounter;
}

CrewCollection::CrewCollection() {
  vector<string> lines = Helper::readFile(FILENAME);
  transform(lines.begin(), lines.end(), back_inserter(crew), &Crew::fromLine);
  auto it = crew.rbegin();
  if (it != crew.rend()) idCounter = (*it)->getId();
}

bool CrewCollection::add() {
  Crew *newCrew = nullptr;
  Crew::Type type = Helper::promptMenu(Crew::TYPE_MENU, "Type of Crew Member");
  if (type == Crew::Type::PILOT) {
    newCrew = new Pilot();
  } else if (type == Crew::Type::CO_PILOT) {
    newCrew = new CoPilot();
  } else if (type == Crew::Type::CABIN) {
    newCrew = new CabinCrew();
  } else {
    throw invalid_argument("Invalid crew member type.");
  }
  if (newCrew->interactiveEdit()) return add(newCrew);
  return false;
}

bool CrewCollection::add(Crew* c) {
  c->valid();
  crew.push_back(c);
  c->saved = true;
  return true;
}


void CrewCollection::save() const {
  vector<string> lines;
  transform(crew.begin(), crew.end(), back_inserter(lines), mem_fn(&Crew::toLine));
  Helper::writeFile(FILENAME, lines);
}

void CrewCollection::print() const {
  cout << *this << endl;
}

bool CrewCollection::isEnd(typename vector<Crew*>::const_iterator it) const {
  return it == crew.end();
}


ostream &operator<<(ostream &out, const CrewCollection &cc) {
  for (const Crew *c : cc.crew) out << *c << endl;
  return out;
}

// finds crew
typename vector<Crew*>::iterator CrewCollection::find(int val) {
  auto dereferencer = bind<Crew&(Crew*)>(Crew::dereference, _1);
  auto getCrewId = mem_fn<int() const>(&Crew::getId);
  function<int(Crew*)> getIdForCrew = bind(getCrewId, bind(dereferencer, _1));
  function<bool(Crew*)> matches = bind(equal_to<int>(), bind(getIdForCrew, _1), val);
  return std::find_if(crew.begin(), crew.end(), matches);
}

typename vector<Crew*>::const_iterator CrewCollection::find(int val) const {
  auto dereferencer = bind<const Crew&(const Crew*)>(Crew::dereference, _1);
  function<bool(const Crew*)> matches = bind(equal_to<int>(), bind(dereferencer, _1), val);
  return std::find_if(crew.begin(), crew.end(), matches);
}

CrewCollection crewCollection;
