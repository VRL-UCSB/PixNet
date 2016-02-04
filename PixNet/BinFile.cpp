#include<string>
#include<fstream>

using namespace std;

template <typename T> 
void WriteToBinFile_2D(T **F, string filename, int dim1, int dim2)
{
	
	
	//std::string filename("input/q250_k200/F_train.bin");
    std::fstream f(filename, std::ios::binary | std::ios::out);
    for (int i=0; i<dim1; i++) {
		char *p = (char *) &F[i][0];
		unsigned int sz = dim2 * sizeof(T);
		f.write(p, sz);
	}
    f.close();
}


template void WriteToBinFile_2D<int>(int **F, string filename, int dim1, int dim2);
template void WriteToBinFile_2D<float>(float **F, string filename, int dim1, int dim2);
template void WriteToBinFile_2D<double>(double **F, string filename, int dim1, int dim2);

///////Initialization--for testing only/////////
	/*unsigned int sz = M * sizeof(float);
	for (int i=0; i<N_train; i++) {
		char *p = (char *) &F_train[i][0];
		memset(p, 0, sz);
	}*/

template <typename T> 
void ReadFromBinFile_2D(T **F, string filename, int dim1,int dim2)
{
	//std::string filename("input/q250_k200/F_train.bin");
	//filename = "input/q250_k200/F_train.bin";
    fstream fi(filename, std::ios::binary | std::ios::in);
    for (int i=0; i<dim1; i++) {
		char *p = (char *) &F[i][0];
		unsigned int sz = dim2* sizeof(T);
		fi.read(p, sz);
	}
    fi.close();

}

template void ReadFromBinFile_2D<int>(int **F, string filename, int dim1,int dim2);
template void ReadFromBinFile_2D<float>(float **F, string filename, int dim1,int dim2);
template void ReadFromBinFile_2D<double>(double **F, string filename, int dim1,int dim2);


/////////////////////////1D////////////////////
template <typename T> 
void WriteToBinFile_1D(T *F, string filename, int dim)
{
		
	//std::string filename("input/q250_k200/F_train.bin");
    std::fstream f(filename, std::ios::binary | std::ios::out);   
	char *p = (char *) &F[0];
	unsigned int sz = dim * sizeof(T);
	f.write(p, sz);
    f.close();
}


template void WriteToBinFile_1D<int>(int *F, string filename, int dim);
template void WriteToBinFile_1D<float>(float *F, string filename, int dim);
template void WriteToBinFile_1D<double>(double *F, string filename, int dim);


template <typename T> 
void ReadFromBinFile_1D(T *F, string filename, int dim)
{

    fstream fi(filename, std::ios::binary | std::ios::in);
	char *p = (char *) &F[0];
	unsigned int sz = dim* sizeof(T);
	fi.read(p, sz);	
    fi.close();
}

template void ReadFromBinFile_1D<int>(int *F, string filename, int dim);
template void ReadFromBinFile_1D<float>(float *F, string filename, int dim);
template void ReadFromBinFile_1D<double>(double *F, string filename, int dim);







/////////////Reading in values/////////
	
	///////writing to a bin file/////////
	/*std::string filename("input/q250_k200/F_train.bin");
    std::fstream f(filename, std::ios::binary | std::ios::out);
    for (int i=0; i<N_train; i++) {
		char *p = (char *) &F_train[i][0];
		unsigned int sz = M * sizeof(float);
		f.write(p, sz);
	}
    f.close();*/

	///////Initialization--for testing only/////////
	/*unsigned int sz = M * sizeof(float);
	for (int i=0; i<N_train; i++) {
		char *p = (char *) &F_train[i][0];
		memset(p, 0, sz);
	}*/

	///////reading from a bin file/////////
	/*std::string filename("input/q250_k200/F_train.bin");
	filename = "input/q250_k200/F_train.bin";
    fstream fi(filename, std::ios::binary | std::ios::in);
    for (int i=0; i<N_train; i++) {
		char *p = (char *) &F_train[i][0];
		unsigned int sz = M * sizeof(float);
		fi.read(p, sz);
	}
    fi.close();*/