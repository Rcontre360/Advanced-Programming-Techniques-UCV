#include <iostream>
#include <vector>
using namespace std;

#ifndef HEAP
#define HEAP

template <class T> class Heap {
private:
  vector<T> elements = vector<T>(1);

  void swim(int i) {
    while (i > 1 && elements[ i / 2 ] > elements[i]){
      std::swap(elements[i], elements[i/2]);
      i = i/2;
    }
  }

public:
  Heap() { }

  Heap(const Heap<T> &h){
    for (int i=1; i < h.elements.size(); i++){
      this->insert(h.elements[i]);
    }
  }

  Heap(const vector<T> &a){
    for (auto number:a)
        this->insert(number);
  }

  ~Heap(){
    elements.clear();
  }

  int size() { return elements.size(); }

  T &head(){
    return elements[1];
  }

  T head() const{
    return elements[1];
  }

  void insert(const T &val) {
    elements.push_back(val);
    swim(elements.size() - 1);
  }

  void remove() {
    elements[1] = elements[elements.size() - 1];
    elements.pop_back();
    sink(1);
  }

  void heapify(){
    for (int i = elements.size()/2; i>0; i--)
      sink( i);
  }

  void sink(int i) {
    int n = elements.size();
    while (2*i <= n){
      int j = 2*i;

      if (j < n && elements[j]>elements[j+1]) j++;
      if (elements[i]<=elements[j]) break;

      swap(elements[i],elements[ j]);
      i = j;
    }
  }

};

#endif
