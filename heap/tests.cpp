#include "point.cpp"
#include <vector>
#include <functional>
#include <iostream>

using namespace std;

struct Test {
    function<bool()> func;
    string name;

    Test(function<bool()> func, string name) : func(func), name(name) {}
};

bool testPointComparison() {
    Point p1 = {3, 4};  // Distance from origin = 5
    Point p2 = {5, 12}; // Distance from origin = 13
    return p1 < p2;  // Should return true
}

bool testEquality() {
    Point p1 = {6, 8};  // Distance from origin = 10
    Point p2 = {6, 8};  // Same as p1
    return !(p1 < p2) && !(p2 < p1);  // Expect neither to be less than the other
}

// Colored output
const string RED = "\033[1;31m";
const string GREEN = "\033[1;32m";
const string RESET = "\033[0m";

int main() {
    vector<Test> tests = {
        Test(testPointComparison, "Point Distance Comparison Test"),
        Test(testEquality, "Point Equality Test")
    };

    bool allTestsPassed = true;

    for (const auto& test : tests) {
        bool result = test.func();
        cout << (result ? GREEN : RED) << test.name << ": " 
             << (result ? "Passed" : "Failed") << RESET << endl;
        if (!result) {
            allTestsPassed = false;
        }
    }

    cout << (allTestsPassed ? GREEN : RED) << "All tests " 
         << (allTestsPassed ? "passed!" : "failed.") << RESET << endl;

    return !allTestsPassed;  // Return error code if any test fails
}

