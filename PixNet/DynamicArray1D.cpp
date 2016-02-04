#include<iostream>
using namespace std;

template <typename T> 
T *DynamicArray1D(int dim)
{
      T *dynamicArray;
	  dynamicArray = new T[dim];
      return dynamicArray;
}
 
//explicit instantiation
template int *DynamicArray1D<int>(int dim);
template float *DynamicArray1D<float>(int dim);
template double *DynamicArray1D<double>(int dim);

/////////////////////////////////////////////////////
template <typename T>
void FreeDynamicArray1D(T* dArray)
{      
      delete [] dArray;
}

//explicit instantiation
template  void FreeDynamicArray1D <int>(int* dArray);
template  void FreeDynamicArray1D <float> (float* dArray);
template  void FreeDynamicArray1D <double> (double* dArray);