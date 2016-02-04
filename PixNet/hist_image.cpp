#include<iostream>
#include <fstream>
using namespace std;

void Hist_image(int DB2, int *num_regions, int **loc, float **w, float **hist, int num_comm, int pooling)
{	
	
	if (pooling==0)
	{
		///////////SUM Pooling/////////////
		cout<<"Sum Pooling"<<endl;
		for(int i=0;i<DB2;i++)  
		{
			for(int k=0;k<num_comm;k++)
			{			
				hist[i][k]=0;			
				////////////sum pooling////////////
				for(int j=0;j<num_regions[i];j++)
				{
					////////////find the row number for w////////////
					int row=loc[i][j]-1;
					hist[i][k]=hist[i][k]+w[row][k];				
				}
			}
		}
	}
	else
	{
		/////////////MAX Pooling/////////////
		cout<<"Max Pooling"<<endl;
		for(int i=0;i<DB2;i++)  
		{
			for(int k=0;k<num_comm;k++)
			{			
				////////////find the row number for w////////////
				int row=loc[i][0]-1;			
				hist[i][k]=w[row][k];
				
				////////////max pooling////////////
				for(int j=0;j<num_regions[i];j++)
				{
					row=loc[i][j]-1;
					if (hist[i][k]<w[row][k])
						hist[i][k]=w[row][k];
				}			
			}
		}
	}
}