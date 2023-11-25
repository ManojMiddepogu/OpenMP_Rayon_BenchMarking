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
void writeCSV(const string& filename, const vector<vector<double>>& data);
string args_to_str(map<string, int> args);
vector<double> arr_to_vec(double arr[], int n);

#endif