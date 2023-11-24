#include <string>
#include <map>
using namespace std;

// utils.h
#ifndef UTILS_H  // Include guard to prevent multiple inclusions
#define UTILS_H

// Declare your functions here
string removeSpacesAndNewLines(const string& jsonString);
map<string, int> parseJSONToMap(const string& jsonString);
map<string, int> readJsonFile(const string& filename);

#endif