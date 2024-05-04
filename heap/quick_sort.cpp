#include <vector>
#include <iostream>

using namespace std;

template <class T>
int partition(vector<T> &arr,int low,int high)
{
  //choose the pivot
  int pivot=arr[high];
  //Index of smaller element and Indicate
  //the right position of pivot found so far
  int i=(low-1);
  
  for(int j=low;j<=high;j++)
  {
    //If current element is smaller than the pivot
    if(arr[j]<pivot)
    {
      //Increment index of smaller element
      i++;
      swap(arr[i],arr[j]);
    }
  }
  swap(arr[i+1],arr[high]);
  return (i+1);
}

// The Quicksort function Implement
template <class T>
void quickSort(vector<T> &arr,int low,int high)
{
  // when low is less than high
  if(low<high)
  {
    // pi is the partition return index of pivot
    
    int pi=partition(arr,low,high);
    
    //Recursion Call
    //smaller element than pivot goes left and
    //higher element goes right
    quickSort(arr,low,pi-1);
    quickSort(arr,pi+1,high);
  }
}
 
