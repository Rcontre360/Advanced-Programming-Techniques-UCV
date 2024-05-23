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

// initializes an empty fibonacci heap
IHeap<int>* initEmptyFib(vector<int> &elems){
    FibHeap* fibHeap = new FibHeap();
    return fibHeap;
}

// initializes a full fibonacci heap with the elements as input
IHeap<int>* initFullFib(vector<int> &elems){
    FibHeap* fibHeap = new FibHeap();
    for (auto x : elems){
        fibHeap->insert(x);
    }
    return fibHeap;
}

// initializes an empty heap
IHeap<int>* initEmptyHeap(vector<int> &elems){
    Heap<int>* heap = new Heap<int>();
    return heap;
}

// initializes an full heap with the elements in the input
IHeap<int>* initFullHeap(vector<int> &elems){
    Heap<int>* heap = new Heap<int>();
    for (auto x : elems){
        heap->insert(x);
    }
    return heap;
}

pair<FibHeap*, vector<pair<FibHeapNode*,int>>>  initFibRandomArgs(
    vector<int> arr 
){
    vector<pair<FibHeapNode*,int>> arrChanges(arr.size());
    FibHeap* fibHeap = new FibHeap();

    for (int i=0;i < arr.size();i++){
        FibHeapNode* node = fibHeap->insertRet(arr[i]);
        arrChanges[i] = make_pair(node,arr[i] > 1 ? (arr[i]-1) : arr[i]);
    }

    return make_pair(fibHeap,arrChanges);
}

pair<Heap<int>*, vector<pair<int,int>>>  initHeapRandomArgs(
    vector<int> arr 
){
    Heap<int>* heap = new Heap<int>();

    for (int i=0;i < arr.size();i++){
        heap->insert(arr[i]);
    }

    vector<pair<int,int>> mock = {make_pair(0,0)};
    return make_pair(heap,mock);
}

//heap operations: INSERT
void heapInsert(IHeap<int>* heap, vector<int> &elems){
    for (auto x : elems){
        heap->insert(x);
    }
}

//heap operations: ACCESS MIN
void heapAccessMin(IHeap<int>* heap,vector<int> &elems){
    for (auto _ : elems){
        heap->getMinimum();
    }
}

//heap operations: REMOVE MIN
void heapRemoveMin(IHeap<int>* heap,vector<int> &elems){
    for (auto _ : elems){
        heap->removeMinimum();
    }
}

//heap operations (only fib heap): DECREASE KEY
void fibDecreaseKey(FibHeap* heap, vector<pair<FibHeapNode*,int>> &elems){
    for (auto p : elems){
        FibHeapNode* node = p.first;
        int val = p.second;

        heap->decrease_key(node,val);
    }
}

//heap operations (only normal heap): DECREASE KEY
void heapDecreaseKey(Heap<int>* heap, vector<pair<int,int>> &elems){
    for (auto p : elems){
        int ind = p.first;
        int val = p.second;

        heap->decreaseKey(ind,val);
    }
}

// this to make every call a different random number
random_device randRd;
mt19937 randEng(randRd());
int rand(int start,int end){
    uniform_int_distribution<> distr(start, end);  // to decide which op
    return distr(randEng);
}

