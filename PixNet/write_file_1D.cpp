#include<iostream>
#include <fstream>
using namespace std;

template <typename T> 
void write_file_1D(string FileName, T *val, int dim)
{
	ofstream file(FileName);        
	for (int i=0;i<dim;i++)   
		file << val[i]<<" ";

	file.close();
}

//explicit instantiation
template void write_file_1D<int>(string FileName, int *val, int dim);
template void write_file_1D<float>(string FileName, float *val, int dim);
template void write_file_1D<double>(string FileName, double *val, int dim);

//void write_file_1D(char* FileName, T *val, int dim)