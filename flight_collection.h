#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <chrono>

using namespace std;

#include "time_span.h"
#include "aircraft.h"

class Crew;

class FlightCollection {
  vector<Flight> flights;
  const string FILENAME = "flights.txt";
public:
  

  bool add();
  bool add(Flight &f);
  template<typename T>
  typename vector<Flight>::iterator find(T val);
  template<typename T>
  typename vector<Flight>::const_iterator find(T val) const;
  template<typename T>
  bool exists(T val) const;
  // helps with checking for duplicate flight numbers by returning the # of elements that have a given flight number
  int flightNumCount(int) const;
  // checks if argument is the end iterator
  bool isEnd(typename vector<Flight>::const_iterator) const;
  template<typename T>
  bool remove(T val);
  // flinds any flights that conflict with the given timespan
  const vector<reference_wrapper<Flight>> find_overlapping(const TimeSpan &ts);
  // lists all flights that have a given crew member/aircraft assigned, or have a given status
  const vector<reference_wrapper<Flight>> find_all(const Aircraft &a);
  const vector<reference_wrapper<Flight>> find_all(const Crew &c);
  const vector<reference_wrapper<Flight>> find_all(const Flight::Status s);
  // uses the relevant find_all function to print a specific list of flights
  template <typename T>
  void print_all(T val);
  // mark completed flights completed
  int updateCompletedFlights();
  // remove cancelled flights
  int removeCancelled();
  // remove completed flights
  int removeCompleted();
  // remove cancelled and completed flights
  int removeCancelledAndCompleted();

  void print() const;

  // load and save flights to/from file
  void load();
  void save() const;
  friend ostream &operator<<(ostream &out, const FlightCollection &f);
};

extern FlightCollection flightCollection;

ostream &operator<<(ostream &out, const Flight &f);
// returns found flight
template<typename T>
typename vector<Flight>::iterator FlightCollection::find(T val) {
  return std::find(flights.begin(), flights.end(), val);
}
template<typename T>
typename vector<Flight>::const_iterator FlightCollection::find(T val) const {
  return std::find(flights.begin(), flights.end(), val);
}
// attempts to find flight specified by val, and if found, removes it
template<typename T>
bool FlightCollection::remove(T val) {
  auto it = find(val);
  if (it == flights.end()) return false;
  try { 
    it->validateRemoval();
  } catch (const invalid_argument& e) {
    const string error(e.what());
    cout << "An error was encountered in performing this operation: " << error << endl;
    cout << "As a result, the deletion requested failed to execute.";
    return false;
  }
  flights.erase(it);
  return true;
}

// returns boolean for if flight was found 
template<typename T>
bool FlightCollection::exists(T val) const {
  return find(val) != flights.end();
}

template <typename T>
void FlightCollection::print_all(T val) {
  const vector<reference_wrapper<Flight>> flights = find_all(val);
  for (const Flight &f : flights) cout << f << endl;
}
