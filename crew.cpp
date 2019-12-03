#include <set>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>

using namespace std;
using namespace std::placeholders;

#include "crew.h"
#include "crew_collection.h"
#include "flight.h"
#include "flight_collection.h"
#include "helper.h"


const map<string, Crew::Type> Crew::TYPE_MENU { 
  { "Pilot", Crew::Type::PILOT },
  { "Cabin", Crew::Type::CABIN }
};

const map<string, Crew::Availability> Crew::AVAILABILITY_MENU { 
  { "Available", Crew::Availability::AVAILABLE },
  { "Leave",     Crew::Availability::ON_LEAVE },
  { "Sick",      Crew::Availability::SICK }
};

const map<Crew::Availability, string> Crew::AVAILABILITY_TO_STRING = Helper::invertMap(Crew::AVAILABILITY_MENU);
const map<Crew::Type, string> Crew::TYPE_TO_STRING = Helper::invertMap(Crew::TYPE_MENU);

Crew::Crew() : id(crewCollection.nextId()) {}
Crew::Crew(Type type) : id(crewCollection.nextId()), type(type) {}
Crew::Crew(const string &name, Type type) : id(crewCollection.nextId()), name(name), type(type) {}
Crew::Crew(int id, const string &name, Type type) : id(id), name(name), type(type) {}

int Crew::getId() const {
  return id;
}

const string &Crew::getName() const {
  return name;
}

Crew::Type Crew::getType() const {
  return type;
}

bool Crew::hasType(Crew::Type type) const {
  return this->type == type;
}

void Crew::setName(const string &name) {
  this->name = name;
}

const string Crew::toLine() const {
  // Cannot return reference here because val gets deallocated when function toLine ends (therefore, a reference to the returned value would refer to something that no longer existed when the function returned and you tried to use it)
  string val = to_string(id) + ",";
  val += to_string(static_cast<int>(type)) + ",";
  val += name;
  return val;
}

bool Crew::operator<(const Crew &c) const {
  return static_cast<int>(*this) < static_cast<int>(c);
}

bool Crew::operator==(const Crew &c) const {
  //cout << "Comparing crew id #" << static_cast<int>(*this);
  //cout << "To ID #" <<  static_cast<int>(c) << endl;
  return static_cast<int>(*this) == static_cast<int>(c);
}

Crew::operator int() const {
  return id;
}

Crew::operator const string&() const {
  return name;
}

Crew Crew::fromLine(const string &line) {
  vector<string> parts = Helper::split(line);
  int id = stoi(parts[0]);
  int typeId = stoi(parts[1]);
  string name = parts[2];
  return Crew(id, name, static_cast<Type>(typeId));
}

bool Crew::interactiveEdit() { 
  cout << "Enter values for each of the following fields for this item." << endl;
  cout << "If you do not proivde a value, the default/old value (indicated in brackets '[]') will be used." << endl;
  name = Helper::promptValueWithDefault("Name", name);
  if (!saved) type = Helper::promptMenu(TYPE_MENU, "Type of Crew Member");
  Availability oldAvail = availability;
  try {
    availability = Helper::promptMenu(AVAILABILITY_MENU, "Availability of Crew Member");
    valid();
  } catch (const invalid_argument &e) {
    const string error(e.what());
    cout << "An error was detected in the data provided: " << error << endl;
    cout << "As a result, the changes just made were not saved.";
    availability = oldAvail;
    return false;
  }
  return true;
}

ostream &Crew::print(ostream &out) const {
  out << "Crew Member ID #: " << id << endl;
  out << "Crew Member Name: " << name << endl;
  out << "Crew Member Type: " << TYPE_TO_STRING.at(type) << endl;
  return out;
}

Crew::Availability Crew::getAvailability() const {
  return availability;
}

void Crew::setAvailability(Crew::Availability avail) {
  availability = avail;
}

bool Crew::isAvailable() const {
  return availability == Availability::AVAILABLE;
}

void Crew::print() const {
  print(cout);
}

void Crew::valid() const {
  const vector<reference_wrapper<Flight>> assignedFlights = flightCollection.find_all(*this);
  if (!isAvailable() && assignedFlights.size() > 0) {
    throw invalid_argument("This crew member is assigned to at least one flight. Unassign before marking unavailable.");
  }
}

void Crew::validateRemoval() const {
  const vector<reference_wrapper<Flight>> assignedFlights = flightCollection.find_all(*this);
  if (assignedFlights.size() > 0) {
    throw invalid_argument("This crew member is assigned to at least one flight. Unassign before deleting.");
  }
}

ostream &operator<<(ostream &out, const Crew &c) {
  return c.print(out);
}

// Returns if this crew member is booked on another flight during this timespan
bool Crew::freeDuring(const TimeSpan &ts, int exceptFlightId) const {
  vector<reference_wrapper<Flight>> flights = flightCollection.find_overlapping(ts);
  const function<bool(const Flight&)> has_crew_member = bind(mem_fn<bool(const Crew&) const>(&Flight::hasCrewMember), _1, *this);
  const function<bool(const Flight&)> doesnt_have_crew_member = bind(logical_not<bool>(), bind(has_crew_member, _1));
  auto it = remove_if(flights.begin(), flights.end(), doesnt_have_crew_member);
  flights.erase(it, flights.end());

  auto getFlightNum = mem_fn<int() const>(&Flight::getFlightNum);
  function<bool(const Flight&)> notExcepted = bind(not_equal_to<int>(), exceptFlightId, bind(getFlightNum, _1));
  return none_of(flights.begin(), flights.end(), notExcepted);
}

bool operator<(const std::reference_wrapper<const Crew> a, const std::reference_wrapper<const Crew> b) {
  return a.get() < b.get();
}
