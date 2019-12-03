#include <string>
#include <iostream>
using namespace std;

#include "crew.h"
#include "cabin_crew.h"
#include "helper.h"

CabinCrew::CabinCrew() : Crew(CabinCrew::TYPE) {}
CabinCrew::CabinCrew(const string &name) : Crew(name, CabinCrew::TYPE) {}
CabinCrew::CabinCrew(const string &name, Position position) : Crew(name, CabinCrew::TYPE), position(position) {}
CabinCrew::CabinCrew(int id, const string &name, Position position) : Crew(id, name, CabinCrew::TYPE), position(position) {}

CabinCrew::Position CabinCrew::getPosition() const {
  return position;
};

ostream &CabinCrew::print(ostream &out) const {
  Crew::print(out);
  out << "Position: " << position << endl;
  return out;
}
