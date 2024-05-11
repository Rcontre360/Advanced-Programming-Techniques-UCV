#include "quick_sort.h"  
#include "point.h"  
#include "heap.h"  
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

template <class T>
void quickSortWrapper(vector<T>& arr) {
    quickSort(arr, 0, arr.size() - 1);
}

template <class T>
void stdSortWrapper(vector<T>& arr) {
    sort(arr.begin() , arr.end());
}

template <class T>
void heapSortWrapper(vector<T> &arr) {
    Heap<T>::sort(arr);
}

bool validateSorting(vector<Point>& arr){
    int n = arr.size();

    //heap sort omits arr[0]
    for (int i = 2; i < n; i++) {
        if (arr[i-1] >= arr[i]) {
            return false; 
        }
    }

    return true; 
}

vector<Point> generateArray(int n, ArrayType type) {
    vector<Point> arr;
    arr.reserve(n);

    // Fill the vector with Points incrementing by 1.0 for simplicity
    for (int i = 0; i < n; ++i) {
        arr.emplace_back(static_cast<float>(i + 1), static_cast<float>(i + 1));
    }

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
double benchmarkSort(Func sortFunc, vector<Point>& arr) {
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

void sortAndLog(const string& type, int size, ofstream& file, ArrayType arrayType, void(*sortFunc)(vector<Point>&), const string& alg){
    vector<Point> arr = generateArray(size, arrayType);
    double time = benchmarkSort(sortFunc, arr);

    cout << "done: " << type << "|" << alg << "|" << size << endl;
    file << size << ", " << type << ", " << time << "\n";

    arr.clear();
}

int main() {
    vector<int> sizes;  // Example sizes, adjust as needed
    ofstream quickFile("./benchmarks/quick_sort.csv");
    ofstream heapFile("./benchmarks/heap.csv");
    ofstream stdSortFile("./benchmarks/std.csv");

    quickFile << "Size, Type, Time (μs)\n";
    heapFile << "Size, Type, Time (μs)\n";
    stdSortFile << "Size, Type, Time (μs)\n";

    for (int i=1000; i <= 10000; i+=1000){
        sizes.push_back(i);
    }
    for (int i=20000; i <= 100000; i+=10000){
        sizes.push_back(i);
    }

    for (int size : sizes) {
        thread threads[9];

        //quick sort
        threads[0] =  std::thread([=, &quickFile]() {
            sortAndLog("Ascending", size, ref(quickFile), ASC, quickSortWrapper, "quick_sort");
        });
        threads[1] = std::thread([=, &quickFile]() {
            sortAndLog("Descending", size, quickFile, DESC, quickSortWrapper, "quick_sort");
        });
        threads[2] = std::thread([=, &quickFile]() {
            sortAndLog("Random", size, quickFile, RAND, quickSortWrapper, "quick_sort");
        });

        //heap
        threads[3] = std::thread([=, &heapFile]() {
            sortAndLog("Ascending", size, heapFile, ASC, heapSortWrapper, "heap_sort");
        });
        threads[4] = std::thread([=, &heapFile]() {
            sortAndLog("Descending", size, heapFile, DESC, heapSortWrapper, "heap_sort");
        });
        threads[5] = std::thread([=, &heapFile]() {
            sortAndLog("Random", size, heapFile, RAND, heapSortWrapper, "heap_sort");
        });

        //std sort
        threads[6] = std::thread([=, &stdSortFile]() {
            sortAndLog("Ascending", size, stdSortFile, ASC, stdSortWrapper, "std_sort");
        });
        threads[7] = std::thread([=, &stdSortFile]() {
            sortAndLog("Descending", size, stdSortFile, DESC, stdSortWrapper, "std_sort");
        });
        threads[8] = std::thread([=, &stdSortFile]() {
            sortAndLog("Random", size, stdSortFile, RAND, stdSortWrapper, "std_sort");
        });


        for (auto& th : threads) {
            th.join();
        }
    }

    quickFile.close();
    heapFile.close();
    stdSortFile.close();

    return 0;
}

