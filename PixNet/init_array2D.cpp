#include<iostream>
#include<vector>
using namespace std;

template <typename T> 
T **init_array2D(T **dArray, int dim1, int dim2, T value)
{    
	#pragma omp parallel for default(shared)
	for(int i=0;i<dim1;i++)
	{
		for (int j=0;j<dim2;j++)
			dArray[i][j]=value;
	} 	

	return(dArray);
}

//explicit instantiation
template int **init_array2D<int>(int **dArray, int dim1, int dim2, int value);
template float **init_array2D<float>(float **dArray, int dim1, int dim2, float value);
template double **init_array2D<double>(double **dArray, int dim1, int dim2, double value);