#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include<math.h>
#include<conio.h> //to make the output console wait
#include <time.h> //to show time
#include<cmath>

#include "general_header.h"
#include "pixnet_header.h"
#include "matlab_header.h"

using namespace std;


int main(int argc, char** argv)
{   	
	///////////////////////////////////////////////////////////////////////Defining variables////////////////////////////////////////////////////////////////
	time_t rawtime;
	int		knn=100;
	int		N1 = stoi( argv[1] );		 
	int		N2	= stoi( argv[2] );        
	int		M  = stoi( argv[3] );		  
	int		DB1 = stoi( argv[4] );        
	int		num_class = stoi( argv[5] );     
	float	L = stoi( argv[6] );     
	int pooling=0; // pooling=0 is for sum pooling and pooling=1 is for max pooling

	string seg_level=argv[7];
	string DB_name=argv[8];
	string Feature=argv[9];

	float **F1=DynamicArray2D<float>(N1, M);
	float **F2=DynamicArray2D<float>(N2, M);
		
	////--only for network creation--for using class similarity--this is used.
	int **label1=DynamicArray2D<int>(DB1, num_class);
	int **sup_loc1=DynamicArray2D<int>(N1, 2);
	
	//For network creation only
	int max_regions1=stoi( argv[10] ); 
	int N_conn=stoi( argv[11] );;      
	int M_size= N1*50; //make sure the size N1*50 is safe. /////////MAYBE GIVE THIS AS INPUT!!!!!!!!!!!!!!!!!!!!!!!!

	int **border1=DynamicArray2D<int>(N1, max_regions1);
	int **loc1=DynamicArray2D<int>(DB1,max_regions1);
	int *num_regions1=DynamicArray1D<int>(DB1);
	
	//for Ncut only
	int num_comm=stoi( argv[12] ); ;    
	
	//for Hist_image
	int knn_consider=stoi( argv[13] );;  


	int DB2=stoi( argv[14] );;           
	int max_regions2=stoi( argv[15] );    
	
	int** border2=DynamicArray2D<int>(N2,max_regions2);
	int** sup_loc2=DynamicArray2D<int>(N2,2);
	int** loc2=DynamicArray2D<int>(DB2, max_regions2);	
	int *num_regions2=DynamicArray1D<int>(DB2);

	time (&rawtime);
	cout<<"--Done with defining variables--The time is "<<ctime (&rawtime); 
	
	///////////////////////////////////////////////////////////////////////Reading in values/////////////////////////////////////////////////////////////////	
    	
	string address="C:/Users/Niloufar/Desktop/SemanticMapping-P3/transfer_C/";
	string M_string;
	ostringstream convert;
	convert << M;
	M_string=convert.str();
	
	string address_feature=address+"/"+DB_name+"/"+Feature+"/"+"input"+"/"+"q"+seg_level+"_k"+ M_string;  
	string address_label=address+"/"+DB_name+"/"+"labels_train.txt"; 
	string address_region=address+"/"+DB_name+"/"+"q"+seg_level;  

	string temp_address;
	
	////F1
	temp_address=address_feature+"/"+"F_train.txt";
	read_file_2D<float>(temp_address, F1, N1, M);
	
	temp_address=address_feature+"/"+"F_train.bin";
	WriteToBinFile_2D<float>(F1, temp_address, N1, M);
	
	//temp_address=address_feature+"/"+"F_train.bin";
	//ReadFromBinFile_2D<float>(F1, temp_address, N1, M);

	////F2
	temp_address=address_feature+"/"+"F_test.txt";
	read_file_2D<float>(temp_address, F2, N2, M);
       
	temp_address=address_feature+"/"+"F_test.bin";
	WriteToBinFile_2D<float>(F2, temp_address, N2, M);
	
	//temp_address=address_feature+"/"+"F_test.bin";
	//ReadFromBinFile_2D<float>(F2, temp_address, N2, M);
	
	////label1
	read_file_2D<int>(address_label, label1, DB1, num_class);
	
	temp_address=address_feature+"/"+"labels_train.bin";
	WriteToBinFile_2D<int>(label1, temp_address, DB1, num_class);

	//temp_address=address_feature+"/"+"labels_train.bin";
	//ReadFromBinFile_2D<int>(label1, temp_address, DB1, num_class);

	////label2-----Not used

	////sup_loc1
	temp_address=address_region+"/"+"sup_loc_train.txt";
	read_file_2D<int>(temp_address, sup_loc1, N1, 2);
	
	temp_address=address_region+"/"+"sup_loc_train.bin";
	WriteToBinFile_2D<int>(sup_loc1, temp_address, N1, 2);

	//temp_address=address_region+"/"+"sup_loc_train.bin";
	//ReadFromBinFile_2D<int>(sup_loc1, temp_address, N1, 2);

	////sup_loc2
	temp_address=address_region+"/"+"sup_loc_test.txt";
	read_file_2D<int>(temp_address, sup_loc2, N2, 2);

	temp_address=address_region+"/"+"sup_loc_test.bin";
	WriteToBinFile_2D<int>(sup_loc2, temp_address, N2, 2);

	//temp_address=address_region+"/"+"sup_loc_test.bin";
	//ReadFromBinFile_2D<int>(sup_loc2, temp_address, N2, 2);

	////border1
	temp_address=address_region+"/"+"border_train.txt";
	read_file_2D<int>(temp_address, border1, N1, max_regions1);

	temp_address=address_region+"/"+"border_train.bin";
	WriteToBinFile_2D<int>(border1, temp_address, N1, max_regions1);

	//temp_address=address_region+"/"+"border_train.bin";
	//ReadFromBinFile_2D<int>(border1, temp_address, N1, max_regions1);
	
	////border2 
	temp_address=address_region+"/"+"border_test.txt";
	read_file_2D<int>(temp_address, border2, N2, max_regions2);

	temp_address=address_region+"/"+"border_test.bin";
	WriteToBinFile_2D<int>(border2, temp_address, N2, max_regions2);

	//temp_address=address_region+"/"+"border_test.bin";
	//ReadFromBinFile_2D<int>(border2, temp_address, N2, max_regions2);

	////loc1
	temp_address=address_region+"/"+"loc_train.txt";
	read_file_2D<int>(temp_address, loc1, DB1, max_regions1);

	temp_address=address_region+"/"+"loc_train.bin";
	WriteToBinFile_2D<int>(loc1, temp_address, DB1, max_regions1);

	//temp_address=address_region+"/"+"loc_train.bin";
	//ReadFromBinFile_2D<int>(loc1, temp_address, DB1, max_regions1);

	////loc2
	temp_address=address_region+"/"+"loc_test.txt";
	read_file_2D<int>(temp_address, loc2, DB2, max_regions2);

	temp_address=address_region+"/"+"loc_test.bin";
	WriteToBinFile_2D<int>(loc2, temp_address, DB2, max_regions2);

	//temp_address=address_region+"/"+"loc_test.bin";
	//ReadFromBinFile_2D<int>(loc2, temp_address, DB2, max_regions2);

	////num_regions1
	temp_address=address_region+"/"+"num_regions_train.txt";
	read_file_1D<int>(temp_address, num_regions1, DB1);

	temp_address=address_region+"/"+"num_regions_train.bin";
	WriteToBinFile_1D<int>(num_regions1, temp_address, DB1);

	//temp_address=address_region+"/"+"num_regions_train.bin";
	//ReadFromBinFile_1D<int>(num_regions1, temp_address, DB1);

	////num_regions2
	temp_address=address_region+"/"+"num_regions_test.txt";
	read_file_1D<int>(temp_address, num_regions2, DB2);

	temp_address=address_region+"/"+"num_regions_test.bin";
	WriteToBinFile_1D<int>(num_regions2, temp_address, DB2);

	//temp_address=address_region+"/"+"num_regions_test.bin";
	//ReadFromBinFile_1D<int>(num_regions2, temp_address, DB2);

	cout<<"M="<<M<<" and "<<"N1= "<<N1<< " and DB1= "<<DB1<<" and "<<"N2= "<<N2<< " and DB2= "<<DB2<<endl;

	time (&rawtime);
	cout<<"--Done with reading--The time is "<<ctime (&rawtime); 

	///////////////////////////////////////////////////////////////////////Searching/////////////////////////////////////////////////////////////////////
    //Note: Indeces start from zero.		
	float **val=DynamicArray2D<float>(N1, knn);
	int **ind=DynamicArray2D<int>(N1, knn);		
		
	knnSearch_wo_classSimilarity(F1, N1, F1, N1, M, knn, val, ind);
	/////kdtreeSearch_wo_classSimilarity(F1, N1, F1, N1, M, knn, val, ind);        //--knnSearch using kdtree 
	
    write_file_2D<int>("ind.txt", ind, N1, knn);
    write_file_2D<float>("val.txt", val, N1, knn);

	temp_address=address_region+"/"+"ind.bin";
	ReadFromBinFile_2D<int>(ind, temp_address, N1, knn);

	temp_address=address_region+"/"+"val.bin";
	ReadFromBinFile_2D<float>(val, temp_address, N1, knn);

	time (&rawtime);
	cout<<"--Done with searching--The time is "<<ctime (&rawtime); 

	float** val_sim=DynamicArray2D<float>(N1,knn);
	int** ind_sim=DynamicArray2D<int>(N1,knn);
	//add_class_similarity(val, ind, N1, knn, sup_loc1, label1, DB1, num_class, L, val_sim, ind_sim);

	knnSearch_w_classSimilarity(F1, N1, M, label1, DB1, num_class, sup_loc1, knn, L, val_sim, ind_sim);

	write_file_2D<int>("ind_sim.txt", ind_sim, N1, knn);
    write_file_2D<float>("val_sim.txt", val_sim, N1, knn);

	read_file_2D<int>("ind_sim.txt", ind_sim, N1, knn);
	read_file_2D<float>("val_sim.txt", val_sim, N1, knn);

	time (&rawtime);
	cout<<"--Done with class_sim--The time is "<<ctime (&rawtime); 
	
	//////////////////////////////////////////////////////Network Creation///////////////////////////////////////////////////////////////////////
	int M_nonZ_size=0;

	int *row=DynamicArray1D<int>(M_size);   
	int *col=DynamicArray1D<int>(M_size);   
	float *weight=DynamicArray1D<float>(M_size);   

	row=init_array1D<int>(row, M_size, -1);  
	
	col=init_array1D<int>(col, M_size, -1); 
	
	weight=init_array1D<float>(weight, M_size, 0);
	
	////For accounting for similarity
	M_nonZ_size=weighted_network(F1, N1, M, ind_sim, val_sim, knn, border1, loc1, num_regions1, N_conn, DB1, max_regions1, row, col, weight, M_size);

	cout<<endl<<"M_nonZ_size = "<<M_nonZ_size<<endl<<endl;
	
	FreeDynamicArray2D<int>(label1);
	FreeDynamicArray2D<float>(val);
	FreeDynamicArray2D<int>(ind);

	FreeDynamicArray2D<float>(val_sim);
	FreeDynamicArray2D<int>(ind_sim);

	write_file_1D<int>("row.txt", row, M_size);
	write_file_1D<int>("col.txt", col, M_size);
	write_file_1D<float>("weight.txt", weight, M_size);

	time (&rawtime);
	cout<<"--Done with creating Network--The time is "<<ctime (&rawtime); 
	
	///////////////////////////////////////Transfer Network to MATLAB-perform NCut and import results from Matlab////////////////////////////////////////////		
	int *net_part= DynamicArray1D<int>(N1);

	MatlabType matlab;
	
	//open matlab
	matlab.MatlabOpen();
	matlab.MalabCCC();

	//send row
	matlab.CToMatlab(row, M_size, "row");
	
	//send col
	matlab.CToMatlab(col, M_size, "col");
	
	//send weight
	matlab.CToMatlab(weight, M_size, "weight");
	
	//send num_comm
	double *temp_num_comm=DynamicArray1D<double>(1);
	temp_num_comm=init_array1D<double>(temp_num_comm,1,num_comm);
	matlab.CToMatlab(temp_num_comm, 1, "C");
	
	//send N
	double *temp_N=DynamicArray1D<double>(1);
	temp_num_comm=init_array1D<double>(temp_N,1,N1);
	matlab.CToMatlab(temp_N, 1, "N");
	
	//send M_nonZ_size
	double *temp_M_nonZ_size=DynamicArray1D<double>(1);
	temp_num_comm=init_array1D<double>(temp_M_nonZ_size,1,M_nonZ_size);
	matlab.CToMatlab(temp_M_nonZ_size, 1, "S");
	
	//send commands to matlab
	matlab.MatlabCommand("cd E:/school/Segmentation/Community_graph");	
	matlab.MatlabCommand("net_part=Ncut_C(row, col, weight, N, C, S)");  // N=num_nodes C=num_comm and S=M_nonZ_size

	//get variables from matlab
	matlab.MatlabToC<int>("net_part", net_part, N1);	
	
	//closing the matlab
	matlab.MatlabClose();


	///////IF WANT TO READ "net_part" FROM TEXT FILE///////
	//address_feature=address+"/"+DB_name+"/"+Feature+"/"+"input"+"/"+"q"+seg_level+"_k"+ M_string;  
	//temp_address=address_feature+"/"+ "Lonly_shared_labels/" + "net_part_100.txt";

	//read_file_1D<int>(temp_address, net_part, N1);

	//for (int i=0;i<5;i++)
	//	cout<<"i= "<<i<<" ---- net_part[i]= "<<net_part[i]<<endl;


	//time (&rawtime);
	//cout<<"--Done with NCUT--The time is "<<ctime (&rawtime);

	/////////////////////////////////////////////////////////////////Compute Comm_Relation/////////////////////////////////////////////////////////////	
	float **p_cc=DynamicArray2D<float>(num_comm,num_comm);
	p_cc=init_array2D<float>(p_cc,num_comm,num_comm,0); 

	comm_Relation(net_part, num_comm, N1, row, col, M_nonZ_size, p_cc);

	write_file_2D<float>("p_cc_100.txt", p_cc, num_comm, num_comm);

	FreeDynamicArray1D<int>(row);
	FreeDynamicArray1D<int>(col);
	FreeDynamicArray1D<float>(weight);


	///////IF WANT TO READ "pcc" FROM TEXT FILE///////
	//address_feature=address+"/"+DB_name+"/"+Feature+"/"+"input"+"/"+"q"+seg_level+"_k"+ M_string;  
	//temp_address=address_feature+"/"+ "L2/" + "pcc_Ncut100_comm.txt";

	//float **p_cc=DynamicArray2D<float>(num_comm,num_comm);
	//p_cc=init_array2D<float>(p_cc,num_comm,num_comm,0); 
	//read_file_2D<float>(temp_address, p_cc, num_comm, num_comm);


	////write_file_1D<int>("net_partt.txt", net_part, N1);
	////write_file_2D<float>("p_cc.txt", p_cc, num_comm, num_comm);

	/*time (&rawtime);
	cout<<"--Done with COMM_Relation --The time is "<<ctime (&rawtime); */

	/////////////////////////////////////////////////////////////////Compute Hist_train/////////////////////////////////////////////////////////////	
	////compute the neasrest neighbor indeces for train
	float **val1=DynamicArray2D<float>(N1,knn);
	int **ind1=DynamicArray2D<int>(N1,knn);

    knnSearch_wo_classSimilarity(F1, N1, F1, N1, M, knn, val, ind);

	//kdtreeSearch_wo_classSimilarity(F1, N1, F1, N1, M, knn, val1, ind1);
	
	FreeDynamicArray2D<float>(val1);
	
	//compute Hist_train
	float** hist1=DynamicArray2D<float>(DB1, num_comm);

	float** w1=DynamicArray2D<float>(N1, num_comm);	
	init_array2D<float>(w1, N1, num_comm, 0);

	float** w11=DynamicArray2D<float>(N1, num_comm);
	init_array2D<float>(w11, N1, num_comm, 0);

	float** w12=DynamicArray2D<float>(N1, num_comm);	
	init_array2D<float>(w12, N1,  num_comm, 0);

	Hist_node(N1, sup_loc1, border1, loc1, net_part, w11, w12, w1, ind1, p_cc, knn_consider, num_comm, max_regions1);
	
	//////writing hist_node to a file
	address_feature=address+"/"+DB_name+"/"+Feature+"/"+"output"+"/"+"q"+seg_level+"_k"+ M_string;  
	temp_address=address_feature+"/"+ "hist_train_node_level.txt";
	write_file_2D<float>(temp_address, w1, N1, num_comm);	
	cout<<"--Done with writing hist_train_node"<<endl;


	FreeDynamicArray2D<float>(w1);
	FreeDynamicArray2D<float>(w11);
	FreeDynamicArray2D<float>(w12);

	time (&rawtime);
	std::cout<<"--Done with Hist_train--The time is "<<ctime (&rawtime)<<endl;

	/////////////////////////////////////////////////////////////////Compute Hist_test/////////////////////////////////////////////////////////////
	int *net_part=DynamicArray1D<int>(N1);
	read_file_1D<int>("net_partt.txt",net_part,N1);


	float **p_cc=DynamicArray2D<float>(num_comm,num_comm);
	read_file_2D<float>("p_cc.txt",p_cc,num_comm,num_comm);

	//compute the neasrest neighbor indeces for test
	float **val2=DynamicArray2D<float>(N2,knn);
	int **ind2=DynamicArray2D<int>(N2,knn);

	kdtreeSearch_wo_classSimilarity(F1, N1, F2, N2, M, knn, val2, ind2);

	cout<<"--Done with Search_test"<<endl;

	FreeDynamicArray2D<float>(val2);

	//compute Hist_test
	float** hist2=DynamicArray2D<float>(DB2,num_comm);

	float** w2=DynamicArray2D<float>(N2,num_comm);	
	init_array2D<float>(w2, N2,  num_comm, 0);

	float** w21=DynamicArray2D<float>(N2,num_comm);
	init_array2D<float>(w21, N2,  num_comm, 0);

	float** w22=DynamicArray2D<float>(N2,num_comm);	
	init_array2D<float>(w22, N2,  num_comm, 0);

	Hist_node(N2, sup_loc2, border2, loc2, net_part, w21, w22, w2, ind2, p_cc, knn_consider, num_comm, max_regions2);
	
	////writing hist_node to a file
	address_feature=address+"/"+DB_name+"/"+Feature+"/"+"output"+"/"+"q"+seg_level+"_k"+ M_string;  
	temp_address=address_feature+"/"+ "hist_test_node_level.txt";
	write_file_2D<float>(temp_address, w2, N2, num_comm);	
	cout<<"--Done with writing hist_test_node"<<endl;
	
	Hist_image(DB2, num_regions2, loc2, w2, hist2, num_comm, pooling);


	FreeDynamicArray2D<float>(w2);
	FreeDynamicArray2D<float>(w21);
	FreeDynamicArray2D<float>(w22);

	time (&rawtime);
	cout<<"--Done with Hist_test--The time is "<<ctime (&rawtime); 

	/////////////Writing Hist_train and Hist_test to txt files/////////
	
	address_feature=address+"/"+DB_name+"/"+Feature+"/"+"output"+"/"+"q"+seg_level+"_k"+ M_string;  

	temp_address=address_feature+"/"+ "hist_train_sim_1.txt";
	write_file_2D<float>(temp_address, hist1, DB1, num_comm);
	
	cout<<"--Done with writing hist_train"<<endl;

	temp_address=address_feature+"/"+ "hist_test_sim_1.txt";
	write_file_2D<float>(temp_address, hist2, DB2, num_comm);
	
	cout<<"--Done with writing hist_test"<<endl;


	time (&rawtime);
	cout<<"--Done with writing the data--The time is "<<ctime (&rawtime);

	/////////////Free Memory/////////	
	FreeDynamicArray2D<float>(F1);
	FreeDynamicArray2D<float>(F2);
		
	FreeDynamicArray2D<int>(sup_loc1);	
	FreeDynamicArray2D<int>(border1);
	FreeDynamicArray2D<int>(loc1);
	FreeDynamicArray1D<int>(num_regions1);
		
	FreeDynamicArray1D<int>(net_part);
	FreeDynamicArray2D<float>(p_cc);

	FreeDynamicArray2D<int>(ind1);
	FreeDynamicArray2D<float>(hist1);

	FreeDynamicArray2D<int>(border2);	
	FreeDynamicArray2D<int>(sup_loc2);	
	FreeDynamicArray2D<int>(loc2);	
	FreeDynamicArray1D<int>(num_regions2);	

	FreeDynamicArray2D<int>(ind2);
	FreeDynamicArray2D<float>(hist2);
	
	time (&rawtime);
	cout<<"--Done with program--The time is "<<ctime (&rawtime)<<endl; 

	_getch();
    return 0;
}

