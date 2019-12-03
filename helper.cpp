#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <functional>
#include <algorithm>

using namespace std;
using namespace std::placeholders;

#include "helper.h"

vector<string> Helper::readFile(const string &filename) {
  vector<string> result;
  ifstream file(filename);
  if (!file.is_open()) return result;
  string line;
  while (!file.eof()) {
    getline(file, line);
    if (line == "") continue;
    result.push_back(line);
  }
  file.close();
  return result;
}

bool Helper::writeFile(const string &filename, const vector<string> &lines) {
  ofstream file(filename, ios::out | ios::trunc);
  if (!file.is_open()) return false;
  for (const string &line : lines) file << line << endl;
  file.close();
  return true;
}

// Code from: https://stackoverflow.com/a/46931770
vector<string> Helper::split(const string &str, const string delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  string token;
  vector<string> res;

  while ((pos_end = str.find(delimiter, pos_start)) != string::npos) {
      token = str.substr(pos_start, pos_end - pos_start);
      pos_start = pos_end + delim_len;
      res.push_back(token);
  }

  res.push_back(str.substr(pos_start));
  return res;
}

int Helper::timepoint_to_int(const chrono::time_point<chrono::high_resolution_clock> &tp) {
  chrono::high_resolution_clock::duration d = tp.time_since_epoch();
  return chrono::duration_cast<chrono::seconds>(d).count();
}

chrono::time_point<chrono::high_resolution_clock> Helper::int_to_timepoint(int secs) {
  std::chrono::seconds dur(secs);
  return chrono::time_point<chrono::high_resolution_clock>(dur);
}

vector<int> Helper::string_to_ints(const string &str, const string delimiter) {
  vector<string> parts = split(str, delimiter);
  vector<int> result;
  auto stoi_fn = bind<int(const string&, size_t*, int)>(stoi, _1, nullptr, 10);
  transform(parts.begin(), parts.end(), back_inserter(result), stoi_fn);
  return result;
}
