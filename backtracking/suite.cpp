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

bool useValidationFunction(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-v" || arg == "--validate") {
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    const string testFolder = "./tests/";
    int testNum = 1;
    string inputFile, expectedFile, line, input, expected;
    bool useValidate = useValidationFunction(argc, argv);

    while (true) {
        inputFile = testFolder + "input_" + to_string(testNum) + "_.txt";
        ifstream inputFS(inputFile);

        if (!inputFS.is_open()) {
            break;  // Stop when files are not found
        }

        // Read entire content of input file
        stringstream strStream;
        strStream << inputFS.rdbuf();
        input = strStream.str();
        inputFS.close();

        // Process the input
        string result = processInput(input);

        bool success;
        if (useValidate) {
            success = validate(result);  // Assume validate returns a bool
        } else {
            expectedFile = testFolder + "output_" + to_string(testNum) + "_.txt";
            ifstream expectedFS(expectedFile);
            if (!expectedFS.is_open()) {
                cout << "Test " << testNum << ": Expected output file missing." << endl;
                testNum++;
                continue;
            }
            getline(expectedFS, expected);
            expectedFS.close();

            success = (result == expected);
        }

        // Display result
        if (success) {
            setColor(2);
            cout << "Test " << testNum << ": Success" << endl;
        } else {
            setColor(4);
            if (!useValidate) {
                cout << "Test " << testNum << ": Failure - Expected " << expected << ", got " << result << endl;
            } else {
                cout << "Test " << testNum << ": Validation Failed - Result: " << result << endl;
            }
        }
        resetColor();

        testNum++;
    }

    return 0;
}
