#include <set> 
#include <string>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <functional>

using namespace std;
using namespace std::placeholders;

#include "helper.h"
#include "aircraft_collection.h"
#include "crew.h"
#include "crew_collection.h"
#include "flight.h"
#include "flight_collection.h"

using namespace Helper;

//enum class Flight::Status 
//};
const map<typename Flight::Status, const string> Flight::FLIGHT_STATUS_STRINGS {
  { Flight::Status::ACTIVE,    "Active"   },
  { Flight::Status::COMPLETE,  "Complete" },
  { Flight::Status::CANCELLED, "Active"   },
};


Flight::Flight(int flightNum, const string &planeId, 
  const set<int> &crewIds, const TimeSpan &timeSpan, 
  const string &startAirport, const string &endAirport, int numPassengers, Status status) : 
    flightNum(flightNum), planeId(planeId), timeSpan(timeSpan), 
    startAirport(startAirport), endAirport(endAirport),
    numPassengers(numPassengers), status(status) {

  for (int crewId : crewIds) addCrewMember(crewId);
}

int Flight::getFlightNum() const {
  return flightNum;
} 

bool Flight::hasFlightNum(int flightNum) const {
  return flightNum == this->flightNum;
}

const string &Flight::getPlaneId() const {
  return planeId;
} 

const set<int> &Flight::getCrewMemberIds() const {
  return crewIds;
}

const TimeSpan::time_point &Flight::getStartTime() const {
  return timeSpan.getStart();
}

const TimeSpan::time_point &Flight::getEndTime() const {
  return timeSpan.getEnd();
}

const TimeSpan &Flight::getTimeSpan() const { 
  return timeSpan;
}

const string &Flight::getStartAirport() const {
  return startAirport;
}

const string &Flight::getEndAirport() const {
  return endAirport;
}

int Flight::getNumPassengers() const {
  return numPassengers;
}

typename Flight::Status Flight::getStatus() const {
  return status;
}

const Aircraft &Flight::getAircraft() const {
  return *aircraftCollection.find(planeId);
}

int Flight::getCrewMemberTypeCount(Crew::Type crewType) const {
  return getCrewMembers(crewType).size();
}

bool Flight::isOverEightHours() const {
  typename TimeSpan::hours numHours(timeSpan.getDuration());
  return numHours.count() > 8.0;
}

// a ? b : c === if(a) { b } else { c }
int Flight::getRequiredCrewMemberTypeCount(Crew::Type crewType) const {
  if (crewType == Crew::Type::PILOT || crewType == Crew::Type::CO_PILOT) {
    return !isOverEightHours() ? 1 : 2;
  } else {
    return !isOverEightHours() ? getAircraft().getMinCrew() : (getAircraft().getMinCrew() * 2);
  }
}

bool Flight::hasRequiredCrewMemberTypeCount(Crew::Type crewType) const {
  return getRequiredCrewMemberTypeCount(crewType) >= getCrewMemberTypeCount(crewType);
}

bool Flight::hasRequiredCrewMemberTypeCount() const {
  auto meetsRequirements = mem_fn<bool(const Crew::Type) const>(&Flight::hasRequiredCrewMemberTypeCount);
  auto meetsTypeRequirements = bind(meetsRequirements, *this, _1);
  auto getKey = mem_fn(&pair<Crew::Type, string>::first);
  function <bool(const pair<Crew::Type, string>)> typeMeetsRequirements = bind(meetsTypeRequirements, bind(getKey, _1));
  return all_of(Crew::TYPE_TO_STRING.begin(), Crew::TYPE_TO_STRING.end(), typeMeetsRequirements);
}

const set<reference_wrapper<const Crew>> Flight::getCrewMembers() const {
  set<reference_wrapper<const Crew>> crew;
  for (int crewId : crewIds) {
    auto crewIt = crewCollection.find(crewId);
    if (crewCollection.isEnd(crewIt)) throw invalid_argument("A crew member with this ID does not exist.");
    crew.insert(cref(*crewIt));
  }
  return crew;
}
const set<reference_wrapper<const Crew>> Flight::getCrewMembers(Crew::Type crewType) const {
  const set<reference_wrapper<const Crew>> allCrew = getCrewMembers();
  function<bool(const Crew&)> predicate = bind(mem_fn(&Crew::hasType), _1, crewType);

  set<reference_wrapper<const Crew>> crew;
  copy_if(allCrew.begin(), allCrew.end(), inserter(crew, crew.end()), predicate);
  return crew;
}
bool Flight::hasAircraft(const string &planeId) const {
  return planeId == this->planeId;
}

