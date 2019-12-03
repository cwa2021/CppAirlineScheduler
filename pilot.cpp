#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

#include "crew.h"
#include "pilot.h"
#include "helper.h"

Pilot::Pilot() : Crew(Pilot::TYPE) {}
Pilot::Pilot(const string &name) : Crew(name, Pilot::TYPE) {}
Pilot::Pilot(const string &name, const string &rating) : Crew(name, Pilot::TYPE), rating(rating) {}
Pilot::Pilot(int id, const string &name, const string &rating) : Crew(id, name, Pilot::TYPE), rating(rating) {}
Pilot::Pilot(const vector<string> &parts) : Crew(parts) { 
  setRating(parts[4]);
}

string Pilot::getRating() const {
  return rating;
};

void Pilot::setRating(const string &newRating) {
  rating = newRating;
  remove(rating.begin(), rating.end(), ',');
  rating.resize(RATING_LENGTH, ' ');
}

ostream &Pilot::print(ostream &out) const {
  Crew::print(out);
  out << "Pilot Rating: " << rating << endl;
  return out;
}

bool Pilot::interactiveEdit() {
  if (!Crew::interactiveEdit()) return false;
  string newRating = Helper::promptValueWithDefault("Rating", rating);
  setRating(newRating);

  return true;
}

string Pilot::toLine() const {
  return Crew::toLine() + "," + rating;
}
