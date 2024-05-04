#include "quick_sort.cpp"  // Ensure this has correct function signature
#include "processing.cpp"  // Ensure this has validate function
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
    if (!arr.empty()) {
        quickSort(arr, 0, arr.size() - 1);
    }
}

void heapSort(vector<int> &arr) {
    Heap<int> *heap = new Heap<int>(arr);
    for (int i = 0; i < arr.size(); i++) {
        arr[i] = heap->head();
        heap->remove();
    }
    delete heap;
}

bool validateSorting(const vector<int>& arr){
    string res;
    for (auto x : arr) res += to_string(x) + " ";
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
        cout << "INVALID SUCCKER" << endl;
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
    vector<int> sizes ;  // Example sizes, adjust as needed
    ofstream quickFile("quick_sort_benchmark.csv");
    ofstream heapFile("heap_sort_benchmark.csv");

    quickFile << "Size, Type, Time (μs)\n";
    heapFile << "Size, Type, Time (μs)\n";

    for (int i=10000; i <= 1000000; i+=10000){
        sizes.push_back(i);
    }

    for (int size : sizes) {
        thread threads[6];

        threads[0] = thread(sortAndLog, "Ascending", size, ref(quickFile), ASC, quickSortWrapper, "quick_sort");
        threads[1] = thread(sortAndLog, "Descending", size, ref(quickFile), DESC, quickSortWrapper,"quick_sort");
        threads[2] = thread(sortAndLog, "Random", size, ref(quickFile), RAND, quickSortWrapper,"quick_sort");

        threads[3] = thread(sortAndLog, "Ascending", size, ref(heapFile), ASC, heapSort,"heap_sort");
        threads[4] = thread(sortAndLog, "Descending", size, ref(heapFile), DESC, heapSort,"heap_sort");
        threads[5] = thread(sortAndLog, "Random", size, ref(heapFile), RAND, heapSort,"heap_sort");

        for (auto& th : threads) {
            th.join();
        }
    }

    quickFile.close();
    heapFile.close();

    return 0;
}

