#include "heap.h"  
#include "fibonacci/FibHeap.h"  
#include <chrono>
#include <fstream>
#include <random>
#include <algorithm>
#include <iostream>
#include <vector>
#include <thread>

using namespace std;
using namespace chrono;

IHeap<int>* initEmptyFib(vector<int> &elems){
    FibHeap* fibHeap = new FibHeap();
    return fibHeap;
}

IHeap<int>* initFullFib(vector<int> &elems){
    FibHeap* fibHeap = new FibHeap();
    for (auto x : elems){
        fibHeap->insert(x);
    }
    return fibHeap;
}

IHeap<int>* initEmptyHeap(vector<int> &elems){
    Heap<int>* heap = new Heap<int>();
    return heap;
}

IHeap<int>* initFullHeap(vector<int> &elems){
    Heap<int>* heap = new Heap<int>();
    for (auto x : elems){
        heap->insert(x);
    }
    return heap;
}

//heap operations
void heapInsert(IHeap<int>* heap, vector<int> &elems){
    for (auto x : elems){
        heap->insert(x);
    }
}

//heap operations
void heapAccessMin(IHeap<int>* heap,vector<int> &elems){
    for (auto _ : elems){
        heap->getMinimum();
    }
}

//heap operations
void heapRemoveMin(IHeap<int>* heap,vector<int> &elems){
    for (auto _ : elems){
        heap->removeMinimum();
    }
}

//fib dec key
void fibDecreaseKey(FibHeap* heap, vector<pair<FibHeapNode*,int>> &elems){
    for (auto p : elems){
        FibHeapNode* node = p.first;
        int val = p.second;

        heap->decrease_key(node,val);
    }
}

//heap dec key
void heapDecreaseKey(Heap<int>* heap, vector<pair<int,int>> &elems){
    for (auto p : elems){
        int ind = p.first;
        int val = p.second;

        heap->decreaseKey(ind,val);
    }
}

vector<int> generateArray(int n) {
    vector<int> arr;
    arr.reserve(n);

    for (int i = 0; i < n; ++i) {
        arr.push_back(i+1);
    }

    random_device rd;
    mt19937 g(rd());
    shuffle(arr.begin(), arr.end(), g);

    return arr;
}

template <typename Func,typename Setup>
double benchmark(Setup setup,Func operation, vector<int>& arr) {
    auto heap = setup(arr); //we skip construction time
    auto start = high_resolution_clock::now();
    operation(heap,arr);
    auto end = high_resolution_clock::now();

    duration<double, micro> elapsed = end - start;
    return elapsed.count();
}

void benchmarkAndLog(
    const string& heap,
    const string& type,
    int size,
    ofstream& file,
    void(*operation)(IHeap<int>*,vector<int>&),
    IHeap<int>*(*setup)(vector<int>&)
){
    vector<int> arr = generateArray(size);
    double time = benchmark(setup,operation, arr);

    cout << "done: " << type << "|" << size << endl;
    file << heap << type << "," << size << "," << time << "\n";
}

void benchmarkAndLogDecreaseKeyFib(
    int size,
    ofstream& file
){
    vector<int> arr = generateArray(size);
    vector<pair<FibHeapNode*,int>> arrChanges(arr.size());
    FibHeap* fibHeap = new FibHeap();

    for (int i=0;i < arr.size();i++){
        FibHeapNode* node = fibHeap->insertRet(arr[i]);
        arrChanges[i] = make_pair(node,arr[i] > 1 ? (arr[i]-1) : arr[i]);
    }

    auto start = high_resolution_clock::now();
    fibDecreaseKey(fibHeap,arrChanges);
    auto end = high_resolution_clock::now();
    duration<double, micro> time = end - start;

    cout << "FibHeap" << "|" << size << "|" << "\n";
    file << "FibHeap" << "DecreaseKey" << "," << size << "," << time.count() << "\n";
}

void benchmarkAndLogDecreaseKeyHeap(
    int size,
    ofstream& file
){
    vector<int> arr = generateArray(size);
    vector<pair<int,int>> arrChanges(arr.size());
    Heap<int>* heap = new Heap<int>();
    random_device rd;
    mt19937 g(rd());
    uniform_int_distribution<> distr(0, arr.size() - 1);

    for (int i=0;i < arr.size();i++){
        int rndIndex = distr(g);
        heap->insert(arr[i]);
        arrChanges[i] = make_pair(rndIndex,arr[i] > 1 ? (arr[i]-1) : arr[i]);
    }

    auto start = high_resolution_clock::now();
    heapDecreaseKey(heap,arrChanges);
    auto end = high_resolution_clock::now();
    duration<double, micro> time = end - start;

    cout << "Heap" << "|" << size << "|" << "\n";
    file << "Heap" << "DecreaseKey" << "," << size << "," << time.count() << "\n";
}

int main() {
    vector<int> sizes;  // Example sizes, adjust as needed
    ofstream heapFile("./benchmarks/heap.csv");
    ofstream fibFile("./benchmarks/fib.csv");

    heapFile << "Heap,Operation,Times,Time (μs)\n";
    fibFile << "Heap,Operation,Times,Time (μs)\n";

    for (int i=10000; i <= 1000000; i+=10000){
        sizes.push_back(i);
    }

    for (int size : sizes) {
        thread threads[2];

        threads[0] = std::thread([=, &fibFile]() {
            benchmarkAndLogDecreaseKeyFib(size, fibFile);
        });
        threads[1] = std::thread([=, &heapFile]() {
            benchmarkAndLogDecreaseKeyHeap(size, heapFile);
        });
        //fib heap
        //threads[0] = std::thread([=, &fibFile]() {
            //benchmarkAndLog("FibHeap", "AccessMin", size, fibFile, heapAccessMin,initFullFib);
        //});
        //threads[1] = std::thread([=, &fibFile]() {
            //benchmarkAndLog("FibHeap","RemoveMin", size, fibFile, heapRemoveMin, initFullFib);
        //});
        //threads[2] = std::thread([=, &fibFile]() {
            //benchmarkAndLog("FibHeap", "Insert", size, fibFile, heapInsert, initEmptyFib);
        //});

        //heap
        //threads[3] = std::thread([=, &heapFile]() {
            //benchmarkAndLog("Heap", "AccessMin", size, heapFile, heapAccessMin,initFullHeap);
        //});
        //threads[4] = std::thread([=, &heapFile]() {
            //benchmarkAndLog("Heap","RemoveMin", size, heapFile, heapRemoveMin, initFullHeap);
        //});
        //threads[5] = std::thread([=, &heapFile]() {
            //benchmarkAndLog("Heap", "Insert", size, heapFile, heapInsert, initEmptyHeap);
        //});


        for (auto& th : threads) {
            th.join();
        }
    }

    heapFile.close();
    fibFile.close();

    return 0;
}

