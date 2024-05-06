#include "quick_sort.cpp"  
#include "point.cpp"  
#include "heap.cpp"  
#include <chrono>
#include <fstream>
#include <random>
#include <algorithm>
#include <iostream>
#include <vector>
#include <thread>

using namespace std;
using namespace chrono;

enum ArrayType { ASC, DESC, RAND };

void quickSortWrapper(vector<int>& arr) {
    quickSort(arr, 0, arr.size() - 1);
}

void stdSortWrapper(vector<int>& arr) {
    sort(arr.begin() , arr.end());
}

void heapSortWrapper(vector<int> &arr) {
    Heap<int>::sort(arr);
}

bool validateSorting(const vector<int>& arr){
    int n = arr.size();

    //heap sort omits arr[0]
    for (int i = 2; i < n; i++) {
        if (arr[i-1] >= arr[i]) {
            return false; 
        }
    }

    return true; 
}

vector<int> generateArray(int n, ArrayType type) {
    vector<int> arr(n);
    iota(arr.begin(), arr.end(), 1); 

    if (type == DESC) {
        reverse(arr.begin(), arr.end());
    } else if (type == RAND) {
        random_device rd;
        mt19937 g(rd());
        shuffle(arr.begin(), arr.end(), g);
    }
    return arr;
}

template <typename Func>
double benchmarkSort(Func sortFunc, vector<int>& arr) {
    auto start = high_resolution_clock::now();
    sortFunc(arr);
    auto end = high_resolution_clock::now();

    if (!validateSorting(arr)){
        cout << "Not sorted correctly" << endl;
        throw std::runtime_error("Not sorted correctly");
    }
    
    duration<double, micro> elapsed = end - start;
    return elapsed.count();
}

void sortAndLog(const string& type,  int size, ofstream& file, ArrayType arrayType, void(*sortFunc)(vector<int>&),const string& alg) {
    vector<int> arr = generateArray(size, arrayType);
    double time = benchmarkSort(sortFunc, arr);

    cout << "done: " << type << "|" << alg << "|" << size << endl;
    file << size << ", " << type << ", " << time << "\n";

    arr.clear();
}

int main() {
    vector<int> sizes  = {10000};  // Example sizes, adjust as needed
    ofstream quickFile("./benchmarks/quick_sort.csv");
    ofstream heapFile("./benchmarks/heap.csv");
    ofstream stdSortFile("./benchmarks/std.csv");

    quickFile << "Size, Type, Time (μs)\n";
    heapFile << "Size, Type, Time (μs)\n";
    stdSortFile << "Size, Type, Time (μs)\n";

    //for (int i=10000; i <= 50000; i+=10000){
        //sizes.push_back(i);
    //}

    for (int size : sizes) {
        thread threads[9];

        threads[0] = thread(sortAndLog, "Ascending", size, ref(quickFile), ASC, quickSortWrapper, "quick_sort");
        threads[1] = thread(sortAndLog, "Descending", size, ref(quickFile), DESC, quickSortWrapper,"quick_sort");
        threads[2] = thread(sortAndLog, "Random", size, ref(quickFile), RAND, quickSortWrapper,"quick_sort");

        threads[3] = thread(sortAndLog, "Ascending", size, ref(heapFile), ASC, heapSortWrapper,"heap_sort");
        threads[4] = thread(sortAndLog, "Descending", size, ref(heapFile), DESC, heapSortWrapper,"heap_sort");
        threads[5] = thread(sortAndLog, "Random", size, ref(heapFile), RAND, heapSortWrapper,"heap_sort");

        threads[6] = thread(sortAndLog, "Ascending", size, ref(stdSortFile), ASC, stdSortWrapper, "std_sort");
        threads[7] = thread(sortAndLog, "Descending", size, ref(stdSortFile), DESC, stdSortWrapper,"std_sort");
        threads[8] = thread(sortAndLog, "Random", size, ref(stdSortFile), RAND, stdSortWrapper,"std_sort");

        for (auto& th : threads) {
            th.join();
        }
    }

    quickFile.close();
    heapFile.close();
    stdSortFile.close();

    return 0;
}

