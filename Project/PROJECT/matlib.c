#include <stdlib.h>
#include <assert.h>



void transpose(int n, int m, double *B, const double *A, int T) {
  for (int i=0; i<n; i++) {
      for (int j=0; j<m; j++) {
        B[j*n+i] = A[i*m+j];
      }
  }
}



void transpose_parallel(int n, int m, double *B, const double *A, int T) {
  #pragma omp parallel for
  for (int i=0; i<n; i++) {
      for (int j=0; j<m; j++) {
        B[j*n+i] = A[i*m+j];
      }
  }
}

void transpose_blocked(int n, int m, double *B, const double *A, int T) {

  for (int ii=0; ii<n; ii+=T) { 
    for (int jj=0; jj<m; jj+=T) {
      for (int i=ii; i<(ii+T); i++) {
        for (int j=jj; j<(jj+T); j++) {
          B[j*n+i] = A[i*m+j]; 
        }
      }
    }
  }
}

void transpose_blocked_parallel(int n, int m, double *B, const double *A, int T) {
  #pragma omp parallel for schedule(dynamic)
  for (int ii=0; ii<n; ii+=T) { 
    for (int jj=0; jj<m; jj+=T) {
      for (int i=ii; i<(ii+T); i++) {
        for (int j=jj; j<(jj+T); j++) {
          B[j*n+i] = A[i*m+j]; 
        }
      }
    }
  }
}







void matmul(int n, int m, int k, double* C, const double* A, const double* B) {
  double acc ; 
  for (int i=0; i<n; i++) {
      for (int j=0; j<k; j++) {
          acc = 0;
          for (int p=0; p<m; p++) {
              acc += A[i*m+p] * B[p*k+j] ;
          }
          C[i*k+j] = acc;
          }
      }
  }

void matmul_locality(int n, int m, int k, double* C, const double* A, const double* B) {
  
  double a; 
  for (int i=0; i<n*k; i++) {
    C[i] = 0 ; 
  }
  for (int i = 0; i < n; i++) {
    for (int p = 0; p < m; p++) {

      a = A[i*m+p];

      for (int j=0; j < k; j++) {
        C[i*k+j] += a * B[p*k+j] ;
      }
    }
  }
}



void matmul_transpose(int n, int m, int k, double* C, const double* A, const double* B) {
  double *BT = malloc((m*k) * sizeof(double));
  int T = 25; 

  for (int ii=0; ii<m; ii+=T) { 
    for (int jj=0; jj<k; jj+=T) {
      for (int i=ii; i<(ii+T); i++) {
        for (int j=jj; j<(jj+T); j++) {
          BT[j*m+i] = B[i*k+j]; 
        }
      }
    }
  }

  for (int i=0; i<n; i++) {
      for (int j=0; j<k; j++) {
          double acc = 0;
          for (int p=0; p<m; p++) {
              acc += A[i*m+p] * BT[j*m+p] ;
          }
          C[i*k+j] = acc;
          }
      }
  free(BT);
}

void matmul_parallel(int n, int m, int k, double* C, const double* A, const double* B) {
  double acc; 
  #pragma omp parallel for reduction(+:acc)  
  for (int i=0; i<n; i++) {
      for (int j=0; j<k; j++) {
          acc = 0;
          for (int p=0; p<m; p++) {
              acc += A[i*m+p] * B[p*k+j] ;
          }
          C[i*k+j] = acc;
          }
      }
  }


void matmul_locality_parallel(int n, int m, int k, double* C, const double* A, const double* B) {
  double a; 

  #pragma omp parallel for 
  for (int i=0; i < n*k; i++) {
    C[i] = 0 ; 
  }

  #pragma omp parallel for reduction(+:a)
  for (int i = 0; i < n; i++) {
    for (int p = 0; p < m; p++) {
      a = A[i*m+p];
      for (int j=0; j < k; j++) {
        C[i*k+j] += a * B[p*k+j] ;
      }
    }
  }
}

void matmul_transpose_parallel(int n, int m, int k, double* C, const double* A, const double* B) {
  double *BT = malloc((m*k) * sizeof(double));
  int T = 25; 

  #pragma omp parallel for schedule(dynamic)
  for (int ii=0; ii<m; ii+=T) { 
    for (int jj=0; jj<k; jj+=T) {
      for (int i=ii; i<(ii+T); i++) {
        for (int j=jj; j<(jj+T); j++) {
          BT[j*m+i] = B[i*k+j]; 
        }
      }
    }
  }

  double acc; 
  #pragma omp parallel for reduction(+:acc)
  for (int i=0; i<n; i++) {
      for (int j=0; j<k; j++) {
          acc = 0;
          for (int p=0; p<m; p++) {
              acc += A[i*m+p] * BT[j*m+p] ;
          }
          C[i*k+j] = acc;
          }
      }

  free(BT);
}

