#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

#include "time_span.h"

class Aircraft {
private:
  string make = "";
  int model = 0;
  string tailNum = "";
  int numSeats = 0;
  int range = 0;
  int minCrew = 0;
  
public:
  //constructors including default and copy
  Aircraft() = default;
  Aircraft(const Aircraft&) = default;
  Aircraft(const string &make, int model, const string &tailNum, int numSeats, int range, int minCrew);
  //getters
  const string &getMake() const;
  int getModel() const;
  const string &getTailNum() const;
  int getNumSeats() const;
  int getRange() const;
  int getMinCrew() const;
  bool hasTailNum(const string &tailNum) const;
  //setters
  void setRange(int range);
  void setMinCrew(int minCrew);
  //show prompt to edit aircraft
  bool interactiveEdit();
  //validate changes to aircraft
  void valid() const;
  void validateRemoval() const;
  void print() const;
  //return if aircraft is in use during this timespan
  bool availableDuring(const TimeSpan &, int exceptFlightId = -1) const;
  //parse a line of text from the save file and return an aircraft
  static Aircraft fromLine(const string&);
  //convert an aircraft to a line in the save file
  const string toLine() const;
  // copy assignment operator
  Aircraft &operator=(const Aircraft&) = default;
  bool operator<(const Aircraft &c) const;
  bool operator==(const Aircraft &c) const;
  bool operator==(const string &) const;

  friend ostream &operator<<(ostream &out, const Aircraft &c);
  // converting an aircraft to a string returns tail number
  operator const string&() const;

};

ostream &operator<<(ostream &out, const Aircraft &c);

