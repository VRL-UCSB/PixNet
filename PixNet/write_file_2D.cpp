#include<iostream>
#include <fstream>
using namespace std;

template <typename T> 
void write_file_2D(string FileName, T **val, int dim1, int dim2)
{
	ofstream file(FileName);        
	for (int i=0;i<dim1;i++)   
	{
		for (int j=0;j<dim2;j++)
			file << val[i][j]<<" ";
		file<<endl;
	}
		
	file.close();
}

//explicit instantiation
template void write_file_2D<int>(string FileName, int **val, int dim1, int dim2);
template void write_file_2D<float>(string FileName, float **val, int dim1, int dim2);
template void write_file_2D<double>(string FileName, double **val, int dim1, int dim2);

//void write_file_2D(char* FileName, T **val, int dim1, int dim2);