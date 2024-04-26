#include "processing.cpp"
#include <fstream>
#include <chrono>
#include <vector>
#include <iostream>
#include <ctime>

using namespace std;
using namespace std::chrono;

void runBenchmark(int nStart, int nEnd, const string& filename, bool isFull) {
    ofstream resultFile(filename);
    resultFile << "N, Average Time (ms), Success Rate, Total Runs\n";

    for (int n = nStart; n <= nEnd; n++) {
        double totalDuration = 0;
        int successfulRuns = 0;
        int totalRuns = 10;

        for (int i = 0; i < totalRuns; i++) {
            auto start = high_resolution_clock::now();

            NQueensBase* problem = isFull ? static_cast<NQueensBase*>(new NQeensFull(n))
                                          : static_cast<NQueensBase*>(new NQeensPartial(n, 30));

            if (problem->solve(0)) {
                auto end = high_resolution_clock::now();
                duration<double, milli> runTime = end - start;

                if (runTime.count() > 40000) { 
                    resultFile << "Test terminated due to time limit.\n"; break;
                }

                string result;
                for (auto el : problem->getMat()) {
                    result += to_string(el) + " ";
                }
                result.pop_back();
                
                if (validate(result)) {
                    successfulRuns++;
                }

                totalDuration += runTime.count();
            }

            delete problem;
        }

        resultFile << n << ", "
                   << (totalDuration / totalRuns) << " ms, "
                   << (static_cast<double>(successfulRuns) / totalRuns * 100.0) << "%, "
                   << totalRuns << "\n";
    }

    resultFile.close();
}

int main() {
    int nStart = 8, nEnd = 25;

    runBenchmark(nStart, nEnd, "full_results.csv", true);

    runBenchmark(nStart, nEnd, "partial_results.csv", false);

    return 0;
}

