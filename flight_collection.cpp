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

FlightCollection flightCollection;

bool FlightCollection::add() {
  Flight newFlight;
  if (newFlight.interactiveEdit()) return add(newFlight);
  return false;
}

bool FlightCollection::add(Flight &f) {
  f.valid();
  flights.push_back(f);
  f.saved = true;
  return true;
}

void FlightCollection::print() const {
  cout << *this << endl;
}

bool FlightCollection::isEnd(typename vector<Flight>::const_iterator it) const {
  return it == flights.end();
}

ostream &operator<<(ostream &out, const FlightCollection &fc) {
  for (const Flight &f : fc.flights) out << f << endl;
  return out;
}

void FlightCollection::load() {
  vector<string> lines = Helper::readFile(FILENAME);
  transform(lines.begin(), lines.end(), inserter(flights, flights.end()), &Flight::fromLine);
}

void FlightCollection::save() const {
  vector<string> lines;
  transform(flights.begin(), flights.end(), back_inserter(lines), mem_fn(&Flight::toLine));
  Helper::writeFile(FILENAME, lines);
}

const vector<reference_wrapper<Flight>> FlightCollection::find_all(const Aircraft &a) {
  auto unbound_predicate = mem_fn<bool(const Aircraft&) const>(&Flight::hasAircraft);
  auto predicate = bind(unbound_predicate, _1, a);
  vector<reference_wrapper<Flight>> matches;
  copy_if(flights.begin(), flights.end(), inserter(matches, matches.end()), predicate);
  return matches;
}

const vector<reference_wrapper<Flight>> FlightCollection::find_all(const Crew &c) {
  function<bool(const Flight&)> predicate = bind(mem_fn<bool(const Crew&) const>(&Flight::hasCrewMember), _1, c);
  vector<reference_wrapper<Flight>> matches;
  copy_if(flights.begin(), flights.end(), inserter(matches, matches.end()), predicate);
  return matches;
}

const vector<reference_wrapper<Flight>> FlightCollection::find_all(const Flight::Status s) {
  auto predicate = bind(mem_fn<bool(const Flight::Status) const>(&Flight::hasStatus), _1, s);
  vector<reference_wrapper<Flight>> matches;
  copy_if(flights.begin(), flights.end(), inserter(matches, matches.end()), predicate);
  return matches;
}

// update flights from active to completed based on time
int FlightCollection::updateCompletedFlights() {
  auto timespan_before_fn = bind(mem_fn<bool(const TimeSpan::time_point&) const>(&TimeSpan::before), _1, TimeSpan::clock::now());
  auto get_timespan_fn = mem_fn(&Flight::getTimeSpan);
  auto completed_predicate = bind(timespan_before_fn, bind(get_timespan_fn, _1));
  auto status_predicate = bind(mem_fn<bool(Flight::Status) const>(&Flight::hasStatus), _1, Flight::Status::COMPLETE);
  auto mark_completed = bind(mem_fn(&Flight::complete), _1);

  int count = 0;
  auto process_flight = [&] (Flight &f) -> void {
    if (completed_predicate(f) && status_predicate(f)) {
      mark_completed(f);
      ++count;
    }
  };
  //set<reference_wrapper<const Flight>> completed_flights, matches;
  //copy_if(flights.begin(), flights.end(), inserter(completed_flights, completed_flights.end()), completed_predicate);
  //copy_if(completed_flights.begin(), completed_flights.end(), inserter(matches, matches.end()), status_predicate);
  //for_each(matches.begin(), matches.end(), mark_completed);
  for_each(flights.begin(), flights.end(), process_flight);
  return count;
}

int FlightCollection::removeCancelled() {
  int oldSize = flights.size();
  function<bool(const Flight&, Flight::Status)> flight_has_status = mem_fn<bool(Flight::Status) const>(&Flight::hasStatus);
  function<bool(const Flight&)> status_predicate = bind(flight_has_status, _1, Flight::Status::CANCELLED);
  remove_if(flights.begin(), flights.end(), status_predicate);
  return oldSize - flights.size();
}

int FlightCollection::removeCompleted() {
  int oldSize = flights.size();
  auto status_predicate = bind(mem_fn(&Flight::hasStatus), _1, Flight::Status::COMPLETE);
  remove_if(flights.begin(), flights.end(), status_predicate);
  return oldSize - flights.size();
}

int FlightCollection::removeCancelledAndCompleted() {
  return removeCancelled() + removeCompleted();
}

const vector<reference_wrapper<Flight>> FlightCollection::find_overlapping(const TimeSpan &ts) {
  auto overlaps_fn = bind(mem_fn<bool(const TimeSpan&) const>(&TimeSpan::overlaps), ts, _1);
  auto get_timespan_fn = mem_fn(&Flight::getTimeSpan);
  auto predicate = bind(overlaps_fn, bind(get_timespan_fn, _1));
  vector<reference_wrapper<Flight>> result;
  copy_if(flights.begin(), flights.end(), inserter(result, result.end()), predicate);
  return result;
}

int FlightCollection::flightNumCount(int flightNum) const {
  auto hasFlightNum = bind(mem_fn(&Flight::hasFlightNum), _1, flightNum);
  return count_if(flights.begin(), flights.end(), hasFlightNum);
}
