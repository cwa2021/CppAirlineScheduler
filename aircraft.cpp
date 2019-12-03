#include <set>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>

using namespace std;
using namespace std::placeholders;

#include "aircraft.h"
#include "aircraft_collection.h"
#include "flight.h"
#include "flight_collection.h"
#include "helper.h"

Aircraft::Aircraft(const string &make, int model, const string &tailNum, int numSeats, int range, int minCrew) : make(make), model(model), tailNum(tailNum), numSeats(numSeats), range(range), minCrew(minCrew) {}

const string &Aircraft::getMake() const { 
  return make;
}

int Aircraft::getModel() const {
  return model;
}

const string &Aircraft::getTailNum() const {
  return tailNum;
}

int Aircraft::getNumSeats() const {
  return numSeats;
}

int Aircraft::getRange() const {
  return range;
}

int Aircraft::getMinCrew() const {
  return minCrew;
}

void Aircraft::setRange(int range) {
  this->range = range;
}

//sets the min crew to the declared value
void Aircraft::setMinCrew(int minCrew) {
  this->minCrew = minCrew;
}

const string Aircraft::toLine() const {
  // Cannot return reference here because val gets deallocated when function toLine ends (therefore, a reference to the returned value would refer to something that no longer existed when the function returned and you tried to use it)
  string val = make + "," + to_string(model) + ",";
  val += tailNum + "," + to_string(numSeats) + ",";
  val += to_string(range) + "," + to_string(minCrew);
  return val;
}

bool Aircraft::operator<(const Aircraft &c) const {
  return static_cast<const string&>(*this) < static_cast<const string&>(c);
}

bool Aircraft::operator==(const Aircraft &c) const {
  return static_cast<const string&>(*this) == static_cast<const string&>(c);
}

bool Aircraft::operator==(const string &s) const {
  return static_cast<const string&>(*this) == s;
}

Aircraft::operator const string&() const {
  return tailNum;
}

// sets ints equal to strings that have been converted to ints?
Aircraft Aircraft::fromLine(const string &line) {
  vector<string> parts = Helper::split(line);
  string make = parts[0];
  int model = stoi(parts[1]);
  string tailNum = parts[2];
  int numSeats = stoi(parts[3]);
  int range = stoi(parts[4]);
  int minCrew = stoi(parts[5]);
  return Aircraft(make, model, tailNum, numSeats, range, minCrew);
}

bool Aircraft::interactiveEdit() {
  Aircraft oldAircraft = *this;
  cout << "Enter values for each of the following fields for this item." << endl;
  cout << "If you do not proivde a value, the default/old value (indicated in brackets '[]') will be used." << endl;
  bool isAlreadyAdded = aircraftCollection.exists(*this);
  make = Helper::promptValueWithDefault("Make", make);
  model = Helper::promptValueWithDefault("Model", model);
  if (!isAlreadyAdded) tailNum = Helper::promptValueWithDefault("Tail Number", tailNum);
  numSeats = Helper::promptValueWithDefault("Number of Seats", numSeats);
  range = Helper::promptValueWithDefault("Range (miles)", range);
  minCrew = Helper::promptValueWithDefault("Minimum Cabin Crew Size", minCrew);
  try { 
    valid();
  } catch (const invalid_argument& e) {
    const string error(e.what());
    cout << "An error was detected in the data provided: " << error << endl;
    cout << "As a result, the changes just made were not saved.";
    *this = oldAircraft;
    return false;
  }
  return true;
}

bool Aircraft::hasTailNum(const string &tailNum) const {
  return this->tailNum == tailNum;
}

void Aircraft::valid() const {
  bool isAlreadyAdded = aircraftCollection.exists(*this);
  int allowedTailNumMatches = isAlreadyAdded ? 1 : 0;
  int tailNumMatches = aircraftCollection.tailNumCount(tailNum);
  if (tailNumMatches > allowedTailNumMatches) throw invalid_argument("Another flight already has this tail number."); 
  auto flightValidFn = mem_fn(&Flight::valid);
  const vector<reference_wrapper<Flight>> assignedFlights = flightCollection.find_all(*this);
  for_each(assignedFlights.begin(), assignedFlights.end(), flightValidFn);
}

void Aircraft::validateRemoval() const {
  const vector<reference_wrapper<Flight>> assignedFlights = flightCollection.find_all(*this);
  if (assignedFlights.size() > 0) {
    throw invalid_argument("This aircraft is assigned to at least one flight. Unassign it before deleting.");
  }
}

ostream &operator<<(ostream &out, const Aircraft &c) {
  out << "Aircraft Make: " << c.make << endl;
  out << "Aircraft Model: " << c.model << endl;
  out << "Aircraft Tail Number: " << c.tailNum << endl;
  out << "Aircraft Number of Seats: " << c.numSeats << endl;
  out << "Aircraft Range (miles): " << c.range << endl;
  out << "Aircraft Min Cabin Crew Size: " << c.minCrew << endl;
  return out;
}

void Aircraft::print() const {
  cout << *this << endl;
}

// Returns the number of flights this aircraft is booked on during this timespan
bool Aircraft::availableDuring(const TimeSpan &ts, int exceptFlightId) const {
  const vector<reference_wrapper<Flight>> flights = flightCollection.find_overlapping(ts);
  auto getFlightNum = mem_fn<int() const>(&Flight::getFlightNum);
  function<bool(const Flight&)> notExcepted = bind(not_equal_to<int>(), exceptFlightId, bind(getFlightNum, _1));
  auto has_aircraft = mem_fn<bool(const Aircraft&) const>(&Flight::hasAircraft);
  function<bool(const Flight&)> has_this_aircraft = bind(has_aircraft, _1, *this);
  function<bool(const Flight&)> matches = bind(logical_and<bool>(), bind(has_this_aircraft, _1), bind(notExcepted, _1));

  return none_of(flights.begin(), flights.end(), matches);
}
