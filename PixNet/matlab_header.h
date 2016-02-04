#ifndef MATLAB_HEADER_H
#define MATLAB_HEADER_H

#include <engine.h>	// Matlab Engine

class MatlabType
{
private:
	Engine *ep;

public:

	void MatlabOpen();
	void MatlabClose(){engEvalString(ep,"quit");engClose(ep);}
	void MalabCCC(){engEvalString(ep,"clear all,close all,clc");}
	void MatlabCommand(const char* command){engEvalString(ep,command);}
	//void CToMatlab(int N, int num_comm, int M_nonZ_size, int *row, int *col, float *weight, int dim);
	//void MatlabToC(int *net_part, int N1);

	template <typename T> 
	void CToMatlab(T *dArray,int dim, char *name);

	template <typename T> 
	void MatlabToC(char *name, T *dArray, int dim);

};

#endif