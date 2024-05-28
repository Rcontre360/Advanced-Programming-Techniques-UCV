#include "heap.h"
#include "fibonacci/FibHeap.h"  
#include <vector>
#include <functional>
#include <random>
#include <algorithm>
#include <iostream>

using namespace std;

struct Test {
    function<bool()> func;
    string name;

    Test(function<bool()> func, string name) : func(func), name(name) {}
};

void print(vector<int> arr){
    for (int i:arr){
        cout << i << " ";
    }
    cout << endl;
}

int randInt(int min, int max) {
    random_device rd;  
    mt19937 eng(rd());
    uniform_int_distribution<> distr(min, max);  

    return distr(eng);
}

bool testHeapInsert() {
    Heap<int> heap;
    heap.insert(10);
    heap.insert(5);
    heap.insert(3);
    heap.insert(8);
    heap.insert(2);

    cout << "INSERT" << endl;
    print({10,5,3,8,2});
    cout << "Expect head to be 2"<< endl;

    return heap.head() == 2; 
}

bool testHeapRemove() {
    Heap<int> heap({10, 5, 3, 8, 2});
    heap.remove();

    cout << "REMOVE" << endl;
    cout << "inserting: ";
    print({10, 5, 3, 8, 2});
    cout << "Expect after removal of minimum. Head == 2"<<endl;

    return heap.head() == 3;  
}

bool testHeapify() {
    vector<int> elements = {-1, 5, 8, 10, 2};
    Heap<int>::heapify(elements);  

    cout << "HEAPIFY" << endl;
    print({-1, 5, 8, 10, 2});
    cout << "Result: ";
    print(elements);

    return elements[1] == 2;  
}

bool testHeapSort() {
    vector<int> elements = {-1, 3, 8, 10, 2};  
    Heap<int>::sort(elements);

    cout << "HEAP SORT"<<endl;
    print({-1, 3, 8, 10, 2});

    cout << "result: "; 
    print(elements);

    for (int i = 2; i < elements.size(); i++) {
        if (elements[i - 1] < elements[i]) {
            return false;  
        }
    }
    return true;
}

bool testHeapUnion() {
    vector<int> first = {87, 24, 63, 15, 42, 55};  
    vector<int> second = {-1, 3, 8, 10, 2, 14};  
    vector<int> all(first);  
    Heap<int>* firstH = new Heap<int>();
    Heap<int>* secondH = new Heap<int>();

    cout << "HEAP UNION" << endl;
    cout << "first: ";
    print({87, 24, 63, 15, 42, 55});
    cout << "second: ";
    print({-1, 3, 8, 10, 2, 14});

    for (int i=0;i < first.size(); i++){
        firstH->insert(first[i]);
        secondH->insert(second[i]);
    }

    all.insert(all.end(),second.begin(),second.end());
    sort(all.begin(),all.end());

    firstH -> merge(secondH);

    cout << "After merge: ";
    for (auto x : all) {
        cout << firstH->getMinimum() << " ";
        if (x != firstH->getMinimum()){
            return false;
        }
        firstH->removeMinimum();
    }
    cout << endl;

    return true;
}

bool testFibHeapInsert() {
    vector<int> elements = {-1, 3, 8, 10, 2, 14};  
    FibHeap* fibHeap = new FibHeap();

    for (auto x : elements){
        fibHeap->insert(x);
    }

    cout << "FIB HEAP INSERT" << endl;
    print({-1, 3, 8, 10, 2, 14});
    cout << "result: ";

    sort(elements.begin(), elements.end());

    for (auto x : elements) {
        cout << fibHeap->getMinimum() << " ";
        if (x != fibHeap->getMinimum()){
            return false;
        }
        fibHeap->removeMinimum();
    }
    cout << endl;
    return true;
}

bool testFibHeapGetMin() {
    vector<int> elements = {87, 24, 63, 15, 42, 55, 91, 3, 68, 30};  
    FibHeap* fibHeap = new FibHeap();

    cout << "FIB HEAP GET MIN" << endl;
    print({87, 24, 63, 15, 42, 55, 91, 3, 68, 30});
    cout << "min is 3" << endl;
    
    for (auto x : elements){
        fibHeap->insert(x);
    }

    return fibHeap->getMinimum() == 3;
}

bool testFibHeapRemoveMin() {
    vector<int> elements = {87, 24, 63, 15, 42, 55, 91, 3, 68, 30};  
    FibHeap* fibHeap = new FibHeap();

    cout << "FIB HEAP REMOVE MIN" << endl;
    print({87, 24, 63, 15, 42, 55, 91, 3, 68, 30});
    cout << "removing 2 minimum" << endl;
    cout << "minimum now is 24" << endl;

    for (auto x : elements){
        fibHeap->insert(x);
    }

    fibHeap -> removeMinimum();
    fibHeap -> removeMinimum();

    return fibHeap->getMinimum() == 24;
}

bool testFibHeapUnion() {
    vector<int> first = {87, 24, 63, 15, 42, 55};  
    vector<int> second = {-1, 3, 8, 10, 2, 14};  
    vector<int> all(first);  
    FibHeap* firstH = new FibHeap();
    FibHeap* secondH = new FibHeap();

    cout << "FIB HEAP UNION" << endl;
    cout << "first: ";
    print({87, 24, 63, 15, 42, 55});
    cout << "second: ";
    print({-1, 3, 8, 10, 2, 14});

    for (int i=0;i < first.size(); i++){
        firstH->insert(first[i]);
        secondH->insert(second[i]);
    }

    all.insert(all.end(),second.begin(),second.end());
    sort(all.begin(),all.end());

    firstH -> merge(*secondH);

    cout << "result: ";
    for (auto x : all) {
        cout << firstH->getMinimum() << " ";
        if (x != firstH->getMinimum()){
            return false;
        }
        firstH->removeMinimum();
    }
    cout << endl;

    return true;
}

const string RED = "\033[1;31m";
const string GREEN = "\033[1;32m";
const string RESET = "\033[0m";

int main() {
    vector<Test> tests = {
        Test(testHeapInsert, "Heap: insert"),
        Test(testHeapRemove, "Heap: remove"),
        Test(testHeapify, "Heap: heapify"),
        Test(testHeapSort, "Heap: sort"),
        Test(testHeapUnion, "Heap: union"),
        Test(testFibHeapInsert, "FibHeap: insert"),
        Test(testFibHeapGetMin, "FibHeap: get min"),
        Test(testFibHeapRemoveMin, "FibHeap: remove min"),
        Test(testFibHeapUnion, "FibHeap: union"),
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

    return !allTestsPassed;  
}

