#include "processing.h"

#include <vector>
#include <iostream>

using namespace std;

class NQueens {
private:
    int N;
    std::vector<int> mat;
    std::vector<bool> row;
    std::vector<bool> leftD;
    std::vector<bool> rightD;

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
    NQueens(int n) {
        N = n;
        mat = std::vector<int>(n, 0); 
        row = std::vector<bool>(n, 0); 
        leftD = std::vector<bool>(2*n-1, 0); 
        rightD = std::vector<bool>(2*n-1, 0); 
    }

    std::vector<int> getMat() {
        std::vector<int> cpy(mat);
        return cpy;
    }

    void print() {
        for (auto x:mat){
            std::cout << x << " ";
        }
        std::cout << std::endl;
    }

    bool solve(int step) {
        if (step == N){
            return true;
        } 
        for (int i=0;  i < N; i++){
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

std::string processInput(const std::string& input) {
    NQueens prob(std::stoi(input));
    string res = "";

    prob.solve(0);

    for (auto el:prob.getMat()) {
        res += std::to_string(el) + " ";
    }
    res.pop_back();

    return res;
}

