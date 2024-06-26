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

long seed = -42;

class NQeensPartial : public NQueensBase{
    private:
        int prob;

        int nextPos(){
            return int(ran3(&seed) * 100000) % N;
        }

        vector<int> candidates(){
            vector<int> res;
            vector<bool> used(N);

            for (int i=0;i < int(N * prob / 100); i++){
                int rnd = nextPos();
                if (!used[rnd]){
                    res.push_back(rnd);
                    used[rnd] = true;
                } 
            }
            return res;
        }

    public: 
        NQeensPartial(int n,int _prob) : NQueensBase(n) {
            prob = _prob;
        }
};
