#pragma once

#include<string>
#include<vector>
using namespace std;

#include "crew.h"

class CoPilot : public Crew {
public: 
  static const Crew::Type TYPE = Crew::Type::CO_PILOT;
  static const int RATING_LENGTH = 4;

protected:
  string rating{RATING_LENGTH, ' '};
  CoPilot(int id, const string &name, const string &rating);
  CoPilot(const vector<string>&);

public:
  CoPilot();
  ~CoPilot() = default;
  CoPilot(const string &name);
  CoPilot(const string &name, const string &rating);
  CoPilot(CoPilot&&) = delete;
  CoPilot(const CoPilot&) = default;

public:
  string getRating() const;
  void setRating(const string &rating);

  ostream &print(ostream&) const override;
  bool interactiveEdit() override;
  string toLine() const override;
  CoPilot &operator=(const CoPilot&) = default;

  // Friends to allow instantiating this object
  friend CrewCollection;
  friend Crew* Crew::fromLine(const string &line);

};

