#include <iostream>
#include "general_header.h"

using namespace std;

void knnSearch_wo_classSimilarity(float **F1, int N1, float **F2, int N2, int M, int knn, float **val, int **ind)
{
	#pragma omp parallel for default(shared)    
	for(int i=0;i<N1;i++) 
	{				

		float *feature=DynamicArray1D<float>(M);
		for (int j=0;j<M;j++)
			feature[j]=F1[i][j];
				
		float *temp_dist=DynamicArray1D<float>(N2);
		for(int k=0;k<N2;k++)                      
		{		          			
			float *comp_feature=DynamicArray1D<float>(M);
			for(int j=0;j<M;j++)
				comp_feature[j]=F2[k][j];
				
			temp_dist[k]=hellinger(feature, comp_feature, M);
			FreeDynamicArray1D<float>(comp_feature);	
		}
	
		//finding the top "knn" nearest neighbors--keeping both the index and the distance		
		for(int count=0;count<knn;count++)
		{
			val[i][count]=temp_dist[0];  
			ind[i][count]=0;            
			for(int p=0;p<N2;p++)                    ///-----DO NOT TOUCH-------//
			{
				if (val[i][count]>temp_dist[p])   
				{
					val[i][count]=temp_dist[p];  
					ind[i][count]=p;    //p+1;             
				}
			}
			temp_dist[ind[i][count]]=100;          
		}
		
	    if (i % 1000 ==0)
		   cout<<"done with i= "<<i<< endl;
	
		FreeDynamicArray1D<float>(feature);	
		FreeDynamicArray1D<float>(temp_dist);
	}

    
}