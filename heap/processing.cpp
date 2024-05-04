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

    Heap<int>* second = new Heap<int>(*heap);
    while (second->size() > 1){
        int el = second->head();
        second->remove();
        result += to_string(el);
        result += " ";
    }


    result.pop_back();
    return result;
}

bool validate(const std::string& output) {
    std::istringstream iss(output);
    std::vector<int> numbers;
    int num;

    while (iss >> num) {
        numbers.push_back(num);
    }

    for (int i = 1; i < numbers.size(); ++i) {
        if (numbers[i-1] >= numbers[i]) {
            return false; 
        }
    }

    return true; // Numbers are in strictly ascending order
}


