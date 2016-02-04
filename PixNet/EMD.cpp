///////////////////////////////////Earth_Movers_Distance Function------d(x,y) = sum(abs(cdf(x)-cdf(y)))  where cdf is computed by cumsum///////////////////
#include<iostream>
using namespace std;

float EMD(float *feature, float *comp_feature, int dim)
{
	float temp_dist=0;
	float cdf_feature=0;
	float cdf_comp_feature=0;

	for(int i=0;i<dim;i++)
	{
		cdf_feature=cdf_feature+feature[i];
		cdf_comp_feature=cdf_comp_feature+comp_feature[i];
		temp_dist=temp_dist+abs( cdf_feature-cdf_comp_feature );
	}

	return temp_dist;
}