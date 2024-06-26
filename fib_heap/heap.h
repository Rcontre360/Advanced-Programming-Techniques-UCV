#include <iostream>
#include <vector>
#include "heap_def.h"

using namespace std;

#ifndef HEAP
#define HEAP

template <class T> class Heap: public IHeap<T>{
private:
  vector<T> elements = vector<T>(1);

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

  static void swim(vector<T> &arr, int i) {
    while (i > 1 && arr[ i / 2 ] > arr[i]){
      std::swap(arr[i], arr[i/2]);
      i = i/2;
    }
  }

  static void sink(vector<T> &arr,int i, int n){
    while (2*i <= n){
      int j = 2*i;

      if (j < n && arr[j]>arr[j+1]) j++;
      if (arr[i]<=arr[j]) break;

      swap(arr[i],arr[j]);
      i = j;
    }
  }

  static void heapify(vector<T> &arr){
    for (int i = arr.size()/2; i>0; i--){
      Heap::sink(arr, i, arr.size() - 1);
    }
  }

  //heap-sort
  static void sort(vector<T> &arr) {
    int n = arr.size()-1;
    Heap::heapify(arr);

    for(int i = n; i >= 1; i--) {
      swap(arr[1], arr[i]);
      Heap::sink(arr,1,--n);
    }
  }

  int size() { return elements.size(); }

  int valueOf(int indx) { return elements[indx]; }

  T &head(){
    return elements[1];
  }

  T head() const{
    return elements[1];
  }

  void merge(Heap* heap){
    for (int i=1; i < heap->elements.size(); i++){
      this->insert(heap->elements[i]);
    }
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

  void sink(int i) {
    Heap::sink(this->elements, i, this->elements.size());
  }

  void swim(int i) {
    Heap::swim(this->elements,i);
  }

  // Methods added to comply with the interface
  T getMinimum(){
    return elements[1];
  }

  T removeMinimum(){
    T el = elements[1];
    this->remove();
    return el;
  }

  void decreaseKey(int ind, T val){
    elements[ind] = val;
    swim(ind);
  }

};

#endif
