#include <string>
#include <iostream>
using namespace std;

#include "crew.h"
#include "copilot.h"
#include "helper.h"

CoPilot::CoPilot() : Crew(CoPilot::TYPE) {}
CoPilot::CoPilot(const string &name) : Crew(name, CoPilot::TYPE) {}
CoPilot::CoPilot(const string &name, const string &rating) : Crew(name, CoPilot::TYPE), rating(rating) {}
CoPilot::CoPilot(int id, const string &name, const string &rating) : Crew(id, name, CoPilot::TYPE), rating(rating) {}

string CoPilot::getRating() const {
  return rating;
};

void CoPilot::setRating(const string &rating) {
  this->rating = rating;
  this->rating.resize(RATING_LENGTH, ' ');
}

ostream &CoPilot::print(ostream &out) const {
  Crew::print(out);
  out << "CoPilot Rating: " << rating << endl;
  return out;
}

bool CoPilot::interactiveEdit() {
  if (!Crew::interactiveEdit()) return false;
  rating = Helper::promptValueWithDefault("Rating", rating);
  return true;
}
