#include<iostream>
#include <fstream>
using namespace std;

float p_appear(int knn_consider, int *net_part, int **ind, int row_nei, int cc)
{
	//find that out of knn, how many belong to community cc
	int counter=0;
	for(int j=0;j<knn_consider;j++)
	{
		
		if(net_part[ind[row_nei][j]]==(cc+1))              //if(net_part[ind[row_nei][j]-1]==(cc+1))
			counter++;
	}

	float p_app=float(counter)/float(knn_consider);

	return(p_app);
}