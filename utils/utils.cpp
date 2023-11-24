#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <stdexcept>

#include "utils.h"

string removeSpacesAndNewLines(const string& jsonString){
    string result;

    for (char c : jsonString) {
        // Exclude newline characters and spaces
        if (c != '\n' && c != ' ' && c != '\t') {
            result += c;
        }
    }

    return result;
}

// Function to parse JSON and create a map
map<string, int> parseJSONToMap(const string& jsonString) {
    map<string, int> resultMap;
    string key, value;

    // Check if the string starts with '{'
    if (jsonString[0] != '{') {
        throw runtime_error("Invalid JSON string");
    }

    size_t pos = 1; // Index after the initial '{'
    while (pos < jsonString.length()) {
        if (jsonString[pos] == '}') {
            break; // Reached the end of the JSON object
        }

        // Extract key
        pos++; // Skip the opening '"'
        while (jsonString[pos] != '"') {
            key += jsonString[pos];
            pos++;
        }
        pos++; // Skip the closing '"'

        // Check for colon separator
        if (jsonString[pos] != ':') {
            throw runtime_error("Invalid JSON format :");
        }
        pos++; // Skip the colon

        // Extract value
        // pos++; // Skip the opening '"'
        while (pos < jsonString.length() && jsonString[pos] != ',') {
            value += jsonString[pos];
            pos++;
        }
        pos++; // Skip the closing '"'

        resultMap[key] = stoi(value);

        // // Check for comma delimiter
        // if (pos < jsonString.length() && jsonString[pos] == ',') {
        //     pos++; // Skip the comma
        // }

        key.clear();
        value.clear(); // Reset for next pair
    }

    return resultMap;
}

map<string, int> readJsonFile(const string& filename){
    // Read JSON from file
    ifstream jsonFile(filename);

    if (!jsonFile.is_open()) {
        throw runtime_error("Failed to open JSON file\n");
    }

    stringstream buffer;
    buffer << jsonFile.rdbuf();
    string json = buffer.str();
    json = removeSpacesAndNewLines(json);

    return parseJSONToMap(json);
}
