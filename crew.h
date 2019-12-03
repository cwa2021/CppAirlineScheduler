#pragma once
#include <set>
#include <map>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

#include "time_span.h"
class Flight;
class CrewCollection;

class Crew {
public:
  enum Type { PILOT, CO_PILOT, CABIN };
  enum Availability { AVAILABLE, ON_LEAVE, SICK };
  //map string representation of crew type to Type enum
  static const map<string, Type> TYPE_MENU;
  static const map<string, Availability> AVAILABILITY_MENU;
  static const map<Availability, string> AVAILABILITY_TO_STRING;
  static const map<Type, string> TYPE_TO_STRING;
private:
  int id = 0;
  string name = "";
  Type type = Type::PILOT;
  Availability availability = Availability::AVAILABLE;
  bool saved = false;

public:
  Crew(const Crew&) = default;
  Crew(Crew&&) = default;
protected:
  //constructors including default and copy
  Crew();
  Crew(Type type);
  Crew(const string &name, Type type);
protected:
  // private constructor to be used when loading file that allows specifying an ID
  Crew(int id, const string &name, Type type);
public:
  //getters
  int getId() const;
  const string &getName() const;
  Type getType() const;
  bool hasType(Type) const;
  Availability getAvailability() const;
  bool isAvailable() const;
  //setters
  void setName(const string &name);
  void setAvailability(Availability);

  // check if this crew member has been booked on another flight during a given timespan
  bool freeDuring(const TimeSpan &ts, int exceptFlightId = -1) const;

  // converts crew instance to string for save file
  virtual const string toLine() const;

  // less than operator (order by id#)
  bool operator<(const Crew&) const;
  // check if crew id # is equal
  bool operator==(const Crew &c) const;

  // interactive prompt to create/edit a crew member
  virtual bool interactiveEdit();
  // determine the validity of crew information or if the member can be safely removed
  virtual void valid() const;
  virtual void validateRemoval() const;
  virtual ostream &print(ostream&) const;
  void print() const;
  friend ostream &operator<<(ostream &out, const Crew &c);
  
  // copy-assign operator
  virtual Crew &operator=(const Crew&) = default;
  // allows casting crew instances to ints (returning their id#)
  operator int() const;
  // allows casting crew instances to strings (returning their name)
  operator const string&() const;

  // parses line from save file into crew instance
  static Crew fromLine(const string &line);

  // Friends to allow instantiating this object
  friend CrewCollection;
};

ostream &operator<<(ostream &out, const Crew &c);

// unsure if used: allows crew members to be compared even if enclosed in reference wrappers
bool operator<(const std::reference_wrapper<const Crew> a, const std::reference_wrapper<const Crew> b);


