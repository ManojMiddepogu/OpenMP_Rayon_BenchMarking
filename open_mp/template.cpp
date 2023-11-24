#include <iostream>
#include <string>
#include <map>

#include "../utils/utils.h"
using namespace std;

int main() {
    // Read JSON from file
    string config_file_pth = "../Config/config_matrix_multiply_cpp.json";

    // Parse JSON and create a map
    map<string, int> args = readJsonFile(config_file_pth);

    // Print the contents of the map
    for (const auto& entry : args) {
        cout << "Key: " << entry.first << ", Value: " << entry.second << endl;
    }

    return 0;
}