// ONLY FIB HEAP this operation function executes a single operation (from above) only once. The operation is chosen randomly
double fibHeapRandomOp(pair<FibHeap*, vector<pair<FibHeapNode*,int>>> args){
    FibHeap* heap = args.first;
    auto elems = args.second;

    // the operation choice
    int choice = rand(0,2);
    int randIndx = rand(0, elems.size() - 1);
    // the pair input only used for the DECREASE KEY operation
    vector<int> intInput = {1};

    //SPECIAL CASE
    //if the size of the heap is 1 we dont remove, only insert. This avoids seg fault
    if (heap->m_numOfNodes <= 1 && choice == 2){
        choice = 0;
    }

    auto start = high_resolution_clock::now(); // Placeholder, will be reset in each case
    auto end = high_resolution_clock::now();   // Placeholder, will be reset in each case
    duration<double, nano> elapsed;
    FibHeapNode* node = nullptr;

    switch (choice) {
        case 0:
            start = high_resolution_clock::now();
            node = heap->insertRet(elems[randIndx].second);
            end = high_resolution_clock::now();

            elems.push_back(make_pair(node,elems[randIndx].second));
            
            elapsed = end - start;
            return elapsed.count();
        case 1:
            start = high_resolution_clock::now();
            heap->getMinimum();
            end = high_resolution_clock::now();

            elapsed = end - start;
            return elapsed.count();
        case 2:
            start = high_resolution_clock::now();
            node = heap->removeMinimumGetNode();
            end = high_resolution_clock::now();

            erase_if(elems, [node](const pair<FibHeapNode*, int>& element) {
                return element.first == node;
            });

            elapsed = end - start;

            return elapsed.count();
        case 3:
            vector<pair<FibHeapNode*,int>> pairInput = {elems[randIndx]}; // a pair that identifies an element. For the decreaseKey

            start = high_resolution_clock::now();
            fibDecreaseKey(heap, pairInput);
            end = high_resolution_clock::now();

            elapsed = end - start;

            return elapsed.count();
    }

    return 0;
}

//ONLY HEAP this operation function executes a single operation (from above) only once. The operation is chosen randomly
double heapRandomOp(pair<Heap<int>*, vector<pair<int,int>>> args){
    Heap<int>* heap = args.first;

    // the operation choice
    int choice = rand(0,3);
    int randIndx = rand(0, heap->size() - 1);
    int randNum = rand(-1000, 1000);

    //SPECIAL CASE
    //if the size of the heap is 1 we dont remove, only insert. This avoids seg fault
    if (heap->size() <= 1 && choice == 2){
        choice = 0;
    }

    auto start = high_resolution_clock::now(); // Placeholder, will be reset in each case
    auto end = high_resolution_clock::now();   // Placeholder, will be reset in each case
    duration<double, nano> elapsed;
    FibHeapNode* node = nullptr;

    switch (choice) {
        case 0:
            start = high_resolution_clock::now();
            heap->insert(randNum);
            end = high_resolution_clock::now();
            
            elapsed = end - start;
            return elapsed.count();
        case 1:
            start = high_resolution_clock::now();
            heap->getMinimum();
            end = high_resolution_clock::now();

            elapsed = end - start;
            return elapsed.count();
        case 2:
            start = high_resolution_clock::now();
            heap->removeMinimum();
            end = high_resolution_clock::now();
            elapsed = end - start;

            return elapsed.count();
        case 3:
            int valDecrease = heap->valueOf(randIndx) - 1;
            start = high_resolution_clock::now();
            heap->decreaseKey(randIndx,valDecrease);
            end = high_resolution_clock::now();

            elapsed = end - start;

            return elapsed.count();
    }

    return 0;
}

// generates an array with random elements from 1 to n
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

// makes a benchmark of a single operation. It receives a setup function to create the initial heap
template <typename Func,typename Setup>
double benchmark(Setup setup,Func operation, vector<int>& arr) {
    auto heap = setup(arr); //we skip construction time
    auto start = high_resolution_clock::now();
    operation(heap,arr);
    auto end = high_resolution_clock::now();

    duration<double, micro> elapsed = end - start;
    return elapsed.count();
}

