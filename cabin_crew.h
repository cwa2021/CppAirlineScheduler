#pragma once

#include <string>
#include <vector>
#include <map>
using namespace std;

#include "crew.h"

class CabinCrew : public Crew {
public: 
  static const Crew::Type TYPE = Crew::Type::CABIN;
  enum Position { FIRST_CLASS, BUSINESS_CLASS, ECONOMY_FRONT, ECONOMY_REAR, LEAD };
  static const map<Position, string> POSITION_TO_STRING;
protected:
  Position position = FIRST_CLASS;
  CabinCrew(int id, const string &name, Position position);
public:
  CabinCrew();
  ~CabinCrew() = default;
  CabinCrew(const string &name);
  CabinCrew(const string &name, Position position);
  CabinCrew(CabinCrew&&) = delete;
  CabinCrew(const CabinCrew&) = default;
  CabinCrew(const vector<string> &parts);

public:
  Position getPosition() const;

  ostream &print(ostream&) const override;
  CabinCrew &operator=(const CabinCrew&) = default;
  string toLine() const override;

  // Friends to allow instantiating this object
  friend CrewCollection;
  friend Crew* Crew::fromLine(const string &line);

};

