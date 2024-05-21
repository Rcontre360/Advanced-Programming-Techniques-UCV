
#include <cmath>
#include <vector>
#include "../heap_def.h"

using namespace std;

#ifndef _FIB_HEAP_H
#define _FIB_HEAP_H

struct FibHeapNode
{
	int key; // assume the element is int...
	FibHeapNode* left;
	FibHeapNode* right;
	FibHeapNode* parent;
	FibHeapNode* child;
	int degree;
	bool mark;
	int id; // the vertex id
};

		
class FibHeap: public IHeap<int> {
public:
	FibHeapNode* m_minNode;
	int m_numOfNodes;

	FibHeap(){  // initialize a new and empty Fib Heap
		m_minNode = nullptr;
		m_numOfNodes = 0;
	}

	~FibHeap() {
		_clear(m_minNode);
	}

	/* Insert a node with key value new_key
	   and return the inserted node*/
	//FibHeapNode* insert(int newKey);
	// to comply with our IHeap def
	void insert(const int &val){
		FibHeapNode* newNode = _create_node(val);
		_insert_node(newNode);
	}

	FibHeapNode* insertRet(const int &val){
		FibHeapNode* newNode = _create_node(val);
		_insert_node(newNode);

		return newNode;
	}

	/* Merge current heap with another*/  	
	void merge(FibHeap &another){
		m_minNode = _merge(m_minNode, another.m_minNode);
		m_numOfNodes += another.m_numOfNodes;
		another.m_minNode = nullptr; // so that another 
		another.m_numOfNodes = 0;
	}

	/* Return the key of the minimum node*/				
	//int  removeMinimum();  RENAMED
	int removeMinimum(){
		FibHeapNode* minNode = _extract_min_node();
		int ret = minNode->key;
		delete minNode;
		return ret;
	}

	/* Decrease the key of node x to newKey*/					
	void decrease_key(FibHeapNode* x, int newKey){
		_decrease_key(x, newKey);
	}

	/*Delete a specified node*/
	void delete_node(FibHeapNode* x){
		_decrease_key(x, m_minimumKey);
		removeMinimum();
	}

// private:
	static const int m_minimumKey =0x80000000; 

//removed to comply with our def
//FibHeapNode* FibHeap::insert(int newKey)
//{
	//FibHeapNode* newNode = _create_node(newKey);
	//_insert_node(newNode);
	//return newNode;
//}
//
	FibHeapNode* _create_node(int newKey){
		FibHeapNode* newNode = new FibHeapNode;
		newNode->key = newKey;
		newNode->left = newNode;
		newNode->right = newNode;
		newNode->parent = nullptr;
		newNode->child = nullptr;
		newNode->degree = 0;
		newNode->mark = false;
		return newNode;
	}
	void _insert_node(FibHeapNode* newNode){
		m_minNode = _merge( m_minNode, newNode);
		m_numOfNodes++;
	}

	/*******************************************************************
	* Remove x from its circular list
	* Without changing the content of x
	* Without freeing x's memory space
	*******************************************************************/
	void _remove_from_circular_list(FibHeapNode* x){
			if (x->right == x)
					return;
			FibHeapNode* leftSib = x->left;
			FibHeapNode* rightSib = x->right;
			leftSib->right = rightSib;
			rightSib->left = leftSib;
			x->left = x->right = x;
	}

	FibHeapNode* _merge(FibHeapNode* a, FibHeapNode* b){
		if(a == nullptr)
			return b;
		if(b == nullptr)	
			return a;
		if( a->key > b->key ) // swap node 
		{
			FibHeapNode* temp = a;
			a = b;
			b = temp;
		}
		FibHeapNode* aRight = a->right;
		FibHeapNode* bLeft	= b->left;
		a->right = b;
		b->left = a;
		aRight->left = bLeft;
		bLeft->right = aRight;
		return a;	
	}

	void _make_child(FibHeapNode *child, FibHeapNode *parent){
			child->parent = parent;
			parent->child = _merge(parent->child, child);
			parent->degree++;
			child->mark = false;
	}

