#include <iostream>
#include <vector>
using namespace std;

#ifndef HEAP
#define HEAP

//template <class Elem>
//class IHeap<Elem>: public vector< Elem >
//{
  //public:
    //IHeap();
    //IHeap(const IHeap<Elem> &h);
    //IHeap(const vector<Elem> &a);
    //~IHeap();
    //void insert(const Elem &e);
    //void remove();
    //Elem &head(); // retorna referencia
    //Elem head() const; // retorna copia
    //static void sort(vector< Elem > &A); // ordena A con “<“
    //void heapify(vector< Elem > &A);
    //void sink(vector< Elem > &A, int k, int n); // hundir
  //private:
    //void swim(int k); // flotar
//};

//template <class T> class Heap : public IHeap<T> {
template <class T> class Heap {
private:
  vector<T> elements = vector<T>(1);

  int parent(int pos) {
    return (pos - 1) / 2 >= 0 && (pos - 1) / 2 < elements.size()
               ? (pos - 1) / 2
               : -1;
  }

  int left(int pos) {
    return 2 * pos + 1 >= 0 && 2 * pos + 1 < elements.size() ? 2 * pos + 1
                                                              : -1;
  }

  int right(int pos) {
    return 2 * pos + 2 >= 0 && 2 * pos + 2 < elements.size() ? 2 * pos + 2
                                                              : -1;
  }

  void swim(int i) {
    while (i > 1 && elements[ i / 2 ] > elements[i]){
      std::swap(elements[i], elements[i/2]);
      i = i/2;
    }
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

public:
  Heap() { }

  int size() { return elements.size(); }

  void insert(const T &val) {
    elements.push_back(val);
    swim(elements.size() - 1);
  }

  T remove() {
    T val = elements[1];
    elements[1] = elements[elements.size() - 1];
    elements.pop_back();
    sink(1);
    return val;
  }
};

#endif
