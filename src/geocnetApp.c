//Developed by Jessica Domingues (jessica.dominguess@gmail.com)

#include <igraph.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void printMatrix(double ***matrix, int n_matrix, int size_matrix);
extern void print_matrix(const igraph_matrix_t *m);
extern void write_matrix(double ***matrix, char *prefix, int n_matrix,int size_matrix, double threshold );
extern void write(const igraph_matrix_t *m, char *prefix, int n_matrix,int size_matrix);
extern void write_data(char *prefix, igraph_integer_t * diameter, 
                                     igraph_real_t * avgCoef, 
                                     igraph_vector_t * vertCoef, 
                                     igraph_vector_t * vertDegreeAll,
                                     int * maxDegreeAll, 
                                     double * avgDegreeAll, 
                                     igraph_vector_t * vertDegreeIn, 
                                     int * maxDegreeIn, 
                                     igraph_vector_t * vertDegreeOut, 
                                     int * maxDegreeOut, 
                                     igraph_vector_t * betweenness);

extern void fluxMatrix(double **matrix, int size_matrix, double threshold, igraph_matrix_t *i_mat);
extern void print_vector_d(igraph_vector_t *v);
extern void calc_indexes(igraph_matrix_t *i_mat, igraph_integer_t * diameter,
                                         igraph_real_t * avgCoef,
                                         igraph_vector_t * vertCoef,
                                         igraph_vector_t * vertDegreeAll,
                                         int * maxDegreeAll,
                                         double * avgDegreeAll,
                                         igraph_vector_t * vertDegreeIn,
                                         int * maxDegreeIn,
                                         igraph_vector_t * vertDegreeOut,
                                         int * maxDegreeOut,
                                         igraph_vector_t * betweenness);
extern void open_matrix(char *filename, int *n_matrix, double **** matrix, int *size_matrix);
extern void destroy(igraph_matrix_t *i_mat, igraph_vector_t * vertCoef, 
                                     igraph_vector_t * vertDegreeAll,
                                     igraph_vector_t * vertDegreeIn, 
                                     igraph_vector_t * vertDegreeOut, 
                                     igraph_vector_t * betweenness);
 

void freeMatrix(double ***matrix, int n_matrix, int size_matrix) {
  int i, k;
  for(k=0; k<n_matrix; k++)  {
    for(i=0; i<size_matrix; i++) {
      free(matrix[k][i]);
    }
    free(matrix[k]);
  }
  free(matrix);
  
}

extern void printMatrix(double ***matrix, int n_matrix, int size_matrix);

int main() {
  
  char filename[100];
  int i, j, n_matrix, size_matrix;
  double threshold;
  double ***matrix;
  
  igraph_matrix_t i_mat;
  igraph_integer_t  diameter;
  
  /*Clustering Coefficient*/
  igraph_real_t  avgCoef;
  igraph_vector_t vertCoef;
  
  /*Degree*/
    /*All*/
  igraph_vector_t vertDegreeAll;
  int maxDegreeAll;
  double avgDegreeAll;
  /*In and Out*/
  igraph_vector_t vertDegreeIn;
  int maxDegreeIn;
  igraph_vector_t vertDegreeOut;
  int maxDegreeOut;    
      
  /*Betweenness */
  igraph_vector_t betweenness;
  
  strcpy(filename, "matrix.in");
  threshold = 0.0;
  
  
  open_matrix(filename, &n_matrix, &matrix, &size_matrix);
//   printMatrix(matrix,n_matrix,size_matrix);

    for(i=0; i<n_matrix; i++) {

     fluxMatrix(matrix[i], size_matrix, threshold, &i_mat);
     
     calc_indexes(&i_mat, &diameter, 
                          &avgCoef, 
                          &vertCoef, 
                          &vertDegreeAll, 
                          &maxDegreeAll, 
                          &avgDegreeAll, 
                          &vertDegreeIn, 
                          &maxDegreeIn, 
                          &vertDegreeOut, 
                          &maxDegreeOut,
                          &betweenness);   
   
     write_data("properties.txt", &diameter, 
                          &avgCoef, 
                          &vertCoef, 
                          &vertDegreeAll, 
                          &maxDegreeAll, 
                          &avgDegreeAll, 
                          &vertDegreeIn, 
                          &maxDegreeIn, 
                          &vertDegreeOut, 
                          &maxDegreeOut,
                          &betweenness);
      
     destroy(&i_mat, &vertCoef, 
                          &vertDegreeAll, 
                          &vertDegreeIn, 
                          &vertDegreeOut, 
                          &betweenness);
  }//for i  
   write_matrix(matrix,"weighted_adjacency.txt", n_matrix, size_matrix,threshold);
   freeMatrix(matrix, n_matrix, size_matrix);
  
  return 0;
}
