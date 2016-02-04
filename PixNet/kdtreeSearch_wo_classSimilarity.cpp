////C++: void flann::Index_<T>::knnSearch(const vector<T>& query, vector<int>& indices, vector<float>& dists, int knn, const SearchParams& params)
////C++: void flann::Index_<T>::knnSearch(const Mat& queries, Mat& indices, Mat& dists, int knn, const SearchParams& params)
//
//#include <iostream>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include <sys/stat.h>
#include <conio.h>
#include <time.h> //to show time
#include "general_header.h"

using namespace cv;
using namespace std;

void kdtreeSearch_wo_classSimilarity(float **F1, int N1, float **F2, int N2, int M, int knn, float **val, int **ind)
{		
	//Mat matrix = Mat(numOfRows,numOfCols,CV_32FC1,&F1);
	time_t rawtime;

	/////////////////cv_feature/////////////////
	//Define a variable cv_feature of class Mat
	Mat cv_feature = Mat(N1,M,CV_32F);	 	
	#pragma omp parallel for default(shared)
	for(int i=0;i<N1;i++) 
		for(int j=0;j<M;j++)
			cv_feature.at<float>(i,j)=sqrt(F1[i][j]);
	
	cout<<"done with defining cv_feature"<<endl;
	
	/////////////////cv_query/////////////////
	//Define a variable cv_feature of class Mat
	Mat cv_query = Mat(N2, M, CV_32F);
	#pragma omp parallel for default(shared)
	for(int i=0;i<N2;i++) 
		for(int j=0;j<M;j++)
			cv_query.at<float>(i,j)=sqrt(F2[i][j]);

	cout<<"done with defining cv_query"<<endl;
	/////////////////define index and dist Matrices/////////////////
	Mat index(N2, knn, CV_32S);
	Mat dist(N2, knn, CV_32F);
	/////////////////knnsearch using kdtree/////////////////
	/////The returned distances using knnsearch are squared euclidean distance.////////
	
	//cv::flann::AutotunedIndexParams IndexParams(0.96, 0.01, 0, 0.1);   //with 256 takes less than 3 minutes (171 seconds for 107038 queries)  \\0.96 gave good results

	//////q150:  (0.96, 0.1, 0, 0.4)
	cv::flann::AutotunedIndexParams IndexParams(0.94, 0.01, 0, 0.1);   //with 256 takes less than 3 minutes (171 seconds for 107038 queries)  \\0.96 gave good results   0.1
	time (&rawtime);	
	cout<<"done with step 1 "<<ctime (&rawtime)<<endl;
	cv::flann::Index kdtree(cv_feature, IndexParams);  // using 5 randomized kdtrees	
	time (&rawtime);	
	cout<<"done with step 2 "<<ctime (&rawtime)<<endl;
	kdtree.knnSearch(cv_query, index, dist, knn, cv::flann::SearchParams(256) ); // maximum number of leafs checked    
	time (&rawtime);	
	cout<<"done with step 3 "<<ctime (&rawtime)<<endl;

	/////////////////saving values of index and dist/////////////////
	#pragma omp parallel for default(shared)
	for (int i=0;i<N2;i++)
	{
		for(int j=0;j<knn;j++)
		{
			val[i][j]=dist.at<float>(i,j); 
			ind[i][j]=index.at<int>(i,j); 
		}
	}
	
}
