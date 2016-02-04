#include<iostream>
#include <fstream>

#include "pixnet_header.h"
#include "general_header.h"

using namespace std;

void Hist_node(int N, int **sup_loc, int **border, int **loc, int *net_part, float **w1, float **w2, float **w, int **ind, float **p_cc, int knn_consider, int num_comm, int max_regions)
{

	#pragma omp parallel for default(shared)
	for (int i=0;i<N;i++)  
	{
		//float feature[M];
		//for (int j=0;j<M;j++)			
			//feature[j]=sqrt(F_train[i][j]);

		////////////find what image node i belongs to//////////
		int image=sup_loc[i][0]-1;
		int region=sup_loc[i][1];

		////////////find the neighbors of node i//////////		
		for(int c=0;c<num_comm;c++)  
		{			
			float p_app;

			int count=0;
			while(border[ i ][count]!=0 && count<max_regions)    //border[ row_num ][count]!=0
			{
				////////////get the region number associated with the neighbor//////////
				int region_nei;
				region_nei=border[i][count]-1;   		

				////////////find its corresponding row number in F//////////
				int row_nei;
				row_nei=loc[image][region_nei]-1;
				////////////find the corresponidng feature//////////
				//float feature_nei[M];
				//for(int j=0;j<M;j++)
				//	feature_nei[j]=sqrt(F_train[row_nei][j]);
				
				////////////find p(j in com)//////////
				for (int cc=0;cc<num_comm;cc++)	
				{						
					p_app=p_appear(knn_consider, net_part, ind, row_nei, cc);					
					w1[i][c]=w1[i][c]+p_cc[c][cc]*p_app;						
				}
				
				p_app=p_appear(knn_consider, net_part, ind, i, c);				
				w2[i][c]=p_app;				
				count++;
			}
		}

		//normalizing w1
		L1_norm(w1[i],num_comm);
		
		//fill out w
		for(int counter=0;counter<num_comm;counter++)
			w[i][counter]=(w1[i][counter]+w2[i][counter])/2;

		//normalizing w
		L1_norm(w[i],num_comm);
		

	}

}