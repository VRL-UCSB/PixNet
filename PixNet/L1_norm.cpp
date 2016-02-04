#include<iostream>
#include <fstream>
using namespace std;

template<typename T>
void L1_norm(T *F, int dim)
{
	float sum_temp=0;
	for(int i=0;i<dim;i++)
		sum_temp=sum_temp+F[i];
				
	if (sum_temp != 0.0)
	{
		for(int i=0;i<dim;i++)
			F[i]=F[i]/sum_temp;				
	}

}

//explicit instantiation
template void L1_norm<int>(int *F, int dim);
template void L1_norm<float>(float *F, int dim);