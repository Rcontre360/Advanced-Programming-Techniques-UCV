#include "heap.cpp"

#include <sstream>
#include <vector>
#include <iostream>
#include <random>

using namespace std;

string processInput(const string& input) {
    std::istringstream iss(input);
    Heap<int>* heap = new Heap<int>();
    string result = "";
    int number;

    while (iss >> number) {
        heap->insert(number);
    }

    std::vector<int> results;

    while (heap->size() > 1){
        int el = heap->remove();
        result += to_string(el);
        result += " ";
    }

    result.pop_back();
    return result;
}

bool validate(const string& output) {
    return false; // No conflicts found
}

