#ifndef PIXNET_HEADER_H
#define PIXNET_HEADER_H

//knnSearch
void knnSearch_wo_classSimilarity(float **F1, int N1, float **F2, int N2, int M, int knn, float **val, int **ind);
void knnSearch_w_classSimilarity(float **F1, int N1, int M, int **label1, int DB1, int num_class, int **sup_loc1, int knn, float L, float **val_sim, int **ind_sim);
void kdtreeSearch_wo_classSimilarity(float **F1, int N1, float **F2, int N2, int M, int knn, float **val, int **ind);
void add_class_similarity(float **val, int **ind, int N1, int knn, int **sup_loc, int **label, int DB1, int num_class, int L, float **val_sim, int **ind_sim);

//network and comm_relation
int weighted_network(float **F1, int N1, int M, int **ind, float **val, int knn, int **border, int **loc, int *num_regions1, int N_conn, int DB1, int max_regions1, int *row, int *col, float *weight, int M_size);
void comm_Relation(int *net_part, int num_comm, int N1, int *row, int * col, int M_nonZ_size, float **p_cc);

//hist_image
void Hist_node(int N, int **sup_loc, int **border, int **loc, int *net_part, float **w1, float **w2, float **w, int **ind, float **p_cc, int knn_consider, int num_comm, int max_regions);
float p_appear(int knn_consider, int *net_part, int **ind, int row_nei, int cc);
void Hist_image(int DB2, int *num_regions, int **loc, float **w, float **hist, int num_comm, int pooling);

#endif