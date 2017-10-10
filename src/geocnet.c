#include <igraph.h>
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

void printMatrix(double ***matrix, int n_matrix, int size_matrix) {
  int i, j, k;
  
  for(k=0; k<n_matrix; k++) {
    for(i=0; i<size_matrix; i++) {
      for(j=0; j<size_matrix; j++) {
        printf("%0.2lf ", matrix[k][i][j]);
      }
      printf("\n");
    }
    printf("\n");
  }
}

void print_matrix(const igraph_matrix_t *m) {
  
  long int i, j, nrow=igraph_matrix_nrow(m), ncol=igraph_matrix_ncol(m);
  
  for(i=0; i<nrow; i++) {
    for(j=0; j<ncol-1; j++) {
      printf("%lf ", MATRIX(*m, i, j));
    }
    printf("%lf\n", MATRIX(*m, i, ncol-1));
  }
  printf("\n");
}

void write_matrix(double ***matrix, char *prefix, int n_matrix,int size_matrix, double threshold) {
  int i, j, k;
  char buf[12];
  FILE *file = fopen(prefix, "w");
  if(file != NULL) {
    for(k=0; k<n_matrix; k++) {
      for(i=0; i<size_matrix; i++) {
        for(j=0; j<size_matrix; j++) {
          if(matrix[k][i][j] < threshold || i == j){
            fprintf(file,"0.0 ");
          }
           else fprintf(file,"%0.2lf ", matrix[k][i][j]);
        }
        fprintf(file,"\n");
      }
    }
  } // end if
  else {
    printf("Function write_matrix:\n");
    printf("ERROR! File %s didn't open!\n", prefix);
    printf("Program was closed!\n");
    exit(0);
  } // end else
}  // end write_matrix

void write_data(char *prefix, 
                              igraph_integer_t * diameter, 
                              igraph_real_t * avgCoef, 
                              igraph_vector_t * vertCoef, 
                              igraph_vector_t * vertDegreeAll,
                              int * maxDegreeAll, 
                              double * avgDegreeAll, 
                              igraph_vector_t * vertDegreeIn, 
                              int * maxDegreeIn, 
                              igraph_vector_t * vertDegreeOut, 
                              int * maxDegreeOut, 
                              igraph_vector_t * betweenness) {
                                           
  long int i, j;
  char buf[12];
  FILE *file = fopen(prefix, "a");
  if(file != NULL) {
    fprintf(file, "Diameter:\n");
    fprintf(file, "%d\n", (int)*diameter);
    fprintf(file, "avg Clustering:\n");
    fprintf(file, "%lf\n", *avgCoef);
    
    fprintf(file, "Clustering vertex\n");
    for(i=0; i<igraph_vector_size(vertCoef); i++) {
        fprintf(file, "%lf ", VECTOR(*vertCoef)[i]);
      } // end for i
    fprintf(file, "\n");
    fprintf(file, "avg Degree:\n");
    fprintf(file, "%f\n", *avgDegreeAll);
    fprintf(file, "Degree All\n");
    for(i=0; i<igraph_vector_size(vertDegreeAll); i++) {
        fprintf(file, "%lf ", VECTOR(*vertDegreeAll)[i]);
      } // end for i
    fprintf(file, "\n");
    fprintf(file, "Degree in\n");
    for(i=0; i<igraph_vector_size(vertDegreeIn); i++) {
        fprintf(file, "%lf ", VECTOR(*vertDegreeIn)[i]);
      } // end for i
    fprintf(file, "\n");
    fprintf(file, "Degree out\n");
    for(i=0; i<igraph_vector_size(vertDegreeOut); i++) {
        fprintf(file, "%lf ", VECTOR(*vertDegreeOut)[i]);
      } // end for i
    fprintf(file, "\n");
    fprintf(file, "Betweenness\n");
    for(i=0; i<igraph_vector_size(betweenness); i++) {
        fprintf(file, "%lf ", VECTOR(*betweenness)[i]);
      } // end for i
    fprintf(file, "\n");   
  } // end if
  else {
    printf("Function write_vector:\n");
    printf("ERROR! File %s didn't open!\n", prefix);
    printf("Program was closed!\n");
    exit(0);
  } // end else
}  // end write_matrix


void open_matrix(char *filename, int *n_matrix, double ****matrix, int *size_matrix) {
  int i, j, k;
  FILE* input_file;
  
  input_file = fopen(filename, "r");
  if(input_file != NULL) {
    fscanf(input_file, "%d", n_matrix);
    fscanf(input_file, "%d", size_matrix);
     
    // Matrix[] => Number of matrix
    *matrix=malloc(*n_matrix*sizeof(double**)); 
    for(k=0; k<*n_matrix; k++) {
      
      // For Matrix[k] load 2D matrix of data
      (*matrix)[k]=malloc(*size_matrix*sizeof(double*)); 
      for(i=0; i<*size_matrix; i++)
        (*matrix)[k][i]=malloc(*size_matrix*sizeof(double));
      
      
      for(i=0; i<*size_matrix; i++) 
        for(j=0; j<*size_matrix; j++)
          if(!fscanf(input_file, "%lf", &(*matrix)[k][i][j])) break;
           
    }

//     printf("matriz: %lf ",*matrix[0][6][0]);
  }
  else {
    printf("Function open_fluxMatrix\n");
    printf("ERROR! Open file %s\n", filename);
    printf("Program was closed!\n");
    exit(0);
  }
}

