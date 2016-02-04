#ifndef GENERAL_HEADER_H
#define GENERAL_HEADER_H

#include<string>

using namespace std;

//write to a bin file 2D
template <typename T>
void WriteToBinFile_2D(T **F, string filename, int dim1, int dim2);

//read from a bin file 2D
template <typename T>
void ReadFromBinFile_2D(T **F, string filename, int dim1,int dim2);

//write to a bin file 1D
template <typename T> 
void WriteToBinFile_1D(T *F, string filename, int dim);

//read from a bin file 1D
template <typename T> 
void ReadFromBinFile_1D(T *F, string filename, int dim);

//create-destroy dynamic 2D array
template <typename T> 
T **DynamicArray2D(int dim1, int dim2);

template <typename T>
void FreeDynamicArray2D(T** dArray);

//create-destroy dynamic 1D array
template <typename T> 
T *DynamicArray1D(int dim);

template <typename T>
void FreeDynamicArray1D(T* dArray);

//read 1D-2D array from a file
template <typename T> 
void read_file_2D(string FileName, T **F2, int dim1, int dim2);

template <typename T> 
void read_file_1D(string FileName, T *F1, int dim);

//write 1D-2D array to a file
template <typename T> 
void write_file_1D(string FileName, T *val, int dim);

template <typename T> 
void write_file_2D(string FileName, T **val, int dim1, int dim2);

//initialize a 1D-2D array
template <typename T> 
T *init_array1D(T *dArray, int dim, T value);

template <typename T> 
T **init_array2D(T **dArray, int dim1, int dim2, T value);

//L1 normalize a 1D array
template<typename T>
void L1_norm(T *F, int dim);

//distance metrics
float hellinger(float *feature, float *comp_feature, int dim);


#endif
