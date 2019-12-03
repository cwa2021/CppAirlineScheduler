#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <numeric>
#include <map>
#include <set>
#include <typeindex>


namespace Helper {
  using namespace std;
  // read a file into a vector of strings/lines
  vector<string> readFile(const string &filename);
  // write a vector of strings/lines into a file
  bool writeFile(const string &filename, const vector<string> &lines) ;
  // splits a string into a vector at a delimiter
  // Code from: https://stackoverflow.com/a/46931770
  vector<string> split(const string &str, const string delimiter = ",");

  // converts a list of ints to a string (separated by delimiters)
  // Based on: https://en.cppreference.com/w/cpp/algorithm/accumulate
  template<typename InputIt>
  string ints_to_string(InputIt begin, InputIt end, const string delimiter = " ") {
    auto fold = [&delimiter](const string &a, int b) {
      if (a == "") return to_string(b);
      return a + delimiter + to_string(b);
    };
    if (distance(begin, end) == 0) return "";
    return accumulate(begin, end, string(""), fold);
  }

  // converts a string (separated by delimiters) to a vector of ints
  vector<int> string_to_ints(const string &str, const string delimiter = " ");

  // convert time_point type to an integer number of seconds since unix epoch (1/1/1970)
  int timepoint_to_int(const chrono::time_point<chrono::high_resolution_clock> &tp);
  // convert an integer number of seconds since unix epoch (1/1/1970) to STL timepoint
  chrono::time_point<chrono::high_resolution_clock> int_to_timepoint(int secs);

  // unused
  // This function from https://stackoverflow.com/a/24263441
  template <class T, class Comp, class Alloc, class Predicate>
  void discard_if(std::set<T, Comp, Alloc>& c, Predicate pred) {
    for (auto it{c.begin()}, end{c.end()}; it != end; ) {
      if (pred(*it)) {
        it = c.erase(it);
      } else {
        ++it;
      }
    }
  }

  // takes a map with string keys and prompts the user to choose one of them
  // returns the value associated with that key
  template<typename T>
  const T &promptMenu(const map<string, T>& choices, const string &prompt) {
    int choiceIdx = -1;
    bool first = true;
    const int numChoices = choices.size();
    while (choiceIdx < 1 || choiceIdx > numChoices) {
      if (!first) cout << "Invalid choice. Try again." << endl;
      cout << prompt << endl;
      cout << "Please select an option from the following list:" << endl;
      int i = 0;
      for (const pair<const string, T> &choice : choices) {
        cout << (++i) << ") " << choice.first << endl;
      }
      cout << "Enter your choice (1-" << i << "): ";
      string line;
      getline(cin, line);
      //cin.ignore();
      istringstream ss(line);
      ss >> choiceIdx;
    }
    typename map<string, T>::const_iterator choice = choices.begin();
    advance(choice, choiceIdx - 1);
    return choice->second;
  }

  // shows the user a prompt where they can optionally replace the previous/default value of a field with a new one
  // if no input is provided, default is kept
  template <typename T>
  T promptValueWithDefault(const string &prompt, T oldValue) {
    T newValue = oldValue;
    while (true) {
      try {
        cout << prompt << " [" << oldValue << "]: ";
        string line;
        getline(cin, line);
        istringstream ss(line);
        ss >> newValue;
      } catch (range_error &e) {
        cout << e.what() << endl;
        cout << "Please try again:" << endl;
        continue;
      }
      break;
    }
    return newValue;
  }

  // prompts the user for a value, without providing a fallback default value
  template <typename T>
  T promptValue(const string &prompt) {
    T value;
    bool first = true;
    bool success = false;
    while (!success) {
      if (!first) cout << "Invalid response. Try again." << endl;
      first = false;
      cout << prompt << ": ";
      string line;
      getline(cin, line);
      //cin.ignore();
      istringstream ss(line);
      ss >> value;
      if (!ss.fail()) success = true;
    }
    return value;
  }

  template <typename K, typename V>
  map<V, K> invertMap(const map<K,V> &oldMap) {
    map<V, K> newMap;
    for (const pair<K,V> &entry : oldMap) {
      pair<V, K> newPair(entry.second, entry.first);
      newMap.insert(newPair);
    }
    return newMap;
  }
}

/*
template<typename Clock, typename Duration>
ostream &operator<<(ostream &out, const chrono::time_point<Clock, Duration> &tp) {
  out << secs;
  return out;
}*/
