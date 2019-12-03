#pragma once

#include<string_view>
using namespace std;

#include "crew.h"

class CabinCrew : public Crew {
public: 
  static const Crew::Type TYPE = Crew::Type::CABIN;
  enum Position { FIRST_CLASS, BUSINESS_CLASS, ECONOMY_FRONT, ECONOMY_REAR, LEAD };
protected:
  Position position = FIRST_CLASS;
  CabinCrew(int id, const string &name, Position position);

public:
  CabinCrew();
  CabinCrew(const string &name);
  CabinCrew(const string &name, Position position);
  CabinCrew(CabinCrew&&) = delete;
  CabinCrew(const CabinCrew&) = default;

public:
  Position getPosition() const;

  virtual ostream &print(ostream&) const override;
  CabinCrew &operator=(const CabinCrew&) = default;

  // Friends to allow instantiating this object
  friend CrewCollection;

};

