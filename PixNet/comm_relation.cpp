#include "general_header.h"
#include "pixnet_header.h"

#include<iostream>

using namespace std;

void comm_Relation(int *net_part, int num_comm, int N1, int *row, int * col, int M_nonZ_size, float **p_cc)
{
	int **counter=DynamicArray2D<int>(num_comm,num_comm);
	counter=init_array2D(counter,num_comm,num_comm,0);

	////Find which nodes, node i is connected to, then find the communities they belong to, and increment the counter of the two communities.
	for (int i=0;i<N1;i++)
	{
		
		int comm1=net_part[i]-1;
		for(int j=0;j<M_nonZ_size;j++)
		{
			if (row[j]==i)
			{
				int comm2;			
				comm2=net_part[col[j]]-1;   
				cout<<"comm2 = "<<comm2<<endl;
				counter[comm1][comm2]++;
			}
		}
	}

	////for each comm, count the total number of connections, and the number of connections between two particular communities
	#pragma omp parallel for default(shared)
	for(int i=0;i<num_comm;i++)
	{
		//total number of coonections that community i has with other communities
		float temp_sum=0;
		for(int j=0;j<num_comm;j++)
		{
			temp_sum=temp_sum+counter[i][j];
		}

		//find the fraction of connections that comm i has with each of the other communities
		for(int j=0;j<num_comm;j++)
		{
			if (temp_sum!=0)
				p_cc[i][j]=counter[i][j]/temp_sum;
			else
				p_cc[i][j]=0;
		}

	}

	FreeDynamicArray2D<int>(counter);

}