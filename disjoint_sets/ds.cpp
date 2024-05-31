#include <iostream>
#include <vector>
using namespace std;

class DisjointSet {
private:
  vector<int> parentOf;

  //goes up from x to its parents compressing. Parent must be parent of x
  void _compress(int x, int parent){
    while (parentOf[x] > 0){
      int temp = x;
      x = parentOf[x];
      parentOf[temp] = parent;
    }
  }

public:
  DisjointSet(int size) { 
    parentOf = vector<int>(size);
  }

  int count_sets(){
    int res = 0;
    for (auto x:parentOf)
      res += x <= 0? 1 : 0;
    return res-1; // minus pos 0 bc its not used
  }

  void print(){
    cout << "print" << endl;
    for (auto x:parentOf)
      cout << x << " ";
    cout << endl;
  }

  int find(int node){
    int parent = parentOf[node];
    while(parentOf[parent] > 0){parent = parentOf[parent];}
    return parent;
  }

  int pertenencia(int x, int y, bool u) {
    int i=x, j=y;

    while(parentOf[i]>0) i=parentOf[i]; 
    while(parentOf[j]>0) j=parentOf[j];

    _compress(x,i);
    _compress(y,j);

    if (u && (i!=j)) {
      if (parentOf[j] < parentOf[i]){ 
        parentOf[j]+=parentOf[i]-1; 
        parentOf[i]=j; 
      }else { 
        parentOf[i]+=parentOf[j]-1; 
        parentOf[j]=i; 
      }
    }

    return i!=j;
  }

  void insert(int node,int parent){
    parentOf[node] = parent; //set element parent
    while (parentOf[parent]>0) parent = parentOf[parent]; //search parent
    _compress(node,parent);
    parentOf[parent]-=1; //increase parents balance
  }
};


//SPOJ: FOXLINGS - Foxlings
int main(){
  int n,m;

  cin >> n >> m;

  DisjointSet* ds = new DisjointSet(n);
  
  for (int i=0;i < m; i++){
    int a,b;
    cin >> a >> b;
    ds->insert(a,b);
  }

  ds->print();
  cout << ds->count_sets();

  return 0;
}
