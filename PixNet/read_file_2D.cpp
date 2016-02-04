#include<iostream>
#include <fstream>
using namespace std;

template <typename T> 
void read_file_2D(string FileName, T **F2, int dim1, int dim2)
{
			int i,j;	
            ifstream file(FileName);
            
			for (i=0;i<dim1;i++)
			{
				for (j=0;j<dim2;j++)
				{
					file>> F2[i][j];		
				}            
			}
            file.close();
}

//explicit instantiation
template void read_file_2D<int>(string FileName, int **F2, int dim1, int dim2);
template void read_file_2D<float>(string FileName, float **F2, int dim1, int dim2);
template void read_file_2D<double>(string FileName, double **F2, int dim1, int dim2);

//void read_file_2D(char* FileName, T **F2, int dim1, int dim2)