bool Flight::hasAircraft(const Aircraft &a) const {
  return hasAircraft(a.getTailNum());
}

bool Flight::hasStatus(const Status status) const {
  return this->status == status;
}
void Flight::setPlaneId(const string &planeId) {
  this->planeId = planeId;
}

void Flight::setStartTime(const typename TimeSpan::time_point& start) {
  timeSpan.setStart(start);
}

void Flight::setEndTime(const typename TimeSpan::time_point& end) {
  timeSpan.setEnd(end);
}

void Flight::setTimeSpan(const TimeSpan &timeSpan) {
  this->timeSpan = timeSpan;
}

void Flight::setStartAirport(const string &startAirport) {
  this->startAirport = startAirport;
}

void Flight::setEndAirport(const string &endAirport) {
  this->endAirport = endAirport;
}

void Flight::setNumPassengers(int numPassengers) {
  this->numPassengers = numPassengers;
}

void Flight::setStatus(typename Flight::Status status) {
  this->status = status;
}

void Flight::cancel() {
  if (status == Status::COMPLETE) throw invalid_argument("Cannot cancel a flight that has completed.");
  status = Status::CANCELLED; 
}

void Flight::complete() {
  if (status == Status::CANCELLED) throw invalid_argument("Cannot cancel a flight that has been cancelled.");
  status = Status::COMPLETE; 
}

bool Flight::addCrewMember(const Crew &c) {
  if (!crewCollection.exists(c)) throw invalid_argument("Crew member ID is not valid.");
  auto result = crewIds.insert(c.getId());
  return result.second;
}

bool Flight::addCrewMember(int crewId) {
  vector<Crew>::iterator crewIt = crewCollection.find(crewId);
  if (crewCollection.isEnd(crewIt)) throw invalid_argument("Crew member ID is not valid.");
  Crew &c = *crewIt;
  return addCrewMember(c);
}

bool Flight::removeCrewMember(const Crew &c) {
  return removeCrewMember(c.getId());
}

bool Flight::removeCrewMember(int crewId) {
  auto it = crewIds.find(crewId);
  if (it == crewIds.end()) return false;
  crewIds.erase(it);
  return true;
}

bool Flight::hasCrewMember(int crewId) const {
  return crewIds.find(crewId) != crewIds.end();
}

bool Flight::hasCrewMember(const Crew &c) const {
  return hasCrewMember(c.getId());
}

const string Flight::toLine() const {
  string str;
  str += to_string(flightNum) + ",";
  str += planeId + ",";
  str += Helper::ints_to_string(crewIds.begin(), crewIds.end()) + ",";
  str += to_string(Helper::timepoint_to_int(timeSpan.getStart())) + ",";
  str += to_string(Helper::timepoint_to_int(timeSpan.getEnd())) + ",";
  str += startAirport + ",";
  str += endAirport + ",";
  str += to_string(numPassengers) + ",";
  str += to_string(static_cast<int>(status));
  return str;
}

bool Flight::operator<(const Flight &f) const {
  return static_cast<key>(*this) < static_cast<key>(f);
}

bool Flight::operator==(const Flight &f) const {
  return static_cast<key>(*this) == static_cast<key>(f);
}

Flight::operator typename Flight::key() const {
  return pair<string, TimeSpan::time_point>(planeId, timeSpan.getStart());
}
  
Flight::operator int() const {
  return flightNum;
}

void Flight::valid() const {
  // confirm the flight number isn't duplicated
  if (!saved && flightCollection.exists(flightNum)) throw invalid_argument("Another flight already has this tail number.");  

  // confirm aircraft exists
  if (!aircraftCollection.exists(planeId)) throw invalid_argument("Plane ID not valid.");
  const Aircraft &aircraft = getAircraft();

  // confirm aircraft is available
  if (!aircraft.availableDuring(timeSpan, flightNum)) throw invalid_argument("The selected aircraft is not available during this time span.");

  // make sure cabin/crew members are available during flight
  set<reference_wrapper<const Crew>> crew = getCrewMembers();
  function<bool(const Crew&)> crewOverbooked = bind(mem_fn(&Crew::freeDuring), _1, timeSpan, flightNum);
  if (!all_of(crew.begin(), crew.end(), crewOverbooked)) throw invalid_argument("One or more crew members are not free during the flight.");
  if (!all_of(crew.begin(), crew.end(), mem_fn(&Crew::isAvailable))) throw invalid_argument("One or more crew members are not currently working.");

  if (hasRequiredCrewMemberTypeCount()) throw invalid_argument("There aren't enough crew members assigned for a flight of this length.");
  if (numPassengers < 0) throw invalid_argument("Number of passengers may not be negative.");
  if (numPassengers > aircraft.getNumSeats()) throw invalid_argument("Plane has insufficent capacity to support number of passengers.");
}

