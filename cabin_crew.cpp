#include <map>
#include <string>
#include <iostream>
using namespace std;

#include "crew.h"
#include "cabin_crew.h"
#include "helper.h"

const map<CabinCrew::Position, string> CabinCrew::POSITION_TO_STRING {
  { FIRST_CLASS, "First Class" },
  { BUSINESS_CLASS, "Business Class" },
  { ECONOMY_FRONT, "Economy Front" },
  { ECONOMY_REAR, "Economy Rear" },
  { LEAD, "Lead" },
};

CabinCrew::CabinCrew() : Crew(CabinCrew::TYPE) {}
CabinCrew::CabinCrew(const string &name) : Crew(name, CabinCrew::TYPE) {}
CabinCrew::CabinCrew(const string &name, Position position) : Crew(name, CabinCrew::TYPE), position(position) {}
CabinCrew::CabinCrew(int id, const string &name, Position position) : Crew(id, name, CabinCrew::TYPE), position(position) {}

CabinCrew::CabinCrew(const vector<string> &parts) : Crew(parts) { 
  position = static_cast<Position>(stoi(parts[4]));
}

CabinCrew::Position CabinCrew::getPosition() const {
  return position;
};

ostream &CabinCrew::print(ostream &out) const {
  Crew::print(out);
  out << "Position: " << POSITION_TO_STRING.at(position) << endl;
  return out;
}

string CabinCrew::toLine() const {
  return Crew::toLine() + "," + to_string(static_cast<int>(position));
}
