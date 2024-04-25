#include "processing.h"
#include "ran3.cpp"

#include <sstream>
#include <vector>
#include <iostream>
#include <random>

using namespace std;

class NQueensBase {
protected:
    int N;
    vector<int> mat;
    vector<bool> row;
    vector<bool> leftD;
    vector<bool> rightD;

    virtual vector<int> candidates() = 0;

    int leftDiag(int step, int pos){
        return (N - step - 1) + (pos);
    }

    int rightDiag(int step, int pos){
        return (N - step - 1) + (N - pos);
    }

    void insertQueen(int step, int pos){
        mat[step] = pos + 1;
        row[pos] = true;
        leftD[leftDiag(step,pos)] = true;
        rightD[rightDiag(step,pos)] = true;
    }
    
    void removeQueen(int step, int pos){
        mat[step] = 0;
        row[pos] = false;
        leftD[leftDiag(step,pos)] = false;
        rightD[rightDiag(step,pos)] = false;
    }

    bool validPos(int step, int pos){
        return !leftD[leftDiag(step,pos)] && !rightD[rightDiag(step,pos)] && !row[pos];
    }

public:

    NQueensBase(int n) {
        N = n;
        mat = vector<int>(n, 0); 
        row = vector<bool>(n, 0); 
        leftD = vector<bool>(2*n-1, 0); 
        rightD = vector<bool>(2*n-1, 0); 
    }

    vector<int> getMat() {
        vector<int> cpy(mat);
        return cpy;
    }

    void print() {
        for (auto x:mat){
            cout << x << " ";
        }
        cout << endl;
    }

    bool solve(int step) {
        if (step == N){
            return true;
        } 
        vector<int> cand = this->candidates();
        for (auto i:cand){
            if (validPos(step,i)){
                insertQueen(step,i);
                if (solve(step+1))
                    return true;
                removeQueen(step,i);
            }
        }
        return false;
    }
};

class NQeensFull : public NQueensBase{
    private:
        vector<int> candidates(){
            vector<int> res(N);
            for (int i=0;i < N; i++)
                res[i] = i;
            return res;
        }
    public: 
        NQeensFull(int n) : NQueensBase(n) {
        }
};

class NQeensPartial : public NQueensBase{
    private:
        int prob;
        long seed;

        int nextPos(vector<bool> &created){
            int n = created.size();
            int res;
            do{
                res = int(ran3(&seed) * 100000) % n;
            }while(created[res]);
            created[res] = true;
            return res;
        }

        vector<int> candidates(){
            vector<int> res(N * prob / 100);
            vector<bool> created(N);
            for (int i=0;i < int(res.size()); i++){
                res[i] = nextPos(created);
            }
            return res;
        }

    public: 
        NQeensPartial(int n,int _prob) : NQueensBase(n) {
            prob = _prob;
            seed = -time(0);
        }
};

string processInput(const string& input) {
    NQeensPartial* prob = new NQeensPartial(stoi(input),30);
    string res = "";

    prob->solve(0);

    for (auto el:prob->getMat()) {
        res += to_string(el) + " ";
    }
    res.pop_back();
    prob->print();

    return res;
}

bool validate(const string& output) {
    istringstream iss(output);
    vector<int> positions;
    int pos;

    // Read the positions from the string into a vector
    while (iss >> pos) {
        positions.push_back(pos - 1); // Convert to 0-based index for easier calculations
    }

    int n = positions.size();

    // Check for conflicts
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            // Check if queens are in the same row
            if (positions[i] == positions[j]) {
                return false;
            }

            // Check diagonal conflicts
            if (abs(positions[i] - positions[j]) == abs(i - j)) {
                return false;
            }
        }
    }

    return true; // No conflicts found
}

