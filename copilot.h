#pragma once

#include<string_view>
using namespace std;

#include "crew.h"

class CoPilot : public Crew {
public: 
  static const Crew::Type TYPE = Crew::Type::CO_PILOT;
  static const int RATING_LENGTH = 4;

protected:
  string rating{RATING_LENGTH, ' '};
  CoPilot(int id, const string &name, const string &rating);

public:
  CoPilot();
  CoPilot(const string &name);
  CoPilot(const string &name, const string &rating);
  CoPilot(CoPilot&&) = delete;
  CoPilot(const CoPilot&) = default;

public:
  string getRating() const;
  void setRating(const string &rating);

  virtual ostream &print(ostream&) const override;
  virtual bool interactiveEdit() override;
  CoPilot &operator=(const CoPilot&) = default;

  // Friends to allow instantiating this object
  friend CrewCollection;

};

