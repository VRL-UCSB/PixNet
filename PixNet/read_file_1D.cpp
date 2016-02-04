#include<iostream>
#include <fstream>
using namespace std;

template <typename T> 
void read_file_1D(string FileName, T *F1, int dim)
{
			int i;	
            ifstream file(FileName);
            
			for (i=0;i<dim;i++)
					file>> F1[i];		
         
            file.close();
}

//explicit instantiation
template void read_file_1D<int>(string FileName, int *F1, int dim);
template void read_file_1D<float>(string FileName, float *F1, int dim);
template void read_file_1D<double>(string FileName, double *F1, int dim);

//void read_file_1D(char* FileName, T *F1, int dim)