#include "nqueens.cpp"

#include <sstream>
#include <vector>
#include <iostream>
#include <random>

using namespace std;

string processInput(const string& input) {
    NQeensPartial* prob = new NQeensPartial(stoi(input),30);
    string res = "";

    prob->solve(0);

    for (auto el:prob->getMat()) {
        res += to_string(el) + " ";
    }
    res.pop_back();
    prob->print();

    return res;
}

bool validate(const string& output) {
    istringstream iss(output);
    vector<int> positions;
    int pos;

    // Read the positions from the string into a vector
    while (iss >> pos) {
        positions.push_back(pos - 1); // Convert to 0-based index for easier calculations
    }

    int n = positions.size();

    // Check for conflicts
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            // Check if queens are in the same row
            if (positions[i] == positions[j]) {
                return false;
            }

            // Check diagonal conflicts
            if (abs(positions[i] - positions[j]) == abs(i - j)) {
                return false;
            }
        }
    }

    return true; // No conflicts found
}

