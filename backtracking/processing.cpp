#include "processing.h"

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
        for (auto i:this->candidates()){
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
        mt19937 gen;
        uniform_int_distribution<> dist;

        int getPos(vector<bool> &created){
            int res;
            do{
                res = dist(gen);
            }while(created[res]);
            return res;
        }

        vector<int> candidates(){
            vector<int> res(N * prob / 100);
            vector<bool> created(N);
            for (int i=0;i < res.size(); i++)
                res[i] = getPos(created);
            return res;
        }

    public: 
        NQeensPartial(int n,int _prob) : NQueensBase(n) {
            random_device rd; 
            mt19937 _gen(rd()); 
            uniform_int_distribution<> _dist(0, n); 

            prob = _prob;
            dist = _dist;
            gen = _gen;
        }
};

int main(){
// string processInput(const string& input) {
    NQeensFull* prob = new NQeensFull(4);
    string res = "";

    prob->solve(0);

    for (auto el:prob->getMat()) {
        res += to_string(el) + " ";
    }
    res.pop_back();
    prob->print();

    //return res;
    return 0;
}

