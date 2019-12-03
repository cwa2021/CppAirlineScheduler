#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

class CrewCollection {
  vector<Crew*> crew;
  const string FILENAME = "crew.txt";
  //generate consecutive ID numbers for crew members
  int idCounter = 0;
public:
  CrewCollection();
  ~CrewCollection(); 
  
  bool add();
  bool add(Crew *c);
  typename vector<Crew*>::iterator find(int val);
  typename vector<Crew*>::const_iterator find(int val) const;
  //check if argument is end iterator
  bool isEnd(typename vector<Crew*>::const_iterator) const;
  template<typename T>
  bool exists(const T &val) const;
  template<typename T>
  bool remove(const T &val);
  //write, retrieve, and print collection
  void save() const;
  void print() const;
  friend ostream &operator<<(ostream &out, const CrewCollection &c);
  //get next ID number
  int nextId();
};

ostream &operator<<(ostream &out, const CrewCollection &c);


// removes crew and will return false if no crew member is found
template<typename T>
bool CrewCollection::remove(const T &val) {
  auto it = find(val);
  if (it == crew.end()) return false;
  Crew *c = *it;
  try { 
    c->validateRemoval();
  } catch (const invalid_argument& e) {
    const string error(e.what());
    cout << "An error was encountered in performing this operation: " << error << endl;
    cout << "As a result, the deletion requested failed to execute.";
    return false;
  }
  crew.erase(it);
  delete c;
  return true;
}
// returns the crew members value, will return false if the end is reached
template<typename T>
bool CrewCollection::exists(const T& val) const {
  return !isEnd(find(val));
}

extern CrewCollection crewCollection;
