#include<iostream>
#include "engine.h"		// Matlab Engine
#include <Windows.h>	//Message Box

#include "general_header.h"
#include "matrix.h"
#include "matlab_header.h"

using namespace std;
/////////////////////////////////////////////////////
void MatlabType::MatlabOpen()
{
		ep=engOpen("null");  //engOpen to open Matlab
		if (!(ep = engOpen(NULL))) 
		{
		MessageBox ((HWND)NULL, (LPSTR)"Can't start MATLAB engine", 
			(LPSTR) "Matlab Engine Error", MB_OK);
		exit(-1);
		}		
}
/////////////////////////////////////////////////////
//void MatlabType::CToMatlab(int N, int num_comm, int M_nonZ_size, int *row, int *col, float *weight, int M_size)
template <typename T> 
void MatlabType::CToMatlab(T *dArray,int dim, char *name)
{	
	//convert to double
	double *temp=DynamicArray1D<double>(dim);
	#pragma omp parallel for default(shared)
	for(int i=0;i<dim;i++)
		temp[i]=double(dArray[i]);	
	
	//send to matlab
	mxArray *xArray= mxCreateDoubleMatrix(1,dim,mxREAL);    	
	memcpy((void *) mxGetPr(xArray), (void *) temp, dim*sizeof(double)); 	
	engPutVariable(ep,name,xArray); 
	
	//free memory
	mxDestroyArray(xArray);	
	FreeDynamicArray1D(temp);

}

template void MatlabType::CToMatlab<int>(int *dArray,int dim, char *name);
template void MatlabType::CToMatlab<float>(float *dArray,int dim, char *name);
template void MatlabType::CToMatlab<double>(double *dArray,int dim, char *name);

///////////////////////////////////////////////////////
//Get array from matlab to C++ (non-sparse arrays).
template <typename T> 
void MatlabType::MatlabToC(char *name, T* dArray, int dim)
{
	
	mxArray *y= mxCreateDoubleMatrix(1,dim,mxREAL);
	y=engGetVariable(ep, name);	

	//mxGetPr will returns pointer to double
	double *temp_dArray;
	temp_dArray=(double*)mxGetPr(y); //name of the variable after imported from matlab
	
	//convert the data to type T
	#pragma omp parallel for default(shared)
	for(int i=0;i<dim;i++)
		dArray[i]=T(temp_dArray[i]);	
	
	//Free memory
	mxDestroyArray(y);	
}

template void MatlabType::MatlabToC<int>(char *name, int *dArray, int dim);
template void MatlabType::MatlabToC<float>(char *name, float *dArray, int dim);
template void MatlabType::MatlabToC<double>(char *name, double *dArray, int dim);
//////////////////////////////////////////////////
