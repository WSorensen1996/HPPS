// This file contains skeleton code for benchmarking your solutions.
// You are not *required* to use it, but it may be very helpful.  Read
// the comments to understand how to use it.  The two important helper
// functions are bench_transpose() and bench_matmul().  See the main()
// function for (commented-out) example uses.
//
// The matmul benchmarking function assume that the "basic" version
// matmul() is correct.  Validation is by comparing the results of the
// given function with the results of matmul().  This means you should
// be absolutely sure that the matmul() is correct, or the validation
// will not work.

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "matlib.h"
#include "timing.h"

// Pointer to a transposition function.  All transpose variants
// defined in matlib.h have this type.
typedef void (*transpose_fn)(int n, int m, double* B, const double* A, int T);

// Pointer to a matrix multiplication function.  All matmul variants
// defined in matlib.h have this type.
typedef void (*matmul_fn)(int n, int m, int k, double* C, const double* A, const double* B);

// Create a random array of 'n' doubles.
double* random_array(int n) {
  double *p = malloc(n * sizeof(double));
  for (int i = 0; i < n; i++) {
    p[i] = ((double)rand())/RAND_MAX;
  }
  return p;
}

void bench_transpose(const char *desc, int runs, transpose_fn f,
                     int n, int m, int T) {
  uint64_t bef = microseconds();
  printf("%30s n=%4d, m=%4d: ", desc, n, m);
  fflush(stdout);

  double *A = random_array(n*m);
  double *B = calloc(m*n, sizeof(double));

  for (int i = 0; i < runs; i++) {
    f(n, m, B, A, T);
  }

  double us = (microseconds()-bef)/runs;

  // Validate the result.
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (A[i*m+j] != B[j*n+i]) {
        printf("\nFailed validation: mismatch at index [%d,%d]\n",
               i, j);
        return;
      }
    }
  }

  double s = us/1e6;
  double mib = ((double)n*m) * sizeof(double) / (1024*1024);
  printf("%4.fms (%5f MiB/s)\n", us/1e3, mib/s);

  free(A);
  free(B);
}

void bench_matmul(const char *desc, int runs, matmul_fn f,
                  int n, int m, int k) {
  uint64_t bef = microseconds();
  printf("%30s n=%4d, m=%4d, k=%4d: ", desc, n, m, k);
  fflush(stdout);

  double *A = random_array(n*m);
  double *B = random_array(m*k);
  double *C = calloc(n*k, sizeof(double));

  // Compute the "expected" output of matrix multiplication.
  double *golden = calloc(n*k, sizeof(double));
  matmul(n, m, k, golden, A, B);

  for (int i = 0; i < runs; i++) {
    f(n, m, k, C, A, B);
  }

  double us = (microseconds()-bef)/runs;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < k; j++) {
      double expected = golden[i*k+j];
      double got = C[i*k+j];
      if (expected != got) {
        printf("\nFailed validation: at index [%d,%d] expected %f, got %f\n",
               i, j, expected, got);
        return;
      }
    }
  }

  double s = us/1e6;
  double gflops = ((double)n*k*(2*m)) / 1e9;
  double mib = ((double)n*k*m) * sizeof(double) / (1024*1024);
  printf("%4.fms (%4f GFLOP/s, %5f MiB/s)\n", us/1e3, gflops/s, mib/s);

  free(A);
  free(B);
  free(C);
  free(golden);
}

