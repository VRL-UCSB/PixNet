#include <iostream>
#include "general_header.h"

using namespace std;

void add_class_similarity(float **val, int **ind, int N1, int knn, int **sup_loc, int **label, int DB1, int num_class, int L, float **val_sim, int **ind_sim)
{
	
	float *temp_sim=DynamicArray1D<float>(knn);
	
	//#pragma omp parallel for default(shared)    
	for(int i=0;i<N1;i++) 
	{				
		for(int j=0;j<knn;j++) 
		{		
			//for integrating the label information---find what image each sup_pix belongs to and give it the same label		
			int active_L=0;
			int x=0;
			while(active_L==0 && x<num_class)
			{
				if ( label[sup_loc[i][0]-1][x] * label[sup_loc[ind[i][j]][0]-1][x] == 1 )
				{
					active_L=1;
				}
				x++;
			}

			//temp_sim[j]=(exp (-1*val[i][j])) * (pow(L, active_L));   
			temp_sim[j]=(exp (-1*val[i][j])) * (active_L);   
		}

		//re-order in the increasing order for the sim score
		for(int count=0;count<knn;count++)
		{
			val_sim[i][count]=temp_sim[0];  
			ind_sim[i][count]=0;             
			for(int p=0;p<knn;p++)
			{
				if(val_sim[i][count]<temp_sim[p])   
				{
					val_sim[i][count]=temp_sim[p];   
					ind_sim[i][count]=p;            
				}
			}
			temp_sim[ind_sim[i][count]]=0; 
		}
			
	}

	FreeDynamicArray1D<float>(temp_sim);	
}