void Flight::validateRemoval() const {}

Flight Flight::fromLine(const string &line) {
  vector<string> parts = Helper::split(line);
  int flightNum = stoi(parts[0]);
  string planeId = parts[1];
  string crewIdsStr = parts[2];
  int startTimestamp = stoi(parts[4]);
  int endTimestamp = stoi(parts[5]);
  string startAirport = parts[6];
  string endAirport = parts[7];
  int numPassengers = stoi(parts[8]);
  int statusNum = stoi(parts[9]);

  vector<int> crewIdsVector = Helper::string_to_ints(crewIdsStr);
  set<int> crewIds(crewIdsVector.begin(), crewIdsVector.end());
  TimeSpan::time_point startTime = Helper::int_to_timepoint(startTimestamp);
  TimeSpan::time_point endTime = Helper::int_to_timepoint(endTimestamp);
  TimeSpan timeSpan(startTime, endTime);
  Status status = static_cast<Status>(statusNum);
  return Flight(flightNum, planeId, crewIds, timeSpan, startAirport, endAirport, numPassengers, status);
}

bool Flight::interactiveEdit() {
  Flight oldFlight = *this;
  cout << "Enter values for each of the following fields for this item." << endl;
  cout << "If you do not proivde a value, the default/old value (indicated in brackets '[]') will be used." << endl;
  flightNum = Helper::promptValueWithDefault("Flight Number", flightNum);
  planeId = Helper::promptValueWithDefault("Plane ID", planeId);
  startAirport = Helper::promptValueWithDefault("Start Airport", startAirport);
  endAirport = Helper::promptValueWithDefault("End Airport", endAirport);
  numPassengers = Helper::promptValueWithDefault("Number of Passengers", numPassengers);
  timeSpan = Helper::promptValueWithDefault("Start/End Time of Flight (in the format 'YYYY-MM-DDTHH:MM:SS YYYY-MM-DDTHH:MM:SS')", timeSpan);
  crewIds = Helper::promptValueWithDefault("Crew Member IDs (comma separated; no spaces)", crewIds);
  try { 
    valid();
  } catch (const invalid_argument& e) {
    const string error(e.what());
    cout << "An error was detected in the data provided: " << error << endl;
    cout << "As a result, the changes just made were not saved.";
    *this = oldFlight;
    return false;
  }
  return true;
}

void Flight::print() const {
  cout << *this << endl;
}

ostream &operator<<(ostream &out, const Flight &f) {
  out << "Flight Number: " << f.flightNum << endl;
  out << "Flight Plane ID: " << f.planeId << endl;
  out << "Flight Crew Member IDs: " << Helper::ints_to_string(f.crewIds.begin(), f.crewIds.end(), ", ") << endl;
  out << "Flight Times: " << f.timeSpan << endl;
  out << "Flight Start Airport: " << f.startAirport << endl;
  out << "Flight End Airport: " << f.endAirport << endl;
  out << "Flight Num Passengers: " << f.numPassengers << endl;
  out << "Flight Status: " << Flight::FLIGHT_STATUS_STRINGS.at(f.status) << endl;
  return out;
}

istream &operator>>(istream &in, set<int> &ints) {
  string int_string;
  in >> int_string;
  if (int_string == "") return in;
  vector<int> int_vector = Helper::string_to_ints(int_string, ",");
  ints = set<int>(int_vector.begin(), int_vector.end());
  return in;
}

ostream &operator<<(ostream &out, const set<int> &ints) {
  out << Helper::ints_to_string(ints.begin(), ints.end(), ",");
  return out;
}

bool operator<(const std::reference_wrapper<Flight> a, const std::reference_wrapper<Flight> b) {
  return a.get() < b.get();
}
