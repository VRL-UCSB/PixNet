#include <iostream>
#include <time.h> //to show time
#include "general_header.h"

using namespace std;

int weighted_network(float **F1, int N1, int M, int **ind, float **val, int knn, int **border, int **loc, int *num_regions1, int N_conn, int DB1, int max_regions1, int *row, int *col, float *weight, int M_size)  
{		
	time_t rawtime;
	int counter=0; //total noumber of nodes connected in the network
	
	////connecting each node with its top N_conn most similar superpixels
	for(int i=0;i<N1;i++)
	{
		
		//make sure node i does not have a ZERO feature vector
		float temp_sum=0;
		for(int j=0;j<M;j++)
		{
			temp_sum=temp_sum+F1[i][j];
		}

		if (temp_sum!=0)
		{
			//connect top N_conn nodes in the network 
			for(int k=1;k<N_conn;k++)
			{
				//make sure the node that is the most similar node does not have ZERO feature vector.
				float temp_sum_nei=0;
				for (int j=0;j<M;j++)
				{					
					temp_sum_nei=temp_sum_nei+F1[ind[i][k]][j];  //ind[i][k] is the index for the kth most similar node to node i in the network
				}

				if (temp_sum_nei!=0)
				{
					row[counter]=i;
					col[counter]=ind[i][k];       
					weight[counter]=val[i][k];
					counter=counter+1;            

					//Defining the network to be symmetric
					row[counter]=ind[i][k];  
					col[counter]=i;
					weight[counter]=val[i][k];
					counter=counter+1;
					
				}
			}
		}
	}

	time (&rawtime);
	cout<<"Done with first loop in net--The time is "<<ctime (&rawtime); 


	//// Each node might be connected to less than N_conn nodes bc of omitting zero nodes.

	////Finding the avg weights associated with each node i in the network
	float *sim_weight_avg=DynamicArray1D<float>(N1);

	//Going through each node i
	#pragma omp parallel for default(shared)
	for(int i=0;i<N1;i++)
	{		
		float temp_sum=0;
		int temp_counter=0;

		//checking all indeces in row and find the ones equal to i
		while(row[temp_counter]!=-1)
		{
			if (row[temp_counter]==i)
				temp_sum=temp_sum+weight[temp_counter];
			
			temp_counter++;	
		}
		

		if (temp_counter!=0)
			sim_weight_avg[i]=temp_sum/float(temp_counter);
		else
			sim_weight_avg[i]=0;
	}
	

	time (&rawtime);
	cout<<"Done with second loop in net--The time is "<<ctime (&rawtime); 

	////coneccting each superpixels with its adjacent superpixels of each image
	for(int i=0;i<DB1;i++)
	{
		int start=loc[i][0];
		for (int j=0;j<num_regions1[i];j++)
		{
			for (int k=j+1;k<num_regions1[i];k++)  //k=j+1
			{
				////making sure there in no link already due to similarity between two nodes start+j-1, start+k-1
				int check=0;

				for (int m=0;m<M_size;m++)
				{
					//gone too far---index more than number of nodes connected based on similarity
					if (row[m]==-1)
						break;
					
					//checking between start+j-1 and start+k-1  
					if (row[m]==start+j-1)
					{
							if (col[m]==start+k-1)
							{
								check=1;  //1 means already connected
								break;   //or migh need continue to make it go to the for loop with m
							}
					
					}
					
					//////////checking between start+k-1 and start+j-1   //////This is for the case of un-symmetric network
					////////if (col[m]==start+j-1)
					////////{
					////////		if (row[m]==start+k-1)
					////////		{
					////////			check=1;  //1 means already connected
					////////			break;   //or migh need continue to make it go to the for loop with m
					////////		}
					////////}
				
				}

				////if two nodes (start+j-1 and start+k-1, start+k-1 and start+j-1) are not connected due to similarity (check=0), check the spatial adjacency
				if (check==0)
				{
					for(int m=0;m<max_regions1;m++)
					{
						//meaning that we have reached end of the border points---so, there is no boundry between the two nodes start+j-1 and start+k-1
						if (border[start+j-1][m]==0)
							break;
						
						if (border[start+j-1][m]==k+1)
						{
							//connect the two nodes in the network
							row[counter]=start+j-1;
							col[counter]=start+k-1;
							weight[counter]= ( sim_weight_avg[start+j-1] + sim_weight_avg[start+k-1] )/2;
							counter++;

							//To make it symmetric
							row[counter]=start+k-1;
							col[counter]=start+j-1;
							weight[counter]= ( sim_weight_avg[start+j-1] + sim_weight_avg[start+k-1] )/2;
							counter++;

						}
					}
				}

			}
		
		}

	}

	time (&rawtime);
	cout<<"Done with third loop in net--The time is "<<ctime (&rawtime); 
	
	int M_nonZ_size=counter;
	FreeDynamicArray1D<float>(sim_weight_avg);
	
	return (M_nonZ_size);
}