int main(int argc, char** argv) {
  int threads = 1 ; 
  if (argc < 1) {
    printf("Usage: \n");
    printf("%s <int input>\n", argv[0]);
    return 1;
  } else if (argc > 1) {
    threads = atoi(argv[1]);
  }

  int n ;
  int m ;
  int k ; 
  int T; 
  int runs; 

  int MODI_ACTIVE = 0; //Change to 1 for modi tests //0 for local machine tests

  if (MODI_ACTIVE==0){
  /////////////TRANSPOSE TESTING PART///////////////////////////
  //Tests of all transpose functions for different values
  n = 900; 
  m = 1800; 
  T = 30; 
  runs = 50; 
  printf("\nRESULTS FOR: (T=%d) m=%d>n=%d and runs == %d\n", T, m,n,runs); 
  bench_transpose("transpose", runs, transpose, n, m, T);
  bench_transpose("transpose_blocked", runs, transpose_blocked, n, m, T);
  bench_transpose("transpose_parallel", runs, transpose_parallel, n, m, T);
  bench_transpose("transpose_blocked_parallel", runs, transpose_blocked_parallel, n, m, T);

  n = 3300; 
  m = 900; 
  T = 30; 
  runs = 100; 
  printf("\nRESULTS FOR: (T=%d) m=%d<n=%d and runs == %d\n", T, m,n,runs); 
  bench_transpose("transpose", runs, transpose, n, m, T);
  bench_transpose("transpose_blocked", runs, transpose_blocked, n, m, T);
  bench_transpose("transpose_parallel", runs, transpose_parallel, n, m, T);
  bench_transpose("transpose_blocked_parallel", runs, transpose_blocked_parallel, n, m, T);

  n = 12000; 
  m = 12000; 
  T = 30; 
  runs = 10; 
  printf("\nRESULTS FOR: (T=%d) m=%d==n=%d and runs == %d\n", T, m,n,runs); 
  bench_transpose("transpose", runs, transpose, n, m, T);
  bench_transpose("transpose_blocked", runs, transpose_blocked, n, m, T);
  bench_transpose("transpose_parallel", runs, transpose_parallel, n, m, T);
  bench_transpose("transpose_blocked_parallel", runs, transpose_blocked_parallel, n, m, T);


/////////////MATMUL TESTING PART///////////////////////////
//Tests of all matmul functions for different values
  n = 200; 
  m = 300; 
  k = 100; 
  runs = 100; 
  printf("\nMAT_MUL RESULTS: \n"); 
  printf("\nRESULTS FOR: m=%d, n=%d, k=%d and runs == %d\n", m,n,k,runs); 
  bench_matmul("matmul", runs, matmul,  n, m, k);
  bench_matmul("matmul_parallel", runs, matmul_parallel, n, m, k);
  bench_matmul("matmul_locality", runs, matmul_locality, n, m, k);
  bench_matmul("matmul_locality_parallel", runs, matmul_locality_parallel, n, m, k);
  printf("T-value = 25: "), 
  bench_matmul("matmul_transpose", runs, matmul_transpose, n, m, k);
  printf("T-value = 25: "), 
  bench_matmul("matmul_transpose_parallel", runs, matmul_transpose_parallel, n, m, k);

  n = 400; 
  m = 200; 
  k = 650; 
  runs = 50; 
  printf("\nRESULTS FOR: m=%d, n=%d, k=%d and runs == %d\n", m,n,k,runs); 
  bench_matmul("matmul", runs, matmul,  n, m, k);
  bench_matmul("matmul_parallel", runs, matmul_parallel, n, m, k);
  bench_matmul("matmul_locality", runs, matmul_locality, n, m, k);
  bench_matmul("matmul_locality_parallel", runs, matmul_locality_parallel, n, m, k);
  printf("T-value = 25: "), 
  bench_matmul("matmul_transpose", runs, matmul_transpose, n, m, k);
  printf("T-value = 25: "), 
  bench_matmul("matmul_transpose_parallel", runs, matmul_transpose_parallel, n, m, k);

  n = 1200; 
  m = 1200; 
  k = 1200; 
  runs = 10; 
  printf("\nRESULTS FOR: m=%d, n=%d, k=%d and runs == %d\n", m,n,k,runs); 
  bench_matmul("matmul", runs, matmul,  n, m, k);
  bench_matmul("matmul_locality", runs, matmul_locality, n, m, k);
  bench_matmul("matmul_parallel", runs, matmul_parallel, n, m, k);
  bench_matmul("matmul_transpose", runs, matmul_transpose, n, m, k);
  bench_matmul("matmul_locality_parallel", runs, matmul_locality_parallel, n, m, k);
  printf("T-value = 25: "), 
  bench_matmul("matmul_transpose_parallel", runs, matmul_transpose_parallel, n, m, k);

  n = 500; 
  m = 500; 
  k = 500; 
  runs = 10; 
  bench_matmul("matmul_locality_parallel", runs, matmul_locality_parallel, n, m, k);
  printf("T-value = 25: "), 
  bench_matmul("matmul_transpose_parallel", runs, matmul_transpose_parallel, n, m, k);
  }

  ///////////// MODI TEST //////////////////////////////////////////////////////
  if (MODI_ACTIVE==1){

  //TESTING FOR BEST T-VALUE
  n = 2520; 
  m = n; 
  runs = 500; 
  printf("\nRESULTS FOR: (m=%d)==(n=%d) and runs == %d\n", m,n,runs); 
  for (int i =1; i<n; i++){
    if( !(n % i))
    {
      T=i;
      printf("T=%d  ", T); 
      bench_transpose("transpose_blocked", runs, transpose_blocked, n, m, T);
    }
  }

  //TEST FOR COMPARASON OF BEST TRANSPOSE FUNCTION FOR INCREASED INPUT
  n = 500 * threads; 
  m = 20000; 
  T = 100; 
  runs = 100; 
  bench_transpose("transpose_parallel", runs, transpose_parallel, n, m, T);
  bench_transpose("transpose_blocked_parallel", runs, transpose_blocked_parallel, n, m, T);
  
  //TEST FOR STRONG SCALING WITH FIXED INPUT
  n = 20000; 
  m = 20000; 
  T = 50; 
  runs = 10; 
  bench_transpose("transpose_blocked", runs, transpose_blocked, n, m, T);
  bench_transpose("transpose_blocked_parallel", runs, transpose_blocked_parallel, n, m, T);

  //TEST FOR WEAK SCALING WITH INCREASED INPUT 
  n = 400 * threads; 
  m = 10000; 
  T = 50; 
  runs = 10; 
  bench_transpose("transpose_blocked", runs, transpose_blocked, n, m, T);
  bench_transpose("transpose_blocked_parallel", runs, transpose_blocked_parallel, n, m, T);

  //TEST FOR COMPARASON OF BEST MATMUL FUNCTION FOR INCREASED INPUT
  n = 250 * threads; 
  m = 250 * threads; 
  k = 250 * threads; 
  T = 50; 
  runs = 10; 
  bench_matmul("matmul_parallel", runs, matmul_parallel, n, m, k);
  bench_matmul("matmul_locality_parallel", runs, matmul_locality_parallel, n, m, k);
  bench_matmul("matmul_transpose_parallel", runs, matmul_transpose_parallel, n, m, k);

  //TEST FOR STRONG SCALING WITH FIXED INPUT
  n = 2500 ; 
  m = 2500 ; 
  k = 2500 ; 
  runs = 10; 
  bench_matmul("matmul_transpose_parallel", runs, matmul_transpose_parallel, n, m, k);
  bench_matmul("matmul_locality", runs, matmul_locality, n, m, k);

  //TEST FOR WEAK SCALING WITH INCREASED INPUT 
  n = 50 * threads ; 
  m = 2000 ; 
  k = 2000 ; 
  runs = 10; 
  bench_matmul("matmul_transpose_parallel", runs, matmul_transpose_parallel, n, m, k);
  bench_matmul("matmul_locality", runs, matmul_locality, n, m, k);
  }


}