// makes a benchmark of the average time for a set of random operations. 
// It receives a setup function to create the initial heap
template <typename Func,typename Setup>
void benchmarkRandomAverage(
    const string& heap,
    int size,
    int times,
    ofstream& file,
    Setup setup,
    Func operation
){
    vector<int> arr = generateArray(size);
    double all = 0;
    auto args = setup(arr);

    for (int i=0; i < times; i++){
        double op_time = operation(args);
        cout << "op_time " << op_time << endl;
        all += op_time;
    }

    cout << "all: " << all / times << " " << times << endl;
    double avr = all / times;
    cout << "done: " << "Random" << "|" << size << endl;
    file << heap << "," << "Random" << "," << size << "," << avr << "\n";
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
    file << heap << "," << type << "," << size << "," << time << "\n";
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

    cout << "done:" << "DecreaseKey" << "|" << size << "\n";
    file << "FibHeap," << "DecreaseKey," << size << "," << time.count() << "\n";
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

    cout << "done:" << "DecreaseKey" << "|" << size << "\n";
    file << "Heap," << "DecreaseKey," << size << "," << time.count() << "\n";
}

void individualOperationsBenchmark() {
    vector<int> sizes;  // Example sizes, adjust as needed
    ofstream heapFile("./benchmarks/heap.csv");
    ofstream fibFile("./benchmarks/fib.csv");

    heapFile << "Heap,Operation,Times,Time (μs)\n";
    fibFile << "Heap,Operation,Times,Time (μs)\n";

    for (int i=10000; i <= 1000000; i+=10000){
        sizes.push_back(i);
    }

    for (int size : sizes) {
        thread threads[8];

        threads[0] = std::thread([=, &fibFile]() {
            benchmarkAndLog("FibHeap", "AccessMin", size, fibFile, heapAccessMin,initFullFib);
        });
        threads[1] = std::thread([=, &fibFile]() {
            benchmarkAndLog("FibHeap","RemoveMin", size, fibFile, heapRemoveMin, initFullFib);
        });
        threads[2] = std::thread([=, &fibFile]() {
            benchmarkAndLog("FibHeap", "Insert", size, fibFile, heapInsert, initEmptyFib);
        });

        threads[3] = std::thread([=, &heapFile]() {
            benchmarkAndLog("Heap", "AccessMin", size, heapFile, heapAccessMin,initFullHeap);
        });
        threads[4] = std::thread([=, &heapFile]() {
            benchmarkAndLog("Heap","RemoveMin", size, heapFile, heapRemoveMin, initFullHeap);
        });
        threads[5] = std::thread([=, &heapFile]() {
            benchmarkAndLog("Heap", "Insert", size, heapFile, heapInsert, initEmptyHeap);
        });
        
        threads[6] = std::thread([=, &fibFile]() {
            benchmarkAndLogDecreaseKeyFib(size, fibFile);
        });
        threads[7] = std::thread([=, &heapFile]() {
            benchmarkAndLogDecreaseKeyHeap(size, heapFile);
        });


        for (auto& th : threads) {
            th.join();
        }
    }

    heapFile.close();
    fibFile.close();
}

void randomOperationsBenchmark() {
    vector<int> sizes;  // Example sizes, adjust as needed
    ofstream heapFile("./benchmarks/heap_rand.csv");
    ofstream fibFile("./benchmarks/fib_rand.csv");

    heapFile << "Heap,Operation,Times,Time (μs)\n";
    fibFile << "Heap,Operation,Times,Time (μs)\n";

    for (int i=100; i*100 <= 1000000; i+=100){
        sizes.push_back(i);
    }

    for (int size : sizes) {
        thread threads[2];

        threads[0] = std::thread([=, &fibFile]() {
            benchmarkRandomAverage("FibHeap", size, size*100, fibFile, initFibRandomArgs,fibHeapRandomOp);
        });
        threads[1] = std::thread([=, &heapFile]() {
            benchmarkRandomAverage("Heap", size, size*100, heapFile, initHeapRandomArgs,heapRandomOp);
        });

        for (auto& th : threads) {
            th.join();
        }
    }

    heapFile.close();
    fibFile.close();
}

int main(){
    cout << "INDIVIDUAL OP BENCHMARK" << endl;
    //individualOperationsBenchmark();
    cout << "RANDOM OP AVG BENCHMARK" << endl;
    randomOperationsBenchmark();

    return 0;
}

