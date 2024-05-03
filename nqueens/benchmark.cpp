#include "processing.cpp"
#include <thread>
#include <fstream>
#include <vector>
#include <iostream>

#define START_N 8
#define END_N 50
#define RUNS_ON_N 1
#define PARTIAL 30
#define FULL_RESULTS "benchmark/deterministic.csv"
#define PARTIAL_RESULTS "benchmark/randomized.csv"

using namespace std;
using namespace std::chrono;

struct TestResult {
    float runTime;
    int successfulRuns;
};

TestResult singleTest(int n, bool isFull) {
    TestResult res = TestResult{0,0};

    NQueensBase* problem = isFull ? static_cast<NQueensBase*>(new NQeensFull(n))
                                  : static_cast<NQueensBase*>(new NQeensPartial(n, PARTIAL));

    auto start = high_resolution_clock::now();

    if (problem->solve(0)) {
        auto end = high_resolution_clock::now();
        duration<double, milli> runTime = end - start;

        string result;
        for (auto el : problem->getMat()) {
            result += to_string(el) + " ";
        }
        result.pop_back();
        
        if (validate(result)) {
            res.successfulRuns++;
        }

        res.runTime = float(runTime.count());
    }

    delete problem;

    return res;
}

void runBenchmark(const string& filename, bool isFull) {
    ofstream file(filename);
    file << "N, Average Time (ms), Success Rate, Total Runs\n";

    for (int n = START_N; n <= END_N; n++) {
        double totalDuration = 0;
        int successfulRuns = 0;

        cout << "SOLVING: " << n << endl;
        for (int i = 0; i < RUNS_ON_N; i++) {
            TestResult res = singleTest(n,isFull);
            totalDuration += res.runTime;
            successfulRuns += res.successfulRuns;
        }

        cout << "WHAAT " << " " << successfulRuns << endl;
        file << n << ", "
                   << double(totalDuration / RUNS_ON_N) << " ms, "
                   << (successfulRuns * 100.0 / RUNS_ON_N) << "%, "
                   << RUNS_ON_N << "\n";
    }

    file.close();
}

int main() {
    //std::thread fullThread(runBenchmark, FULL_RESULTS, true);
    std::thread partialThread(runBenchmark, PARTIAL_RESULTS, false);

    //fullThread.join();
    partialThread.join();

    return 0;
}

