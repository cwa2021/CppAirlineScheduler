#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

#include "aircraft.h"

class AircraftCollection {
  vector<Aircraft> aircraft;
  const string FILENAME = "aircraft.txt";
  
public:
  AircraftCollection();
  void add();
  void add(Aircraft &c);
  template<typename T>
  typename vector<Aircraft>::iterator find(T val);
  //check if an aircraft exists
  template<typename T>
  bool exists(T val);
  template<typename T>
  bool remove(T val);
  //check if argument given is end iterator
  bool isEnd(typename vector<Aircraft>::const_iterator it);
  int tailNumCount(const string&) const;
  //write, retrieve and print aircraft collection
  void save();
  void print() const;
  friend ostream &operator<<(ostream&, const AircraftCollection&);
  //friend istream &operator>>(istream&, AircraftCollection&);
};

ostream &operator<<(ostream&, const AircraftCollection&);
//istream &operator>>(istream&, AircraftCollection&);

//iterates through and returns an iterator to the first aircraft equal to val, or aircraft.end() if none exist.
template<typename T>
typename vector<Aircraft>::iterator AircraftCollection::find(T val) {
  return std::find(aircraft.begin(), aircraft.end(), val);
}


// Call the find function above, and if an aircraft matching val is found, remove it from the set
template<typename T>
bool AircraftCollection::remove(T val) {
  auto it = find(val);
  if (it == aircraft.end()) return false;
  try { 
    it->validateRemoval();
  } catch (const invalid_argument& e) {
    const string error(e.what());
    cout << "An error was encountered in performing this operation: " << error << endl;
    cout << "As a result, the deletion requested failed to execute.";
    return false;
  }
  aircraft.erase(it);
  return true;
}

// returns a boolean for if an aircraft matching val was found in the aircraft set
template<typename T>
bool AircraftCollection::exists(T val) {
  return find(val) != aircraft.end();
}

extern AircraftCollection aircraftCollection;
