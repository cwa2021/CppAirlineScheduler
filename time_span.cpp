#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <stdexcept>

using namespace std;

#include "time_span.h"

const string TimeSpan::TIME_FORMAT = "%Y-%m-%dT%H:%M:%S";
TimeSpan::TimeSpan(time_point start, time_point end) : start(start), end(end) {
  if (start > end) throw range_error("Start must be before end.");
}
TimeSpan::TimeSpan(time_point start, duration d) : TimeSpan(start, start + d) {}

const typename TimeSpan::time_point &TimeSpan::getStart() const {
  return start;
}
const typename TimeSpan::time_point &TimeSpan::getEnd() const {
  return end;
}
typename TimeSpan::duration TimeSpan::getDuration() const {
  return end - start;
}

void TimeSpan::setStart(const typename TimeSpan::time_point &start) {
  if (start > end) throw range_error("Start must be before end.");
  this->start = start;
}
  
void TimeSpan::setEnd(const typename TimeSpan::time_point &end) {
  if (start > end) throw range_error("Start must be before end.");
  this->end = end;
}

void TimeSpan::setDuration(const typename TimeSpan::duration &d) {
  if (d < std::chrono::seconds(0)) throw range_error("Duration cannot be negative.");
  this->end = start + d;
}

bool TimeSpan::before(const TimeSpan &other) const {
  return end <= other.start;
}

bool TimeSpan::before(const typename TimeSpan::time_point &other) const {
  return end <= other;
}

bool TimeSpan::after(const TimeSpan &other) const {
  return start >= other.end;
}

bool TimeSpan::after(const time_point &other) const {
  return start >= other;
}

bool TimeSpan::overlaps(const TimeSpan &other) const {
  return !before(other) && !after(other);
}

bool TimeSpan::overlaps(const typename TimeSpan::time_point &other) const {
  return !before(other) && !after(other);
}

ostream &operator<<(ostream &out, const typename TimeSpan::time_point &tp) {
  time_t time = TimeSpan::clock::to_time_t(tp);
  return out << put_time(localtime(&time), "%c");
}

ostream &operator<<(ostream &out, const TimeSpan &ts) {
  out << ts.start << " - " << ts.end;
  return out;
}

// Reads a timespan from two space-separated times
// each in the format YYYY-MM-DDTHH:MM:SS
istream &operator>>(istream &in, TimeSpan& ts) {
  tm start_tm, end_tm;
  // TODO: check for invalid entries
  in >> get_time(&start_tm, TimeSpan::TIME_FORMAT.c_str());
  in >> get_time(&end_tm, TimeSpan::TIME_FORMAT.c_str());
  time_t start_t, end_t;
  start_t = mktime(&start_tm);
  end_t = mktime(&end_tm);
  ts.start = TimeSpan::clock::from_time_t(start_t);
  ts.end = TimeSpan::clock::from_time_t(end_t);
  return in;
}
