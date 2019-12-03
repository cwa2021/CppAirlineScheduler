#include <set>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>

using namespace std;
using namespace std::placeholders;

#include "aircraft_collection.h"
#include "aircraft.h"
#include "helper.h"

AircraftCollection aircraftCollection;


void AircraftCollection::add() {
  Aircraft newAircraft;
  newAircraft.interactiveEdit();
  aircraft.push_back(newAircraft);
}

void AircraftCollection::add(Aircraft &a) {
  a.valid();
  aircraft.push_back(a);
}

void AircraftCollection::load() {
  vector<string> lines = Helper::readFile(FILENAME);  
  transform(lines.begin(), lines.end(), inserter(aircraft, aircraft.end()), &Aircraft::fromLine);
}


void AircraftCollection::save() {
  vector<string> lines;
  transform(aircraft.begin(), aircraft.end(), back_inserter(lines), mem_fn(&Aircraft::toLine));
  Helper::writeFile(FILENAME, lines);
}

void AircraftCollection::print() const {
  cout << *this << endl;
}

ostream &operator<<(ostream &out, const AircraftCollection &ac) {
  for(const Aircraft &a : ac.aircraft) out << a << endl;
  return out;
}

bool AircraftCollection::isEnd(typename vector<Aircraft>::const_iterator it) {
  return it == aircraft.end();
}


int AircraftCollection::tailNumCount(const string& tailNum) const {
  auto hasTailNum = bind(mem_fn(&Aircraft::hasTailNum), _1, tailNum);
  return count_if(aircraft.begin(), aircraft.end(), hasTailNum);
}
