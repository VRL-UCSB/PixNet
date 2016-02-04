#include<iostream>
#include<vector>
using namespace std;

template <typename T> 
T **DynamicArray2D(int dim1, int dim2)
{
      T **dynamicArray;
      dynamicArray = new T*[dim1];
      for( int i = 0 ; i < dim1 ; i++ )
		  dynamicArray[i] = new T [dim2];
      return dynamicArray;
}

//explicit instantiation
template int **DynamicArray2D<int>(int dim1, int dim2);
template float **DynamicArray2D<float>(int dim1, int dim2);
template double **DynamicArray2D<double>(int dim1, int dim2);

////////////////////////////////
template <typename T>
void FreeDynamicArray2D(T** dArray)
{
      delete [] *dArray;
      delete [] dArray;
}

//explicit instantiation
template void FreeDynamicArray2D<int>(int** dArray);
template void FreeDynamicArray2D<float>(float** dArray);
template void FreeDynamicArray2D<double>(double** dArray);


