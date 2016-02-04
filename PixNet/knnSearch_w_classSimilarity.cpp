#include <iostream>
#include<math.h>
#include "general_header.h"
using namespace std;


void knnSearch_w_classSimilarity(float **F1, int N1, int M, int **label1, int DB1, int num_class, int **sup_loc1, int knn, float L, float **val_sim, int **ind_sim)
{	
		
	#pragma omp parallel for default(shared)
	for(int i=0;i<N1;i++)  
	{
		
		float *feature=DynamicArray1D<float>(M);
		for (int j=0;j<M;j++)
			feature[j]=sqrt(F1[i][j]);
			
		float *temp_dist=DynamicArray1D<float>(N1);
		float *sim=DynamicArray1D<float>(N1);

	    for(int k=0;k<N1;k++)
		{		
			
			float *comp_feature=DynamicArray1D<float>(M);
			for (int x=0;x<M;x++)
				 comp_feature[x]=sqrt(F1[k][x]);
			
			temp_dist[k]=hellinger(feature, comp_feature, M);
			
			//for integrating the label information---find what image each sup_pix belongs to and give it the same label			
			int active_L=0;
			int x=0;
			while(active_L==0 && x<num_class)
			{
				if ( label1[sup_loc1[i][0]-1][x] * label1[sup_loc1[k][0]-1][x] == 1 )
				{
					active_L=1;
				}
				x++;

			}

			////if we want to give a highert weight to sim score if they share a label
			//sim[k]=(exp (-1*temp_dist[k])) * (pow(L, active_L));   
			
			////if we want to only consider sim between the one that share a label
			sim[k]=(exp (-1*temp_dist[k])) * (active_L);   

			FreeDynamicArray1D<float>(comp_feature);

		}
		
		//Finding the top "knn" nodes with highest similaity scores for each node
		for(int count=0;count<knn;count++)
		{
			val_sim[i][count]=sim[0];  
			ind_sim[i][count]=0;             
			for(int p=0;p<N1;p++)
			{
				if(val_sim[i][count]<sim[p])   
				{
					val_sim[i][count]=sim[p];   
					ind_sim[i][count]=p;         
				}
			}
			sim[ind_sim[i][count]]=0; 

		}
		

		if (i % 1000 ==0)
			cout<<"done with i= "<<i<< endl;


		FreeDynamicArray1D<float>(feature);
		FreeDynamicArray1D<float>(sim);
		FreeDynamicArray1D<float>(temp_dist);

	}
	
	

}