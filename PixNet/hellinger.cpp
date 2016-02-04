#include<iostream>
using namespace std;

 float hellinger(float *feature, float *comp_feature, int dim)
{	
	////////////////////L1 normalize the features////////////////
	//float sum_feature=0;
	//float sum_comp_feature=0;
	//for(int j=0;j<dim;j++)
	//{			
	//	sum_feature=sum_feature+feature[j];
	//	sum_comp_feature=sum_comp_feature+comp_feature[j];
	//}

	//if (sum_feature!=0)
	//{
	//	for(int j=0;j<dim;j++)
	//	{			
	//		feature[j]=feature[j]/sum_feature;
	//	}
	//}
	//
	//if (sum_comp_feature!=0)
	//{
	//	for(int j=0;j<dim;j++)
	//	{			
	//		comp_feature[j]=comp_feature[j]/sum_comp_feature;
	//	}
	//}
	//	
	//////////////////take the sqrt elementwise////////////////
	//for(int j=0;j<dim;j++)
	//{			
	//	feature[j]=sqrt(feature[j]);
	//	comp_feature[j]=sqrt(comp_feature[j]);
	//}
	//
	////////////////////finding the euclidean distance of the two features////////////////
	float temp_dist=0;
	for(int j=0;j<dim;j++)
	{
	   temp_dist=temp_dist+((feature[j]-comp_feature[j])*(feature[j]-comp_feature[j]));
	}
	
	temp_dist=sqrt(temp_dist);

	return temp_dist;
}