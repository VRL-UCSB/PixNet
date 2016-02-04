///////////////////////////////////////////////////Chi Function-----d(x,y) =  [sum( (xi-yi)^2 / (xi+yi) ) / 2]////////////////////////////////////////////
#include<iostream>
using namespace std;

float chi(float *feature,float *comp_feature, int dim)
{
	float temp_dist=0;
	float up;
	float down;
	for(int j=0;j<dim;j++)
	{			
		down=feature[j]+comp_feature[j];
		if (down!=0)
		{   
			up=(feature[j]-comp_feature[j]) * (feature[j]-comp_feature[j]);
			temp_dist=temp_dist+(up)/(down);
		}		
	}

	return temp_dist;
}