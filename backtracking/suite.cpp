#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "processing.h"

using namespace std;

void setColor(int color) {
    if (color == 2) { // Green
        cout << "\033[1;32m";
    } else if (color == 4) { // Red
        cout << "\033[1;31m";
    }
}

void resetColor() {
    cout << "\033[0m";
}

int main() {
    const string testFolder = "./tests/";  // Folder where tests are stored
    int testNum = 1;
    string inputFile, expectedFile, line, input, expected;

    while (true) {
        inputFile = testFolder + "input_" + to_string(testNum) + "_.txt";
        expectedFile = testFolder + "output_" + to_string(testNum) + "_.txt";

        ifstream inputFS(inputFile);
        ifstream expectedFS(expectedFile);

        if (!inputFS.is_open() || !expectedFS.is_open()) {
            break;  // Stop when files are not found
        }

        // Read entire content of input file
        stringstream strStream;
        strStream << inputFS.rdbuf();
        input = strStream.str();

        // Get expected output
        getline(expectedFS, expected);

        // Process the input
        string result = processInput(input);
        
        // Display result
        if (result == expected) {
            setColor(2);
            cout << "Test " << testNum << ": Success" << endl;
        } else {
            setColor(4);
            cout << "Test " << testNum << ": Failure - Expected " << expected << ", got " << result << endl;
        }
        resetColor();

        inputFS.close();
        expectedFS.close();
        testNum++;
    }

    return 0;
}