void fluxMatrix(double **matrix, int size_matrix, double threshold, igraph_matrix_t *i_mat) {
  int i, j;  
  igraph_matrix_init(i_mat, size_matrix, size_matrix);
  
  for(i=0; i<size_matrix; i++)  {
    for(j=0; j<size_matrix; j++)  {
      if(matrix[i][j])  {
        if(matrix[i][j] < threshold || i == j) MATRIX(*i_mat, i, j) = 0.0;
        else                                   MATRIX(*i_mat, i, j) = matrix[i][j];
      }
    }
  }    
  igraph_i_set_attribute_table(&igraph_cattribute_table);

} // end open_fluxMatrix


int calc_indexes(igraph_matrix_t *i_mat, igraph_integer_t * diameter, 
                                         igraph_real_t * avgCoef, 
                                         igraph_vector_t * vertCoef, 
                                         igraph_vector_t * vertDegreeAll,
                                         int * maxDegreeAll, 
                                         double * avgDegreeAll, 
                                         igraph_vector_t * vertDegreeIn, 
                                         int * maxDegreeIn, 
                                         igraph_vector_t * vertDegreeOut, 
                                         int * maxDegreeOut, 
                                         igraph_vector_t * betweenness) {
  
   igraph_t g;
   long int size_matrix = igraph_matrix_nrow(i_mat);
   
   igraph_weighted_adjacency(&g, i_mat, IGRAPH_ADJ_DIRECTED, 0, /*loops=*/ 1);
   igraph_vector_init(vertCoef, size_matrix);
   igraph_vector_init(vertDegreeAll, size_matrix);
   igraph_vector_init(vertDegreeIn, size_matrix);
   igraph_vector_init(vertDegreeOut, size_matrix);
   igraph_vector_init(betweenness, size_matrix);

   /*Diameter*/
   igraph_diameter(&g, diameter, 0, 0, 0, IGRAPH_DIRECTED, 1);
   
   /*Clustering Coefficient*/
   igraph_transitivity_avglocal_undirected(&g, avgCoef, IGRAPH_TRANSITIVITY_ZERO);
   igraph_transitivity_local_undirected(&g, vertCoef, igraph_vss_all(),IGRAPH_TRANSITIVITY_ZERO);

/*Degree*/
     /*All Degree*/
   igraph_degree(&g, vertDegreeAll, igraph_vss_all(), IGRAPH_ALL, IGRAPH_LOOPS);
   *maxDegreeAll = (int)igraph_vector_max(vertDegreeAll); 
   *avgDegreeAll = ((double)(igraph_vector_sum(vertDegreeAll)/(double)igraph_vector_size(vertDegreeAll)));
//    printf("%f ",*avgDegreeAll);
     /*In Degree*/
   igraph_degree(&g, vertDegreeIn, igraph_vss_all(), IGRAPH_IN, IGRAPH_LOOPS);
   *maxDegreeIn = (int)igraph_vector_max(vertDegreeIn); 
     /*Out Degree*/
   igraph_degree(&g, vertDegreeOut, igraph_vss_all(), IGRAPH_OUT, IGRAPH_LOOPS);
   *maxDegreeOut = (int)igraph_vector_max(vertDegreeOut); 
     
   /*Betweenness*/
   igraph_betweenness(&g, betweenness, igraph_vss_all(), IGRAPH_UNDIRECTED, /*weights=*/ 0, /*nobigint=*/ 1);
   
 }
 
 void destroy(igraph_matrix_t *i_mat, igraph_vector_t * vertCoef, 
                                     igraph_vector_t * vertDegreeAll,
                                     igraph_vector_t * vertDegreeIn, 
                                     igraph_vector_t * vertDegreeOut, 
                                     igraph_vector_t * betweenness){
  igraph_matrix_destroy(i_mat);                                         
  igraph_vector_destroy(vertCoef);
  igraph_vector_destroy(vertDegreeAll);
  igraph_vector_destroy(vertDegreeIn);
  igraph_vector_destroy(vertDegreeOut);
  igraph_vector_destroy(betweenness);
   
 
   
   
 }

 void print_vector_d(igraph_vector_t *v) {
  long int i;
  for (i=0; i<igraph_vector_size(v); i++) {
    printf("%lf ", VECTOR(*v)[i]);
  }
  printf("\n");
}
 