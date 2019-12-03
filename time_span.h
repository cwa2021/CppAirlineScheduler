#pragma once
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

using namespace std;

class TimeSpan {
public:
  // Defines a time format string for parsing data inputted from command line
  // See: https://en.cppreference.com/w/cpp/io/manip/get_time
  static const string TIME_FORMAT;
  // Define simpler type names for classes inside std::chrono
  typedef chrono::high_resolution_clock clock;
  typedef clock::duration duration;
  typedef clock::time_point time_point; 
  // Define a type to represent lengths of time in units of (float) hours
  // https://en.cppreference.com/w/cpp/chrono/duration#Example
  typedef chrono::duration<float, ratio<3600>> hours;
private:
  // A time span has a start and end point
  time_point start, end;
public:
  // Default constructor -- start and end will be at midnight on 1/1/1970
  TimeSpan() = default;
  // A timespan can be can be constructed from a start time and either an end time or duration
  TimeSpan(time_point start, time_point end);
  TimeSpan(time_point start, duration d);

  // getters
  const time_point &getStart() const;
  const time_point &getEnd() const;
  duration getDuration() const;

  // setters
  void setStart(const time_point &start);
    
  void setEnd(const time_point &end);

  void setDuration(const duration &d);

  // checks if a timespan occurs before another span or point
  bool before(const TimeSpan &other) const;

  bool before(const time_point &other) const;

  // checks if a timespan occurs after another span or point
  bool after(const TimeSpan &other) const;
  
  bool after(const time_point &other) const;

  // return if a timespan overlaps with another span/point in any way
  bool overlaps(const TimeSpan &other) const;

  bool overlaps(const time_point &other) const;

  friend ostream &operator<<(ostream&, const TimeSpan&);
  // Allows for parsing time spans from strings given in the TIME_FORMAT specified above
  friend istream &operator>>(istream&, TimeSpan&);
};

ostream &operator<<(ostream &out, const typename TimeSpan::time_point &tp);
ostream &operator<<(ostream &out, const TimeSpan &ts);
istream &operator>>(istream &in, TimeSpan& ts);
