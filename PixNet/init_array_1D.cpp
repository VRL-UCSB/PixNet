#include<iostream>
#include<vector>
using namespace std;

template <typename T> 
T *init_array1D(T *dArray, int dim, T value)
{    
	#pragma omp parallel for default(shared)
	for(int i=0;i<dim;i++)
		dArray[i]=value;	

	return(dArray);
}

//explicit instantiation
template int *init_array1D<int>(int *dArray, int dim, int value);
template float *init_array1D<float>(float *dArray, int dim, float value);
template double *init_array1D<double>(double *dArray, int dim, double value);