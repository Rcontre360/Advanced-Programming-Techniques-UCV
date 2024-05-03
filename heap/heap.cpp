#include <iostream>
#include <vector>
using namespace std;

#ifndef HEAP
#define HEAP

template <class Elem>
class IHeap<Elem>: public vector< Elem >
{
  public:
    IHeap();
    IHeap(const IHeap<Elem> &h);
    IHeap(const vector<Elem> &a);
    ~IHeap();
    void insert(const Elem &e);
    void remove();
    Elem &head(); // retorna referencia
    Elem head() const; // retorna copia
    static void sort(vector< Elem > &A); // ordena A con “<“
    void heapify(vector< Elem > &A);
    void sink(vector< Elem > &A, int k, int n); // hundir
  private:
    void swim(int k); // flotar
};

//template <class T> class Heap : public IHeap<T> {
template <class T> class Heap {
private:
  vector<T> *elements;

  int parent(int pos) {
    return (pos - 1) / 2 >= 0 && (pos - 1) / 2 < elements->size()
               ? (pos - 1) / 2
               : -1;
  }

  int left(int pos) {
    return 2 * pos + 1 >= 0 && 2 * pos + 1 < elements->size() ? 2 * pos + 1
                                                              : -1;
  }

  int right(int pos) {
    return 2 * pos + 2 >= 0 && 2 * pos + 2 < elements->size() ? 2 * pos + 2
                                                              : -1;
  }

  void swim(int i) {
    if (parent(i) != -1 && (*elements)[i] < (*elements)[parent(i)]) {
      std::swap((*elements)[i], (*elements)[parent(i)]);
      swim(parent(i));
    }
  }

  void sink(int i) {
    int mn = i;
    if (left(i) != -1 && (*elements)[left(i)] < (*elements)[i])
      mn = left(i);
    if (right(i) != -1 && (*elements)[right(i)] < (*elements)[mn])
      mn = right(i);
    if (mn != i) {
      std::swap((*elements)[i], (*elements)[mn]);
      sink(mn);
    }
  }

public:
  Heap() { elements = new vector<T>(); }

  void print() {
    while (elements->size() > 0)
      cout << remove();
  }

  int size() { return elements->size(); }

  void insert(const T &val) {
    elements->pushBack(val);
    swim(elements->size() - 1);
  }

  T remove() {
    T val = (*elements)[0];
    (*elements)[0] = elements->popBack();
    sink(0);
    return val;
  }
};

#endif
