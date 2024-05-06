#include "point.cpp"
#include "heap.cpp"
#include <vector>
#include <functional>
#include <random>
#include <iostream>

using namespace std;

struct Test {
    function<bool()> func;
    string name;

    Test(function<bool()> func, string name) : func(func), name(name) {}
};

Point randPoint() {
    random_device rd;  // Obtain a random number from hardware
    mt19937 eng(rd()); // Seed the generator
    uniform_real_distribution<> distr(-100.0, 100.0);  // Define the range as -100.0 to 100.0

    Point p(distr(eng),distr(eng));
    return p;
}

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

bool testHeapInsert() {
    Heap<int> heap;
    heap.insert(10);
    heap.insert(5);
    heap.insert(3);
    heap.insert(8);
    heap.insert(2);

    return heap.head() == 10;
}

bool testHeapRemove() {
    Heap<int> heap({10, 5, 3, 8, 2});
    heap.remove();  

    return heap.head() == 8;
}

bool testHeapSink() {
    vector<int> elements = {0, 5, 3, 8, 10, 15};
    int n = elements.size();
    Heap<int>::sink(elements, 2, n);  

    return elements[n - 1] == 3;
}

bool testHeapify() {
    vector<int> elements = {0,9, 5, 8, 10, 2};
    Heap<int>::heapify(elements);  

    return elements[1] == 10;
}

bool testHeapSort() {
    vector<int> elements = {0, 5, 3, 8, 10, 2};  // Include 0 to align indices as 1-based
    Heap<int>::sort(elements);

    for (int i = 2; i < elements.size(); i++) {
        if (elements[i - 1] > elements[i]) {
            return false;  
        }
    }
    return true;
}

bool testHeapSortPoint() {
    vector<Point> elements = {Point(0,0), randPoint(), randPoint(), randPoint(), randPoint(), randPoint()};  // Include 0 to align indices as 1-based
    Heap<Point>::sort(elements);

    for (int i = 2; i < elements.size(); i++) {
        if (elements[i-1] > elements[i]) {
            return false;  
        }
    }
    return true;
}

// Colored output
const string RED = "\033[1;31m";
const string GREEN = "\033[1;32m";
const string RESET = "\033[0m";

int main() {
    vector<Test> tests = {
        Test(testPointComparison, "Point Distance Comparison Test"),
        Test(testEquality, "Point Equality Test"),
        Test(testHeapInsert, "Test heap insert"),
        Test(testHeapRemove, "Test heap remove"),
        Test(testHeapSink, "Test heap sink"),
        Test(testHeapify, "Test heapify"),
        Test(testHeapSort, "Test heap sort"),
        //Test(testHeapSortPoint, "Test heap sort on points"),
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

