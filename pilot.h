#pragma once

#include<string>
#include<vector>
using namespace std;

#include "crew.h"

class Pilot : public Crew {
public: 
  static const Crew::Type TYPE = Crew::Type::PILOT;
  static const int RATING_LENGTH = 5;

protected:
  string rating{RATING_LENGTH, ' '};
  Pilot(int id, const string &name, const string &rating);
  Pilot(const vector<string> &parts);

public:
  Pilot();
  ~Pilot() = default;
  Pilot(const string &name);
  Pilot(const string &name, const string &rating);
  Pilot(Pilot&&) = delete;
  Pilot(const Pilot&) = default;

public:
  string getRating() const;
  void setRating(const string &rating);

  ostream &print(ostream&) const override;
  bool interactiveEdit() override;
  string toLine() const override;
  Pilot &operator=(const Pilot&) = default;

  // Friends to allow instantiating this object
  friend CrewCollection;
  friend Crew* Crew::fromLine(const string &line);

};

