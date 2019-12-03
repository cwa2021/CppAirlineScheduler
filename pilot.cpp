#include <string>
#include <iostream>
using namespace std;

#include "crew.h"
#include "pilot.h"
#include "helper.h"

Pilot::Pilot() : Crew(Pilot::TYPE) {}
Pilot::Pilot(const string &name) : Crew(name, Pilot::TYPE) {}
Pilot::Pilot(const string &name, const string &rating) : Crew(name, Pilot::TYPE), rating(rating) {}
Pilot::Pilot(int id, const string &name, const string &rating) : Crew(id, name, Pilot::TYPE), rating(rating) {}

string Pilot::getRating() const {
  return rating;
};

void Pilot::setRating(const string &rating) {
  this->rating = rating;
  this->rating.resize(RATING_LENGTH, ' ');
}

ostream &Pilot::print(ostream &out) const {
  Crew::print(out);
  out << "Pilot Rating: " << rating << endl;
  return out;
}

bool Pilot::interactiveEdit() {
  if (!Crew::interactiveEdit()) return false;
  rating = Helper::promptValueWithDefault("Rating", rating);
  return true;
}

