#pragma once
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <algorithm>
#include <chrono>

using namespace std;

#include "time_span.h"
#include "aircraft.h"
#include "crew.h"

class FlightCollection;

class Flight {
public:
  // possible statuses for each flight
  enum class Status { 
    ACTIVE,
    COMPLETE,
    CANCELLED
  };
  // Flights are uniquely identified by the planeId and start time
  typedef const pair<const string, const typename TimeSpan::time_point> key;
  // map flight statuses to string representations
  static const map<Status, const string> FLIGHT_STATUS_STRINGS;
private:
  int flightNum = 0;
  string planeId = "";
  set<int> crewIds;
  TimeSpan timeSpan;
  string startAirport = "";
  string endAirport = "";
  int numPassengers = 0;
  Status status = Status::ACTIVE;
  bool saved = false;
  
public:
  // constructors, including default and copy
  Flight() = default;
  Flight(const Flight&) = default;
  Flight(int flightNum, const string &planeId, const set<int> &crewIds, const TimeSpan &timeSpan, const string &startAirport, const string &endAirport, int numPassengers, Status status);

  // getters
  int getFlightNum() const;
  const string &getPlaneId() const;
  const Aircraft &getAircraft() const;
  const set<int> &getCrewMemberIds() const;
  const set<reference_wrapper<const Crew>> getCrewMembers() const;
  const set<reference_wrapper<const Crew>> getCrewMembers(Crew::Type) const;
  const typename TimeSpan::time_point &getStartTime() const;
  const typename TimeSpan::time_point &getEndTime() const;
  const TimeSpan &getTimeSpan() const;
  const TimeSpan::duration getDuration() const;
  const string &getStartAirport() const;
  const string &getEndAirport() const;
  int getNumPassengers() const;
  Status getStatus() const;
  int getCrewMemberTypeCount(Crew::Type) const;
  int getRequiredCrewMemberTypeCount(Crew::Type) const;
  bool hasRequiredCrewMemberTypeCount() const;
  bool hasRequiredCrewMemberTypeCount(Crew::Type) const;

  // helper functions to see if flight matches certain conditions
  bool hasAircraft(const string &planeId) const;
  bool hasAircraft(const Aircraft &a) const;
  bool hasStatus(const Status) const;
  bool isOverEightHours() const;
  bool hasFlightNum(int) const;

  // setters
  void setPlaneId(const string &);
  void setStartTime(const typename TimeSpan::time_point&);
  void setEndTime(const typename TimeSpan::time_point&);
  void setTimeSpan(const TimeSpan&);
  void setStartAirport(const string &);
  void setEndAirport(const string &);
  void setNumPassengers(int);
  void setStatus(Status);
  void cancel();
  void complete();

  // add/remove to/from pilots/crew (mostly unusued)
  bool addCrewMember(const Crew &c);
  bool addCrewMember(int crewId);
  bool removeCrewMember(const Crew &c);
  bool removeCrewMember(int crewId);
  bool hasCrewMember(int crewId) const;
  bool hasCrewMember(const Crew &c) const;

  // ensures flight is valid
  void valid() const;
  // ensures flight can be removed (no op)
  void validateRemoval() const;
  // convert flight into a line that can be inserted in save file
  const string toLine() const;

  // interactive prompts to set/edit fields
  bool interactiveEdit();

  // copy-assign operator
  Flight &operator=(const Flight &f) = default;

  // not used 
  bool operator<(const Flight &f) const;
  // compare flights by number
  bool operator==(const Flight &f) const;
  // get flight num when casting to int
  operator int() const;
  // get flight num/start time pair, when casting to key type
  operator key() const;

  void print() const;
  friend ostream &operator<<(ostream &out, const Flight &f);
  

  // parse line from save file into flight
  static Flight fromLine(const string &line);
  friend FlightCollection;
};


ostream &operator<<(ostream &out, const Flight &f);


istream &operator>>(istream &in, set<int> &ints);
ostream &operator<<(ostream &out, const set<int> &ints);

bool operator<(const std::reference_wrapper<Flight> a, const std::reference_wrapper<Flight> b);
