#include "quick_sort.cpp"
#include "processing.cpp"
#include <chrono>
#include <fstream>
#include <random>
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;
using namespace chrono;

enum ArrayType { ASC, DESC, RAND };

bool validateSorting(vector<int> arr){
    string res;
    for (auto x:arr) res += to_string(x);
    return validate(res);
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
        throw std::runtime_error("Not sorted correctly");
    }
    
    duration<double, micro> elapsed = end - start;
    return elapsed.count();
}

void heapSort(vector<int> &arr) {
    Heap<int> heap;
    for (int num : arr) {
        heap.insert(num);
    } 
    for (int i = 0; i < arr.size(); i++) {
        arr[i] = heap.head();
        heap.remove();
    }
}

void quickSortWrapper(vector<int> &arr) {
    quickSort(arr, 0, arr.size() - 1);
}

int main() {
    vector<int> sizes;
    string sortTypes[3] = {"Ascending", "Descending", "Random"};
    
    for (int i=10000; i <= 1000000; i+=10000){
        sizes.push_back(i);
    }

    ofstream quickFile("quick_sort_benchmark.csv"), heapFile("heap_sort_benchmark.csv");
    quickFile << "Size, Type, Time (μs)\n";
    //heapFile << "Size, Type, Time (μs)\n";

    for (int size : sizes) {
        cout << "SIZE " << size << endl;
        for (int type = 0; type < 1; ++type) {
            vector<int> arr = generateArray(size, static_cast<ArrayType>(type));
            vector<int> arrCopy = arr; 

            double timeQuick = benchmarkSort(quickSortWrapper, arr);
            //double timeHeap = benchmarkSort(heapSort, arrCopy);

            quickFile << size << ", " << sortTypes[type] << ", " << timeQuick << "\n";
            //heapFile << size << ", " << sortTypes[type] << ", " << timeHeap << "\n";
        }
    }

    quickFile.close();
    //heapFile.close();
    return 0;
}

