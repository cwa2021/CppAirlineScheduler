#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

#include "crew.h"
#include "co_pilot.h"
#include "helper.h"

CoPilot::CoPilot() : Crew(CoPilot::TYPE) {}
//CoPilot::CoPilot(const string &name) : Crew(name, CoPilot::TYPE) {}
//CoPilot::CoPilot(const string &name, const string &rating) : Crew(name, CoPilot::TYPE), rating(rating) {}
//CoPilot::CoPilot(int id, const string &name, const string &rating) : Crew(id, name, CoPilot::TYPE), rating(rating) {}
CoPilot::CoPilot(const vector<string> &parts) : Crew(parts) { 
  setRating(parts[4]);
}

string CoPilot::getRating() const {
  return rating;
};

void CoPilot::setRating(const string &newRating) {
  rating = newRating;
  remove(rating.begin(), rating.end(), ',');
  rating.resize(RATING_LENGTH, ' ');
}

ostream &CoPilot::print(ostream &out) const {
  Crew::print(out);
  out << "CoPilot Rating: " << rating << endl;
  return out;
}

bool CoPilot::interactiveEdit() {
  if (!Crew::interactiveEdit()) return false;
  string newRating = Helper::promptValueWithDefault("Rating", rating);
  setRating(newRating);
  return true;
}

string CoPilot::toLine() const {
  return Crew::toLine() + "," + rating;
}
