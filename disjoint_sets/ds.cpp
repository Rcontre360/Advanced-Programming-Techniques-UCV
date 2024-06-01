#include <bits/stdc++.h>
using namespace std;

template <class T>
class DisjointSet {
private:
  // instead of vector, map to support more cases
  unordered_map<T,T> parentOf;
  // weight that measures the size of a parent
  unordered_map<T,int> weight;
  // track already inserted elements
  unordered_map<T,bool> visited;

  //goes up from x to its parents compressing. Parent must be parent of x
  void _compress(T x, T parent){
    while (_hasParent(x)){
      T temp = x;
      x = parentOf[x];
      parentOf[temp] = parent;
    }
  }

  // checks if a key has been used
  bool _hasParent(T x){
    bool res = parentOf.count(x) > 0;
    return res;
  }

public:
  DisjointSet() {}

  void print(){
    cout << "parent: ";
    for (auto x:parentOf)
      cout << "(" << x.first << " " << x.second << "), ";
    cout << endl << "weight: ";
    for (auto x:weight)
      cout << "(" << x.first << " " << x.second << "), ";
    cout << endl;
  }

  T find(T node){
    // here the unordered_map weirdly assigns to parentOf[node]
    // so to avoid that we first check if node has a parent
    if (!_hasParent(node))
      return node;

    T parent = parentOf[node];
    while(_hasParent(parent)){parent = parentOf[parent];}

    return parent;
  }

  // get the size of a group given a member of it
  int groupSize(T node){
    int _weight = weight[find(node)]; 
    return _weight > 0 ? _weight : 1;
  }

  bool pertenencia(T x, T y, bool u) {
    T i=x, j=y;

    //get parents
    while(_hasParent(i)) i=parentOf[i]; 
    while(_hasParent(j)) j=parentOf[j];

    //compress given node and parents
    _compress(x,i);
    _compress(y,j);

    //assign if needed
    if (u && (i!=j)) {
      if (weight[j] < weight[i]){ 
        weight[j]+=weight[i]+1; 
        weight[i] = 0;
        parentOf[i]=j; 
      }else { 
        weight[i]+=weight[j]+1; 
        weight[j] = 0;
        parentOf[j]=i; 
      }
    }

    return i!=j;
  }
};


//SPOJ: FOXLINGS - Foxlings
int main(){
  int t;

  cin >> t;

  while (t--){
    int n;
    string a,b;
    DisjointSet<string>* ds = new DisjointSet<string>();
    unordered_map<string,bool> visited;

    cin >> n;

    while (n--){

      cin >> a >> b;

      if (visited[a] && visited[b]) //already set both
        continue;
      if (visited[a]) //only node set, so it will be the parent
        swap(a,b);

      ds->pertenencia(a,b,true);

      cout << ds->groupSize(a) + 1 << endl;
    }
  }
  

  return 0;
}
