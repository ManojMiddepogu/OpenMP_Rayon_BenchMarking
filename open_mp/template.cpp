#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <stdexcept>

#include "../utils/utils.h"
using namespace std;

int main() {
    // Read JSON from file
    ifstream jsonFile("../Config/config_matrix_multiply_cpp.json");

    if (!jsonFile.is_open()) {
        cerr << "Failed to open JSON file\n";
        return 1;
    }

    stringstream buffer;
    buffer << jsonFile.rdbuf();
    string json = buffer.str();
    json = removeSpacesAndNewLines(json);

    // Parse JSON and create a map
    map<string, int> myMap = parseJSONToMap(json);

    // Print the contents of the map
    for (const auto& entry : myMap) {
        cout << "Key: " << entry.first << ", Value: " << entry.second << endl;
    }

    return 0;
}