	void _consolidate(){
			int Dn = (int)(log2(m_numOfNodes) / log2(1.618));
			FibHeapNode** A = new FibHeapNode*[Dn + 1];
			for(int i = 0; i < Dn + 1; i++){
					A[i] = nullptr;
			}
			vector<FibHeapNode*> nodeList; // It needs optimization! It makes the time longer now.
			auto node = m_minNode;
			do{
					nodeList.emplace_back(node);
					node = node->right;
			} while (node != m_minNode);
			for (auto e: nodeList){
					int d = e->degree;
					_remove_from_circular_list(e);
					while(A[d] != nullptr){
							auto tmp = A[d];
							if (e->key > tmp->key){
									swap(e, tmp);
							}
							_make_child(tmp, e);
							A[d++] = nullptr;
					}
					A[e->degree] = e;
					m_minNode = e;
			}
			for (int i = 0; i < Dn + 1; i++){
					if (A[i] != nullptr && A[i] != m_minNode){
							_merge(m_minNode, A[i]);
					}
			}
			FibHeapNode* flag = m_minNode;
			FibHeapNode* iter = flag;
			do{
					if (iter->key < m_minNode->key){
							m_minNode = iter;
					}
					iter = iter->right;
			} while (iter != flag);
			delete []A;
	}

	/*make all nodes' parent nullptr in a circular list*/
	void _unparent_all(FibHeapNode* x){
		if(x == nullptr)
			return;
		FibHeapNode* y = x;
		do {
			y->parent = nullptr;
			y = y->right;
		}while(y != x);
	}

	/***********************************************************
	 * Rearrange the heap 
	 * Update the m_minNode
	 * Return the current minimum node 
	***********************************************************/
	FibHeapNode* _extract_min_node(){
		FibHeapNode* mn = m_minNode;
			if (mn == nullptr){
					return nullptr;
			}
			_unparent_all(mn->child);
			_merge(mn, mn->child);
			if (mn == mn->right){
					m_minNode = nullptr;
			}else{
					m_minNode = mn->right;
			}
			_remove_from_circular_list(mn);
			if (m_minNode != nullptr){
					_consolidate();
			}
			m_numOfNodes--;
			return mn;
	}

	void _decrease_key(FibHeapNode* x, int newKey){
		x->key = newKey;
		FibHeapNode* y = x->parent;
		if ( y != nullptr && x->key < y->key )
		{
			_cut(x, y);
			_cascading_cut(y);
		}
		if (x->key < m_minNode->key)
			m_minNode = x;
	}

	/***********************************************************************
	* Remove x from the child list of y, decrement y->degree
	* Add x to the root list, make its parent NULL
	* And clear the mark of x
	***********************************************************************/
	void _cut(FibHeapNode* x, FibHeapNode* y){
			y->child = (x == x->right ? nullptr : x->right);
			_remove_from_circular_list(x);
			y->degree--;
			_merge(m_minNode, x);
			x->parent = nullptr;
			x->mark = false;
	}

	/***********************************************************************
	* Continue cutting on the path from the decreased node to the root
		Until meet one node, which is a root or is unmarked
	***********************************************************************/
	void _cascading_cut(FibHeapNode* y){
		FibHeapNode* z = y->parent;
		if ( z != nullptr)
		{
			if( y->mark == false)
				y->mark = true;	
			else
			{
				_cut(y,z);
				_cascading_cut(z);
			}
		}
	}

	/*********************************************************************
	* t1 is used to traversal the circular list. 
		When t1 == x for the second time (the first time is at t1's initialization),
		t1 has completed the traversal.
	**********************************************************************/
	void _clear(FibHeapNode* x){
		if ( x != nullptr )
		{
			FibHeapNode* t1 = x;  
			do{
				FibHeapNode* t2 = t1; 
				t1 = t1->right;
				_clear(t2->child);
				delete t2;
			} while(t1 != x); 
		}
	}

	int getMinimum(){
		return m_minNode->key;
	}

};

#endif
